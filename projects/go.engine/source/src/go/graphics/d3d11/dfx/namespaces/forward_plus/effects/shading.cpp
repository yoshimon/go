/*
    Go GPL Source Code
    Copyright (C) 2012 Gokhan Ozdogan.

    This file is part of the Go GPL Source Code ("Go Source Code").

    Go Source Code is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Go Source Code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Go Source Code.  If not, see <http://www.gnu.org/licenses/>.

    In addition, the Go Source Code is also subject to certain additional terms.
    You should have received a copy of these additional terms immediately
    following the terms and conditions of the GNU General Public License which
    accompanied the Go Source Code.  If not, please request a copy in writing
    from mailto:goekhan.oezdogan@hotmail.de.
 */


#include "go/stdafx.h"


#include "dfx/namespaces/forward_plus/effects/shading/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/forward_plus/effects/shading/effect_types.h"
#include "dfx/namespaces/forward_plus/effects/shading/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/forward_plus/effects/shading.h"

#include "go/graphics/d3d11/dfx/forward_plus_util.h"
#include "go/graphics/asset_management/material_manager.h"
#include "go/graphics/render_points.h"
#include "go/graphics/sky.h"
#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

void clear_shading_render_targets(go::dfx_effects::forward_plus::shading_parameters *parameters)
{
    go::the_gfx_renderer->clear_render_target(parameters->out.colorRT, 0, &parameters->in.sky->color.x);
    go::the_gfx_renderer->clear_render_target(parameters->out.albedoRT, 0, 0.f);
    go::the_gfx_renderer->clear_render_target(parameters->out.metallicRoughnessRT, 0, 0.f);
    go::the_gfx_renderer->clear_render_target(parameters->out.normalWSRT, 0, 0.f);
    go::the_gfx_renderer->clear_render_target(parameters->out.depthVSRT, 0, parameters->in.renderPoint->transform.frustum.farClip);
    go::the_gfx_renderer->clear_render_target(parameters->out.ambientOcclusionRT, 0, 1.0f);
}


void bind_shading_render_targets(go::dfx_effects::forward_plus::shading_parameters *parameters)
{
    go::the_gfx_renderer->bind_render_target(0, parameters->out.colorRT, 0);
    go::the_gfx_renderer->bind_render_target(1, parameters->out.albedoRT, 0);
    go::the_gfx_renderer->bind_render_target(2, parameters->out.metallicRoughnessRT, 0);
    go::the_gfx_renderer->bind_render_target(3, parameters->out.normalWSRT, 0);
    go::the_gfx_renderer->bind_render_target(4, parameters->out.depthVSRT, 0);
    go::the_gfx_renderer->bind_render_target(5, parameters->out.ambientOcclusionRT, 0);
    go::the_gfx_renderer->match_viewport(parameters->out.colorRT);
}


// ================================================================================ //
// dfx::forward_plus::shading functions
// ================================================================================ //

void dfx::forward_plus::shading::on_initialize(void *userData)
{
}


void dfx::forward_plus::shading::on_dispose()
{
}


void dfx::forward_plus::shading::on_resize()
{
}


void dfx::forward_plus::shading::on_begin_run(void *userData)
{
    // Reset the renderer
    go::the_gfx_renderer->bind_default_pipeline_state();

    // Bind input buffers
    auto parameters = (go::dfx_effects::forward_plus::shading_parameters *)userData;

    // Spherical lights
    {
        auto &sceneLight = parameters->in.sphericalLights;
        dfx_hlsl::forward_plus::shading::g_sphericalLightProps = sceneLight.parameters;
        dfx_hlsl::forward_plus::shading::g_sphericalLightProxyShapes = sceneLight.shapes;
        dfx_hlsl::forward_plus::shading::g_tileSphericalLightIndices = sceneLight.tileIndices;
    }

    // Disk lights
    {
        auto &sceneLight = parameters->in.diskLights;
        dfx_hlsl::forward_plus::shading::g_diskLightProps = sceneLight.parameters;
        dfx_hlsl::forward_plus::shading::g_diskLightProxyShapes = sceneLight.shapes;
        dfx_hlsl::forward_plus::shading::g_tileDiskLightIndices = sceneLight.tileIndices;
    }

    // Shadow maps
    {
        dfx_hlsl::forward_plus::shading::g_directionalSM = parameters->in.shadowMaps.directionalLight;
        dfx_hlsl::forward_plus::shading::g_pointLightSM = parameters->in.shadowMaps.pointLightAtlas->texture();
        dfx_hlsl::forward_plus::shading::g_spotLightSM = parameters->in.shadowMaps.spotLightAtlas->texture();
    }

    // Bind shaders
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &shaderCache = renderPath->renderer_helper().shader_cache();
    m_pass1VS = shaderCache.common.skyVS;
    m_pass1PS = shaderCache.materialShaders[(uint8_t)go::gfx_material_shader_id::unlit];
}


