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


#include "dfx/namespaces/forward_plus/effects/prepass/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/forward_plus/effects/prepass/effect_types.h"
#include "dfx/namespaces/forward_plus/effects/prepass/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/forward_plus/effects/prepass.h"

#include "go/graphics/render_context.h"
#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

void cull_spherical_proxy_lights(
    uint32_t numLights,
    go::gfx_shader_resource *proxyShapes,
    go::gfx_shader_resource *tileLightIndices)
{
    // Update constant buffers
    auto cb = dfx_hlsl::forward_plus::prepass::g_constants;
    auto p = static_cast<dfx_hlsl::forward_plus::prepass::constants *>(cb->begin_update());
    {
        p->g_forwardPlusPrepassNumLightsToCull = numLights;
    };
    cb->end_update();

    // Bind the buffers
    go::the_gfx_renderer->bind_compute_shader_resource
    (
        dfx_hlsl::forward_plus::prepass::kRegister_lightSphereProxyShapes,
        proxyShapes,
        0
    );

    go::the_gfx_renderer->bind_compute_shader_uav
    (
        dfx_hlsl::forward_plus::prepass::kRegister_tileLightIndicesRW,
        tileLightIndices,
        0
    );

    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto numThreadGroups = renderPath->light_culling_thread_groups();
    go::the_gfx_renderer->dispatch_compute(numThreadGroups.x, numThreadGroups.y, 1);
}


// ================================================================================ //
// dfx::forward_plus::prepass functions
// ================================================================================ //

void dfx::forward_plus::prepass::on_initialize(void *userData)
{
    dfx_hlsl::forward_plus::prepass::g_constants = new go::gfx_constant_buffer(sizeof(dfx_hlsl::forward_plus::prepass::constants));
}


void dfx::forward_plus::prepass::on_dispose()
{
    GO_SAFE_DELETE(dfx_hlsl::forward_plus::prepass::g_constants);
}


void dfx::forward_plus::prepass::on_resize()
{
}


void dfx::forward_plus::prepass::on_begin_run(void *userData)
{
    // Reset the renderer
    go::the_gfx_renderer->bind_default_pipeline_state();

    // Bind shaders
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &shaderCache = renderPath->renderer_helper().shader_cache();
    m_pass0CS = shaderCache.cullSphericalLightsCS;
}


void dfx::forward_plus::prepass::on_do_pass0(void *userData)
{
    auto parameters = (go::dfx_effects::forward_plus::prepass_parameters *)userData;

    GO_PERF_BEGIN_EVENT("Tile-based Light Culling");
    // Spherical
    cull_spherical_proxy_lights(
        parameters->in.numSphericalLights,
        parameters->in.sphericalLightProxyShapes,
        parameters->out.sphericalLightTileIndices);
    
    // Disk
    cull_spherical_proxy_lights(
        parameters->in.numDiskLights,
        parameters->in.diskLightProxyShapes,
        parameters->out.diskLightTileIndices);
    GO_PERF_END_EVENT;
}


void dfx::forward_plus::prepass::on_do_pass1(void *userData)
{
	auto parameters = (go::dfx_effects::forward_plus::prepass_parameters *)userData;

	GO_PERF_BEGIN_EVENT("Z-Prepass");
	// NOTE: complementary Z!
	go::the_gfx_renderer->clear_depth_stencil_buffer(parameters->out.depthStencilBuffer, 0, 0.0f);
	go::the_gfx_renderer->bind_depth_stencil_buffer(parameters->out.depthStencilBuffer, 0);
	go::the_gfx_renderer->match_viewport(parameters->out.depthStencilBuffer);

	auto &rasterizerStates = go::the_gfx_device->common_rasterizer_states();
	auto &depthStencilStates = go::the_gfx_device->common_depth_stencil_states();

	// Grab the Forward+ renderer
	auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
	auto &renderHelper = renderPath->renderer_helper();
	auto &shaderCache = renderHelper.shader_cache();

	// Submit the scene geometry
	go::dfx_util::forward_plus_render_pass renderPass;
	renderPass.materialDB = &parameters->assetContext->material_db();
	renderPass.mask = go::dfx_util::geometry_mask::opaque_only;
	renderPass.depthOnly = true;
	renderPass.writeDepth = true;
	renderPass.cmdBuffers = &parameters->in.renderPoint->commandBuffers;
	renderPass.unskinnedGeometryVS = shaderCache.common.unskinnedDepthGeometryVS;
	renderPass.backFacesRS = &rasterizerStates.backFaces;
	renderPass.frontFacesRS = &rasterizerStates.frontFaces;
	renderPass.twoSidedRS = &rasterizerStates.twoSided;
	renderPass.depthTestWriteEnabledDSS = &depthStencilStates.greaterTestWrite;
	renderPass.depthTestEnabledDSS = &depthStencilStates.greaterTestOnly;
	renderHelper.submit_draw_calls(renderPass);
	GO_PERF_END_EVENT;
}
