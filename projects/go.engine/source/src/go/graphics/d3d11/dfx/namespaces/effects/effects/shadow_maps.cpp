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

#include "dfx/namespaces/effects/effects/shadow_maps/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/effects/effects/shadow_maps/effect_types.h"
#include "dfx/namespaces/effects/effects/shadow_maps/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/effects/shadow_maps.h"

#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"
#include "go/graphics/render_points.h"


using namespace DirectX;


 // ================================================================================ //
 // Helper functions
 // ================================================================================ //

void update_gpu_constants(const DirectX::XMFLOAT4X4 &viewProj)
{
    auto cb = dfx_hlsl::effects::shadow_maps::g_constants;
    auto p = (dfx_hlsl::effects::shadow_maps::constants *)cb->begin_update();
    {
        auto mVP = XMLoadFloat4x4(&viewProj);
        XMStoreFloat4x4((XMFLOAT4X4 *)&p->g_currentLightViewProj, XMMatrixTranspose(mVP));
    }
    cb->end_update();
}


// ================================================================================ //
// dfx::effects::shadow_maps member functions
// ================================================================================ //

void dfx::effects::shadow_maps::on_initialize(void *userData)
{
    dfx_hlsl::effects::shadow_maps::g_constants = new go::gfx_constant_buffer(sizeof(dfx_hlsl::effects::shadow_maps::constants));
}


void dfx::effects::shadow_maps::on_dispose()
{
    GO_SAFE_DELETE(dfx_hlsl::effects::shadow_maps::g_constants);
}


void dfx::effects::shadow_maps::on_resize()
{
}


void dfx::effects::shadow_maps::on_begin_run(void *userData)
{
    // Reset the renderer
    go::the_gfx_renderer->bind_default_pipeline_state();
}


void dfx::effects::shadow_maps::on_do_pass0(void *userData)
{
    auto &&parameters = (go::dfx_effects::shadow_maps_parameters *)userData;

    GO_PERF_BEGIN_EVENT("Spot-Light Shadow Maps");
    auto &&shadowMapAtlas = parameters->out.spotLightShadowMapAtlas;

    // Reset the atlas
    shadowMapAtlas->free_all(false);

    // Clear the underlying depth buffer
    auto &&shadowMap = shadowMapAtlas->texture();
    go::the_gfx_renderer->clear_depth_stencil_buffer(shadowMap, 0, 1.0f);

    // Setup the render pass
    auto &&rasterizerStates = go::the_gfx_device->common_rasterizer_states();
    auto &&depthStencilStates = go::the_gfx_device->common_depth_stencil_states();

    // Bind the shadow map
    go::the_gfx_renderer->bind_depth_stencil_buffer(shadowMap, 0);

    // Grab the Forward+ renderer
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &&renderHelper = renderPath->renderer_helper();
    auto &&shaderCache = renderHelper.shader_cache();

    go::dfx_util::forward_plus_render_pass renderPass;
    renderPass.materialDB = &parameters->assetContext->material_db();
    renderPass.mask = go::dfx_util::geometry_mask::all;
    renderPass.depthOnly = true;
    renderPass.writeDepth = true;
    renderPass.unskinnedGeometryVS = shaderCache.shadowMapsEffect.unskinnedGeometryVS;
    renderPass.backFacesRS = &rasterizerStates.shadow;
    renderPass.frontFacesRS = &rasterizerStates.shadow;
    renderPass.twoSidedRS = &rasterizerStates.shadowTwoSided;
    renderPass.depthTestWriteEnabledDSS = &depthStencilStates.lessTestWrite;
    renderPass.depthTestEnabledDSS = &depthStencilStates.lessTestOnly;

    // NOTE: we assume a row major layout of the shadow maps
    GO_ASSERT(shadowMapAtlas->info().numColumns == 1);

    // Render each light
    for(auto &&light : *parameters->in.spotLightRenderPoints)
    {
        // The first inactive light indicates the end of the list
        if(!light.isActive)
        {
            break;
        }

        // This should never happen
        GO_ASSERT(shadowMapAtlas->is_full());

        // Allocate a new shadow map
        int32_t shadowMapIndex;
        shadowMapAtlas->allocate(&shadowMapIndex);

        // Actually render into it if the face is active
        if(!light.commandBuffers.is_empty())
        {
            GO_PERF_BEGIN_EVENT("Light");
            auto &&rect = shadowMapAtlas->px_bounds_of(shadowMapIndex);

            // Adjust the viewport to this face
            go::the_gfx_renderer->match_viewport(rect);

            // Update constants
            update_gpu_constants(light.transform.viewProjectionMatrix);

            // Submit the draw calls
            renderPass.cmdBuffers = &light.commandBuffers;
            renderHelper.submit_draw_calls(renderPass);
            GO_PERF_END_EVENT;
        }
    }
    GO_PERF_END_EVENT;
}