void dfx::forward_plus::shading::on_do_pass0(void *userData)
{
    auto parameters = (go::dfx_effects::forward_plus::shading_parameters *)userData;

    GO_PERF_BEGIN_EVENT("Fragment Shading");
    clear_shading_render_targets(parameters);
    bind_shading_render_targets(parameters);

    // NOTE: complementary Z!
    //go::the_gfx_renderer->clear_depth_stencil_buffer(parameters->out.depthStencilBuffer, 0, 0.0f);
    go::the_gfx_renderer->bind_depth_stencil_buffer(parameters->out.depthStencilBuffer, 0);
    
    auto &rasterizerStates = go::the_gfx_device->common_rasterizer_states();
    auto &depthStencilStates = go::the_gfx_device->common_depth_stencil_states();

    // Grab the Forward+ renderer
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &renderHelper = renderPath->renderer_helper();
    auto &shaderCache = renderHelper.shader_cache();

    // Submit the scene geometry
    go::dfx_util::forward_plus_render_pass renderPass;
    renderPass.materialDB = &parameters->assetContext->material_db();
    renderPass.mask = go::dfx_util::geometry_mask::all;
    renderPass.depthOnly = false;
    renderPass.writeDepth = false;
    renderPass.cmdBuffers = &parameters->in.renderPoint->commandBuffers;
    renderPass.unskinnedGeometryVS = shaderCache.common.unskinnedGeometryVS;
    renderPass.backFacesRS = &rasterizerStates.backFaces;
    renderPass.frontFacesRS = &rasterizerStates.frontFaces;
    renderPass.twoSidedRS = &rasterizerStates.twoSided;
    renderPass.depthTestWriteEnabledDSS = &depthStencilStates.greaterTestWrite;
    renderPass.depthTestEnabledDSS = &depthStencilStates.greaterEqualTestOnly;
    renderHelper.submit_draw_calls(renderPass);
    GO_PERF_END_EVENT;
}


void dfx::forward_plus::shading::on_do_pass1(void *userData)
{
    auto parameters = (go::dfx_effects::forward_plus::shading_parameters *)userData;

    // Only render if a valid sky is bound
    auto sky = parameters->in.sky;
    if(!sky || sky->material.empty())
    {
        return;
    }

    // Grab the Forward+ renderer
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &renderHelper = renderPath->renderer_helper();
    auto &shaderCache = renderHelper.shader_cache();

    // Lookup sky material
    auto &materialDB = parameters->assetContext->material_db();
    auto material = materialDB[sky->material];
    if(!material)
    {
        // No sky bound
        return;
    }

    // Manually load material
    auto &assetContext = parameters->assetContext;
    auto &materialMgr = assetContext->material_mgr();
    materialMgr.load_material_textures(material);

    auto &rasterizerStates = go::the_gfx_device->common_rasterizer_states();
    auto &depthStencilStates = go::the_gfx_device->common_depth_stencil_states();

    GO_PERF_BEGIN_EVENT("Sky");
    bind_shading_render_targets(parameters);

    // OM Depth
    go::the_gfx_renderer->bind_depth_stencil_buffer(parameters->out.depthStencilBuffer, 0);
    go::the_gfx_renderer->bind_depth_stencil_state(&depthStencilStates.greaterEqualTestOnly, 0);

    // OM
    go::the_gfx_renderer->bind_blend_state(nullptr);
    go::the_gfx_renderer->bind_rasterizer_state(&rasterizerStates.frontFaces);

    // IA
    auto &skyModel = parameters->assetContext->model_cache().unit_ico_sphere();
    auto &ib = skyModel.index_buffer();
    auto &vb = skyModel.unskinned_vertex_buffer();
    go::the_gfx_renderer->bind_vertex_buffer(0, &vb);
    go::the_gfx_renderer->bind_index_buffer(&ib, 0);

    // PS
    auto &texMgr = assetContext->texture_mgr();
    go::dfx_materials::bind_material(&texMgr, material, go::dfx_util::forward_plus_renderer::kUnlitMaterialTextureBindings);

    auto numIndices = ib.size() / ib.element_size();
    go::the_gfx_renderer->draw_indexed(numIndices, 0, 0);

    GO_PERF_END_EVENT;
}