void dfx::effects::shadow_maps::on_do_pass1(void *userData)
{
    auto &&parameters = (go::dfx_effects::shadow_maps_parameters *)userData;

    GO_PERF_BEGIN_EVENT("Point-Light Shadow Maps");
    auto &&shadowMapAtlas = parameters->out.pointLightShadowMapAtlas;

    // Reset the atlas
    shadowMapAtlas->free_all(false);

    // Clear the underlying depth buffer
    auto &&shadowMap = shadowMapAtlas->texture();
    go::the_gfx_renderer->clear_depth_stencil_buffer(shadowMap, 0, 1.0f);

    // Setup the render pass
    auto &&rasterizerStates = go::the_gfx_device->common_rasterizer_states();
    auto &&depthStencilStates = go::the_gfx_device->common_depth_stencil_states();

    // Bind the shadow map
    go::the_gfx_renderer->bind_depth_stencil_buffer(shadowMap, 0);

    // Grab the Forward+ renderer
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &&renderHelper = renderPath->renderer_helper();
    auto &&shaderCache = renderHelper.shader_cache();

    go::dfx_util::forward_plus_render_pass renderPass;
    renderPass.materialDB = &parameters->assetContext->material_db();
    renderPass.mask = go::dfx_util::geometry_mask::all;
    renderPass.depthOnly = true;
    renderPass.writeDepth = true;
    renderPass.unskinnedGeometryVS = shaderCache.shadowMapsEffect.unskinnedGeometryVS;
    renderPass.backFacesRS = &rasterizerStates.shadow;
    renderPass.frontFacesRS = &rasterizerStates.shadow;
    renderPass.twoSidedRS = &rasterizerStates.shadowTwoSided;
    renderPass.depthTestWriteEnabledDSS = &depthStencilStates.lessTestWrite;
    renderPass.depthTestEnabledDSS = &depthStencilStates.lessTestOnly;

    // NOTE: we assume a column major layout of the shadow maps
    GO_ASSERT(shadowMapAtlas->info().numColumns == 6);

    // Render each point light
    {
        auto renderPointIndex = 0;
        for(auto &&light : *parameters->in.pointLightRenderPoints)
        {
            // The first inactive light indicates the end of the list
            if(!light.isActive)
            {
                break;
            }

            // This should never happen
            GO_ASSERT(shadowMapAtlas->is_full());

            auto firstEvent = true;
            auto endEvent = false;

            // Render each face
            for(size_t faceIndex = 0; faceIndex < 6; ++faceIndex)
            {
                // Allocate a new shadow map
                int32_t shadowMapIndex;
                shadowMapAtlas->allocate(&shadowMapIndex);

                auto &&faceCmdBuffers = &light.faceCommandBuffers[faceIndex];

                // Actually render into it if the face is active
                if(light.isFaceActive[faceIndex] && !faceCmdBuffers->is_empty())
                {
                    if(firstEvent)
                    {
                        GO_PERF_BEGIN_EVENT("Light");
                        firstEvent = false;
                        endEvent = true;
                    }

                    auto &&rect = shadowMapAtlas->px_bounds_of(shadowMapIndex);

                    // Adjust the viewport to this face
                    go::the_gfx_renderer->match_viewport(rect);

                    // Update constants
                    update_gpu_constants(light.faceTransforms[faceIndex].viewProjectionMatrix);

                    // Submit the draw calls
                    renderPass.cmdBuffers = faceCmdBuffers;
                    renderHelper.submit_draw_calls(renderPass);
                }
            }

            if(endEvent)
            {
                GO_PERF_END_EVENT;
            }

            ++renderPointIndex;
        }
    }
    GO_PERF_END_EVENT;
}


void dfx::effects::shadow_maps::on_do_pass2(void *userData)
{
    auto parameters = (go::dfx_effects::shadow_maps_parameters *)userData;

    GO_PERF_BEGIN_EVENT("Directional-Light Shadow Maps");
    // Clear Z
    go::the_gfx_renderer->clear_depth_stencil_buffer(parameters->out.directionalLightShadowMap, 0, 1.0f);
    
    // Bind Z
    go::the_gfx_renderer->bind_depth_stencil_buffer(parameters->out.directionalLightShadowMap, 0);
    go::the_gfx_renderer->match_viewport(parameters->out.directionalLightShadowMap);

    auto &rasterizerStates = go::the_gfx_device->common_rasterizer_states();
    auto &depthStencilStates = go::the_gfx_device->common_depth_stencil_states();

    // Update constants
    update_gpu_constants(parameters->in.directionalLightRenderPoint->transform.viewProjectionMatrix);

    // Grab the Forward+ renderer
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &renderHelper = renderPath->renderer_helper();
    auto &shaderCache = renderHelper.shader_cache();

    // Submit the scene geometry
    go::dfx_util::forward_plus_render_pass renderPass;
    renderPass.materialDB = &parameters->assetContext->material_db();
    renderPass.mask = go::dfx_util::geometry_mask::all;
    renderPass.depthOnly = true;
    renderPass.writeDepth = true;
    renderPass.cmdBuffers = &parameters->in.directionalLightRenderPoint->commandBuffers;
    renderPass.unskinnedGeometryVS = shaderCache.shadowMapsEffect.unskinnedGeometryVS;
    renderPass.backFacesRS = &rasterizerStates.shadowDirectional;
    renderPass.frontFacesRS = &rasterizerStates.shadowDirectional;
    renderPass.twoSidedRS = &rasterizerStates.shadowDirectionalTwoSided;
    renderPass.depthTestWriteEnabledDSS = &depthStencilStates.lessTestWrite;
    renderPass.depthTestEnabledDSS = &depthStencilStates.lessTestOnly;
    renderHelper.submit_draw_calls(renderPass);
    GO_PERF_END_EVENT;
}
