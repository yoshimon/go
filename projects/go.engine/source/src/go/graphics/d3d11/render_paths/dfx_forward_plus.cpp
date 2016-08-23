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


#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


#include "go/variables.h"
#include "go/core/timer.h"
#include "go/core/performance_counter.h"
#include "go/graphics/render_context.h"

#include "go/graphics/d3d11/dfx/go_types.h"
#include "dfx/dfx.h"
#include "dfx/namespaces/constants/namespace_types.h"
#include "dfx/namespaces/resources/namespaces/material_constants/namespace.h"
#include "dfx/namespaces/forward_plus/namespaces/constants/namespace_types.h"

// Effect parameters
#include "go/graphics/d3d11/dfx/namespaces/effects/dof.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/film_grain.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/fxaa.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/gaussian_blur.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/halfres_upsample.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/hdr_bloom.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/min_z_downsampling.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/overlay.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/shadow_maps.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/ssao.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/tonemapping.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/vignette.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/volumetric_lighting.h"

#include "go/graphics/d3d11/dfx/namespaces/forward_plus/effects/ambient_lighting.h"
#include "go/graphics/d3d11/dfx/namespaces/forward_plus/effects/prepass.h"
#include "go/graphics/d3d11/dfx/namespaces/forward_plus/effects/shading.h"


using namespace DirectX;


// ================================================================================ //
// Helper functions
// ================================================================================ //

template<class LightType, class GPUBuffers>
void update_light_buffers(const LightType &inLight, GPUBuffers &gpuBuffers)
{
    auto paramSize = (uint32_t)sizeof(decltype(inLight.parameters)::value_type);
    auto shapeSize = (uint32_t)sizeof(decltype(inLight.shapes)::value_type);

    gpuBuffers.parameters.update(inLight.parameters.data(), inLight.parameters.size() * paramSize);
    gpuBuffers.shapes.update(inLight.shapes.data(), inLight.shapes.size() * shapeSize);
}


// ================================================================================ //
// go::forward_plus_render_path member functions
// ================================================================================ //

go::dfx_forward_plus_render_path::dfx_forward_plus_render_path(gfx_renderer *renderer)
{
    // Initialize caches
    {
        gfx_texture2d_regular_atlas_info atlasInfo;
        atlasInfo.numColumns = 6;
        atlasInfo.numRows = DFX_MAX_NUM_SHADOW_CASTING_POINT_LIGHTS;
        atlasInfo.elementWidth = DFX_POINT_LIGHT_SHADOW_MAP_ATLAS_PADDED_FACE_SIZE;
        atlasInfo.elementHeight = atlasInfo.elementWidth;
        atlasInfo.colorFormat = gfx_color_format::d32_float;
        m_shadowMaps.pointLightAtlas.reset(atlasInfo);
    }

    {
        gfx_texture2d_regular_atlas_info atlasInfo;
        atlasInfo.numColumns = 1;
        atlasInfo.numRows = DFX_MAX_NUM_SHADOW_CASTING_SPOT_LIGHTS;
        atlasInfo.elementWidth = DFX_SPOT_LIGHT_SHADOW_MAP_ATLAS_PADDED_FACE_SIZE;
        atlasInfo.elementHeight = atlasInfo.elementWidth;
        atlasInfo.colorFormat = gfx_color_format::d32_float;
        m_shadowMaps.spotLightAtlas.reset(atlasInfo);
    }

    // Initialize all effects
    m_effects.bilateralBlur.initialize(nullptr);
    m_effects.dof.initialize(nullptr);
    m_effects.filmGrain.initialize(nullptr);
    m_effects.fxaa.initialize(nullptr);
    m_effects.gaussianBlur.initialize(nullptr);
    m_effects.halfresUpsample.initialize(nullptr);
    m_effects.hdrBloom.initialize(nullptr);
    m_effects.minZDownsampling.initialize(nullptr);
    m_effects.overlay.initialize(nullptr);
    m_effects.shadowMaps.initialize(nullptr);
    m_effects.ssao.initialize(nullptr);
    m_effects.tonemapping.initialize(nullptr);
    m_effects.vignette.initialize(nullptr);
    m_effects.volumetricLighting.initialize(nullptr);

    m_effects.forwardPlus.ambientLighting.initialize(nullptr);
    m_effects.forwardPlus.prepass.initialize(nullptr);
    m_effects.forwardPlus.shading.initialize(nullptr);

    // No material DB bound
    m_currentMaterialDB = nullptr;

    reset_render_targets();
    reset_buffers();
}


go::dfx_forward_plus_render_path::~dfx_forward_plus_render_path()
{
    m_effects.bilateralBlur.dispose();
    m_effects.dof.dispose();
    m_effects.filmGrain.dispose();
    m_effects.fxaa.dispose();
    m_effects.gaussianBlur.dispose();
    m_effects.halfresUpsample.dispose();
    m_effects.hdrBloom.dispose();
    m_effects.minZDownsampling.dispose();
    m_effects.overlay.dispose();
    m_effects.shadowMaps.dispose();
    m_effects.ssao.dispose();
    m_effects.tonemapping.dispose();
    m_effects.vignette.dispose();
    m_effects.volumetricLighting.dispose();

    m_effects.forwardPlus.ambientLighting.dispose();
    m_effects.forwardPlus.prepass.dispose();
    m_effects.forwardPlus.shading.dispose();
}


auto go::dfx_forward_plus_render_path::effects() -> effects_t &
{
    return m_effects;
}


DirectX::XMUINT2 go::dfx_forward_plus_render_path::light_culling_thread_groups() const
{
    DirectX::XMUINT2 result;
    auto &nativeFmt = go::the_gfx_device->back_buffer()->format();
    result.x = go::ceil_div(nativeFmt.width, DFX_FORWARD_PLUS_CULL_LIGHTS_NUM_LOCAL_THREADS_X);
    result.y = go::ceil_div(nativeFmt.height, DFX_FORWARD_PLUS_CULL_LIGHTS_NUM_LOCAL_THREADS_Y);
    return result;
}


void go::dfx_forward_plus_render_path::on_resize()
{
    m_effects.bilateralBlur.on_resize();
    m_effects.dof.on_resize();
    m_effects.filmGrain.on_resize();
    m_effects.fxaa.on_resize();
    m_effects.gaussianBlur.on_resize();
    m_effects.halfresUpsample.on_resize();
    m_effects.hdrBloom.on_resize();
    m_effects.minZDownsampling.on_resize();
    m_effects.overlay.on_resize();
    m_effects.shadowMaps.on_resize();
    m_effects.ssao.on_resize();
    m_effects.tonemapping.on_resize();
    m_effects.vignette.on_resize();
    m_effects.volumetricLighting.on_resize();

    m_effects.forwardPlus.ambientLighting.on_resize();
    m_effects.forwardPlus.prepass.on_resize();
    m_effects.forwardPlus.shading.on_resize();

    reset_render_targets();
    resize_buffers();
}


void go::dfx_forward_plus_render_path::reset_render_targets()
{
    auto &backBufferFmt = go::the_gfx_device->back_buffer()->format();

    // G-Buffer
    {
        auto fmt = backBufferFmt;

        // Recreate all render-targets
        fmt.colorFormat = gfx_color_format::b8g8r8a8_unorm;
        m_gbuffer.albedo.reset(fmt);

        fmt.colorFormat = gfx_color_format::r16g16b16a16_float;
        m_gbuffer.normalWS.reset(fmt);

        fmt.colorFormat = gfx_color_format::r16g16b16a16_float;
        m_gbuffer.color.reset(fmt);

        fmt.colorFormat = gfx_color_format::b8g8r8a8_unorm;
        m_gbuffer.metallicRoughness.reset(fmt);

        auto depthVSFmt = fmt;
        depthVSFmt.flags = go::gfx_texture::kFlagUAV;
        depthVSFmt.mipLevels = 2;
        depthVSFmt.srvMipLevels = 1; // Only top-most mip readable
        depthVSFmt.uavMipLevels = depthVSFmt.mipLevels; // half-resolution depth
        depthVSFmt.colorFormat = gfx_color_format::r16_float;
        m_gbuffer.depthVS.reset(depthVSFmt);

        // Just grab the device back-buffer
        m_gbuffer.depthBuffer = go::the_gfx_device->depth_buffer();

        fmt.colorFormat = gfx_color_format::r16_float;
        m_gbuffer.ambientOcclusion.reset(fmt);
    }

    auto &hdrFmt = m_gbuffer.color.format();

    // Misc RTs
    {
        // HDR
        {
            auto fmt = hdrFmt;
            fmt.flags = gfx_texture::kFlagUAV;
            fmt.width = go::ceil_div(fmt.width, 2);
            fmt.height = go::ceil_div(fmt.height, 2);
            fmt.mipLevels = dfx_effects::hdr_bloom_parameters::kNumBlurPasses;
            fmt.srvMipLevels = fmt.mipLevels;
            fmt.uavMipLevels = fmt.mipLevels;
            m_hdrBuffers.bloom.reset(fmt);
        }

        // Volumetric lighting
        {
            auto fmt = hdrFmt;
            fmt.flags = gfx_texture::kFlagUAV;
            fmt.width = go::ceil_div(fmt.width, 2);
            fmt.height = go::ceil_div(fmt.height, 2);
            fmt.srvMipLevels = fmt.mipLevels;
            fmt.uavMipLevels = fmt.mipLevels;
            m_volumetricLightingBuffers.output.reset(fmt);
        }
    }

    // HDREffect buffers
    {
        {
            auto fmt = hdrFmt;
            fmt.flags = gfx_texture::kFlagUAV;
            fmt.width = go::ceil_div(fmt.width, 2);
            fmt.height = go::ceil_div(fmt.height, 2);
            fmt.mipLevels = dfx_effects::hdr_bloom_parameters::kNumBlurPasses;
            fmt.srvMipLevels = fmt.mipLevels;
            fmt.uavMipLevels = fmt.mipLevels;
            m_hdrBuffers.blurredBrightnessScratch.reset(fmt);
            m_hdrBuffers.blurredBrightness.reset(fmt);
            m_hdrBuffers.brightness.reset(fmt);
        }

        {
            auto brightnessFmt = m_hdrBuffers.brightness.format();
            auto w = go::ceil_div(go::ceil_div(brightnessFmt.width, 2), DFX_HDR_BLOOM_LUM_AND_BRIGHT_THREADS);
            auto h = go::ceil_div(go::ceil_div(brightnessFmt.height, 2), DFX_HDR_BLOOM_LUM_AND_BRIGHT_THREADS);
            auto fmt = gfx_texture_format::as_2d(w, h, gfx_color_format::r32_float);
            fmt.flags = gfx_texture::kFlagUAV;
            m_hdrBuffers.luminance.reset(fmt);

            fmt.width = 1;
            fmt.height = 1;
            m_hdrBuffers.lastLuminance.reset(fmt);
        }
    }

    // Temp buffers
    {
        auto fmt = backBufferFmt;
        m_tempBuffers.backBufferMirror.reset(fmt);
    }

    // Shadow maps
    {
        auto fmt = gfx_texture_format::as_2d(DFX_DIRECTIONAL_LIGHT_SHADOW_MAP_SIZE, DFX_DIRECTIONAL_LIGHT_SHADOW_MAP_SIZE, gfx_color_format::d32_float);
        m_shadowMaps.directionalLight.reset(fmt);
    }
}


void go::dfx_forward_plus_render_path::reset_buffers()
{
    // Recreate all buffers
    m_buffers.diskLights.parameters.reset(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE, sizeof(go::gfx_disk_lights::parameters_t), gfx_access_policy::read_only);
    m_buffers.diskLights.shapes.reset(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE, sizeof(go::gfx_disk_lights::proxy_shape_t), gfx_access_policy::read_only);

    m_buffers.sphericalLights.parameters.reset(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE, sizeof(go::gfx_spherical_lights::parameters_t), gfx_access_policy::read_only);
    m_buffers.sphericalLights.shapes.reset(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE, sizeof(go::gfx_spherical_lights::proxy_shape_t), gfx_access_policy::read_only);

    resize_buffers();
}


void go::dfx_forward_plus_render_path::resize_buffers()
{
    auto &nativeFmt = go::the_gfx_device->back_buffer()->format();

    {
        auto numThreadGroups = light_culling_thread_groups();
        auto numTiles = numThreadGroups.x * numThreadGroups.y;
        m_buffers.diskLights.tileIndices.reset(DFX_FORWARD_PLUS_MAX_NUM_ELEMENTS_PER_TILE * numTiles, sizeof(uint32_t), gfx_access_policy::read_write, gfx_buffer_update_frequency::high, gfx_structured_buffer_data_type::native);
        m_buffers.sphericalLights.tileIndices.reset(DFX_FORWARD_PLUS_MAX_NUM_ELEMENTS_PER_TILE * numTiles, sizeof(uint32_t), gfx_access_policy::read_write, gfx_buffer_update_frequency::high, gfx_structured_buffer_data_type::native);
    }
}


void go::dfx_forward_plus_render_path::update_per_frame_constants(const gfx_render_context &context)
{
    // Update CB
    auto &fmt = the_gfx_device->back_buffer()->format();

    {
        // Scale + bias matrix for clip-space to UV space transform
        auto mTexScaleBias = XMMatrixScaling(0.5f, -0.5f, 1.0f);
        mTexScaleBias *= XMMatrixTranslation(0.5f, 0.5f, 0.0f);

        auto cb = dfx_hlsl::constants::g_frame;
        auto p = static_cast<dfx_hlsl::constants::frame *>(cb->begin_update());
        {
            //
            // Main render point
            //
            auto &mainRenderPointTransforms = context.renderPoints.main.transform;

            // View
            auto mainRPView = DirectX::XMLoadFloat4x4(&mainRenderPointTransforms.viewMatrix);
            auto mainRPInvView = DirectX::XMMatrixInverse(nullptr, mainRPView);

            // Proj
            auto mainRPProj = DirectX::XMLoadFloat4x4(&mainRenderPointTransforms.projectionMatrix);
            auto mainRPInvProj = DirectX::XMMatrixInverse(nullptr, mainRPProj);

            // Combined
            auto mainRPViewProj = DirectX::XMLoadFloat4x4(&mainRenderPointTransforms.viewProjectionMatrix);

            XMStoreFloat3x4((XMFLOAT4X3 *)p->g_mainRenderPointView, DirectX::XMMatrixTranspose(mainRPView));
            XMStoreFloat4x4((XMFLOAT4X4 *)p->g_mainRenderPointInvView, DirectX::XMMatrixTranspose(mainRPInvView));
            XMStoreFloat4x4((XMFLOAT4X4 *)p->g_mainRenderPointViewProj, XMMatrixTranspose(mainRPViewProj));
            XMStoreFloat4x4((XMFLOAT4X4 *)p->g_mainRenderPointInvProjection, XMMatrixTranspose(mainRPInvProj));
            p->g_mainRenderPointPositionWS[0] = mainRenderPointTransforms.frustum.position.x;
            p->g_mainRenderPointPositionWS[1] = mainRenderPointTransforms.frustum.position.y;
            p->g_mainRenderPointPositionWS[2] = mainRenderPointTransforms.frustum.position.z;
            p->g_mainRenderPointPositionWS[3] = 1.0f;
            p->g_mainRenderPointNearClip = mainRenderPointTransforms.frustum.nearClip;
            p->g_mainRenderPointFarClip = mainRenderPointTransforms.frustum.farClip;

            // Inverse fov-aspect (stretch in xy by the projection)
            p->g_mainRenderPointInvFovAspect[0] = 1.0f / mainRenderPointTransforms.projectionMatrix._11;
            p->g_mainRenderPointInvFovAspect[1] = 1.0f / mainRenderPointTransforms.projectionMatrix._22;
            
            //
            // Directional light render point
            //
            auto &directionalLight = context.renderPoints.directionalLight[0].transform;

            auto vSunDir = -XMLoadFloat3(&context.sky.sunDirection);
            XMStoreFloat3((XMFLOAT3 *)p->g_directionalLightDirWS, vSunDir);

            auto vSunColor = XMLoadFloat3(&context.sky.sunColor);
            XMStoreFloat3((XMFLOAT3 *)p->g_directionalLightColor, vSunColor);

            // Combined
            auto vSunViewProj = DirectX::XMLoadFloat4x4(&directionalLight.viewProjectionMatrix);
            XMStoreFloat4x4((XMFLOAT4X4 *)p->g_directionalLightViewProj, XMMatrixTranspose(vSunViewProj));

            // Scale and bias to texture space
            auto vSunViewProjUV = vSunViewProj * mTexScaleBias;
            XMStoreFloat4x4((XMFLOAT4X4 *)p->g_directionalLightViewProjUV, XMMatrixTranspose(vSunViewProjUV));

            // Main RP to directional light
            auto vSunView = XMLoadFloat4x4(&directionalLight.viewMatrix);
            auto mainViewToDirectionalView = mainRPInvView * vSunView;
            auto mainViewToDirectionalViewProjUV = mainRPInvView * vSunViewProj * mTexScaleBias;
            XMStoreFloat4x4((XMFLOAT4X4 *)p->g_mainRenderPointViewToDirectionalLightView, XMMatrixTranspose(mainViewToDirectionalView));
            XMStoreFloat4x4((XMFLOAT4X4 *)p->g_mainRenderPointViewToDirectionalLightViewProjUV, XMMatrixTranspose(mainViewToDirectionalViewProjUV));


            //
            // Point lights
            //
            for(size_t i = 0, end = context.renderPoints.pointLights.max_size(); i < end; ++i)
            {
                auto &&light = context.renderPoints.pointLights[i];
                for(size_t j = 0; j < 6; ++j)
                {
                    auto &&faceTransformIndex = i * 6 + j;
                    auto &&vLightViewProj = XMLoadFloat4x4(&light.faceTransforms[j].viewProjectionMatrix);
                    auto &&vLightViewProjUV = vLightViewProj * mTexScaleBias;
                    XMStoreFloat4x4((XMFLOAT4X4 *)p->g_shadowCastingPointLightsViewProjectionUV[faceTransformIndex], XMMatrixTranspose(vLightViewProjUV));
                }
            }


            //
            // Spot lights
            //
            for(size_t i = 0, end = context.renderPoints.spotLights.max_size(); i < end; ++i)
            {
                auto &&light = context.renderPoints.spotLights[i];
                auto &&vLightViewProj = XMLoadFloat4x4(&light.transform.viewProjectionMatrix);
                auto &&vLightViewProjUV = vLightViewProj * mTexScaleBias;
                XMStoreFloat4x4((XMFLOAT4X4 *)p->g_shadowCastingSpotLightsViewProjectionUV[i], XMMatrixTranspose(vLightViewProjUV));
            }


            //
            // General rendering constants
            //
            // Resolution
            p->g_nativeResolution[0] = (float)fmt.width;
            p->g_nativeResolution[1] = (float)fmt.height;

            // rcp(Resolution)
            p->g_nativeResolutionRcp[0] = 1.0f / fmt.width;
            p->g_nativeResolutionRcp[1] = 1.0f / fmt.height;

            p->g_halfResolutionRcp[0] = 1.0f / (0.5f * fmt.width);
            p->g_halfResolutionRcp[1] = 1.0f / (0.5f * fmt.height);

            p->g_nativeTwoTexelSize[0] = 2.0f / p->g_nativeResolution[0];
            p->g_nativeTwoTexelSize[1] = -2.0f / p->g_nativeResolution[1];

            // TODO:
            p->g_gammaRcp = 1.f / go::clamp(GO_CVAR(display_gamma).valuef(), 0.1f, 4.0f);

            // TODO:
            p->g_randomUNorm = 1.0f;
            p->g_periodicSine = 1.0f;
            p->g_elapsedTimeMS = go::performance.elapsedTime;
        }
        cb->end_update();
    }

    // Forward+ CB
    {
        auto numThreadGroups = light_culling_thread_groups();
        
        auto cb = dfx_hlsl::forward_plus::constants::g_frame;
        auto p = static_cast<dfx_hlsl::forward_plus::constants::frame *>(cb->begin_update());
        {
            p->g_forwardPlusCullingNumThreadGroups[0] = numThreadGroups.x;
            p->g_forwardPlusCullingNumThreadGroups[1] = numThreadGroups.y;
        }
        cb->end_update();
    }
}


void go::dfx_forward_plus_render_path::allocate_shadow_maps(gfx_render_context &context)
{
    // Point lights
    {
        auto &&atlas = m_shadowMaps.pointLightAtlas;
        std::array<int32_t, DFX_MAX_NUM_SHADOW_CASTING_POINT_LIGHTS * 6> shadowMapIndices;
        {
            // Temporal coherence: try to find the same shadow maps from last frame
            {
                auto renderPointIndex = 0;
                for(auto &&light : context.renderPoints.pointLights)
                {
                    if(!light.isActive)
                    {
                        break;
                    }

                    // Update the shadow caster index
                    auto &&lightIndex = context.lights.shadowCasting.spherical[renderPointIndex];

                    // Try to find the shadow map if it was already rendered
                    for(size_t faceIndex = 0; faceIndex < 6; ++faceIndex)
                    {
                        // The shadow map slot
                        auto &&shadowMapSlot = renderPointIndex * 6 + faceIndex;

                        // Use the render point ID to calculate a unique shadow map ID
                        auto &&shadowMapID = light.id * 6 + faceIndex;

                        // Try to allocate the shadow map
                        int32_t shadowCasterIndex;
                        if(atlas.try_allocate_by_id(&shadowCasterIndex, shadowMapID))
                        {
                            // Save the index
                            shadowMapIndices[shadowMapSlot] = shadowCasterIndex;

                            // Calculate the row index
                            auto &&gpuShadowCasterIndex = shadowCasterIndex / 6;

                            // Update the GPU shadow caster index (row in the shadow map atlas)
                            auto &&kind = light.kind;

                            switch(kind)
                            {
                            case gfx_render_point_kind::spherical_light: context.lights.spherical.parameters[lightIndex].shadowCasterIndex = (float)gpuShadowCasterIndex;
                            default: GO_MSGASSERT(false, "Unhandled light type."); break;
                            }

                            // If the shadow map has changed we have to redraw
                            if(light.forceRedraw)
                            {
                                // Queue dirty region to be reset

                            }
                        }
                        else
                        {
                            // Don't pick a shadow map yet
                            shadowMapIndices[shadowMapSlot] = -1;
                        }
                    }

                    ++renderPointIndex;
                }
            }

            // Some shadow maps might be new: allocate them now that the existing ones have been resolved
            {
                auto renderPointIndex = 0;
                for(auto &&light : context.renderPoints.pointLights)
                {
                    if(!light.isActive)
                    {
                        break;
                    }

                    // Try to find the shadow map if it was already rendered
                    for(size_t faceIndex = 0; faceIndex < 6; ++faceIndex)
                    {
                        auto shadowMapID = light.id * 6 + faceIndex;
                        int32_t shadowCasterIndex;
                        if(atlas.allocate(&shadowCasterIndex, shadowMapID))
                        {
                            shadowMapIndices[renderPointIndex * 6 + faceIndex] = shadowCasterIndex;

                            if(light.forceRedraw)
                            {
                                // Queue dirty region to be reset
                            }
                        }
                    }

                    ++renderPointIndex;
                }
            }
        }
    }
}


void go::dfx_forward_plus_render_path::update_lights(const gfx_render_context &context)
{
    // Disk light
    update_light_buffers(context.lights.disk, m_buffers.diskLights);
    
    // Spherical light
    update_light_buffers(context.lights.spherical, m_buffers.sphericalLights);
}


void go::dfx_forward_plus_render_path::temporal_upload_material_db(go::gfx_material_database *materialDB)
{
    // Only update if they differ
    if(m_currentMaterialDB == materialDB)
    {
        return;
    }

    // Reset material DB buffers
    dfx::resources::material_constants::on_shutdown();
    dfx::resources::material_constants::on_initialize(materialDB);

    // Save it out
    m_currentMaterialDB = materialDB;
}


void go::dfx_forward_plus_render_path::render(
    gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    // NOTE: this makes asset context switches very bad, if the material database changes!
    temporal_upload_material_db(&assetContext.material_db());

    // Reset the helper
    m_helperRenderer.reset(&assetContext.texture_mgr(), &assetContext.shader_mgr());

    // Allocate all shadow maps
    // NOTE: this has to be done before the lighting data is uploaded to the GPU (below)
    // so that the shadow caster indices can be adjusted
    // allocate_shadow_maps(renderContext);

    // Update the global per-frame constant buffer
    update_per_frame_constants(renderContext);

    // Update lights
    update_lights(renderContext);

    // Execute the effects pipeline on the context
    GO_PERF_BEGIN_EVENT("Render");
    {
        // Begin the frame timer
        auto &&gpuTimers = m_gpuTimers.active();
        auto &&gpuTimerFrequency = m_gpuTimerFrequency.active();

        // Enable the timer if requested
        auto &&enableGPUProfiling = GO_CVAR(enable_gpu_profiling).valuei() == 1;
        gpuTimers.enable(enableGPUProfiling);
        gpuTimerFrequency.enable(enableGPUProfiling);

        // See if we can collect the previous frame
        auto &&prevFrameGPUTimers = m_gpuTimers.inactive();
        auto &&prevFrameGPUTimerFrequency = m_gpuTimerFrequency.inactive();
        auto &&gpuTimestamps = prevFrameGPUTimers.resolved_timestamps();
        if(m_gpuTimers.swap_count() > 0)
        {
            uint64_t frequency;
            if(prevFrameGPUTimerFrequency.wait_for(&frequency))
            {
                if(prevFrameGPUTimers.resolve_milliseconds(frequency))
                {
                    go::performance.gpu.shadowMaps = gpuTimestamps[0];
                    go::performance.gpu.prepass = gpuTimestamps[1];
                    go::performance.gpu.shading = gpuTimestamps[2];
                    go::performance.gpu.depthDownsampling = gpuTimestamps[3];
                    go::performance.gpu.hbao = gpuTimestamps[4];
                    go::performance.gpu.ambientLighting = gpuTimestamps[5];
                    go::performance.gpu.volumetricLighting = gpuTimestamps[6];
                    go::performance.gpu.hdrBloom = gpuTimestamps[7];
                    go::performance.gpu.tonemapping = gpuTimestamps[8];
                    go::performance.gpu.fxaa = gpuTimestamps[9];
                    go::performance.gpu.filmGrain = gpuTimestamps[10];
                    go::performance.gpu.vignette = gpuTimestamps[11];
                }
            }

            prevFrameGPUTimers.clear();
        }

        // Begin a new frame
        gpuTimerFrequency.begin();

        // Do the first query
        gpuTimers.add_event();

        // CPU start time
        uint64_t start;

        GO_PERF_BEGIN_EVENT("Shadow Maps");
        {
            start = go::timer::query_current_time_cycles();
            run_shadow_maps_effect(renderContext, assetContext);
            go::performance.cpu.shadowMaps = go::timer::query_delta_time_milliseconds(start);
            gpuTimers.add_event();
        }
        GO_PERF_END_EVENT;

        GO_PERF_BEGIN_EVENT("Forward+ Prepass & Shading");
        {
            start = go::timer::query_current_time_cycles();
            run_prepass_effect(renderContext, assetContext);
            go::performance.cpu.prepass = go::timer::query_delta_time_milliseconds(start);
            gpuTimers.add_event();

            start = go::timer::query_current_time_cycles();
            run_shading_effect(renderContext, assetContext);
            go::performance.cpu.shading = go::timer::query_delta_time_milliseconds(start);
            gpuTimers.add_event();
        }
        GO_PERF_END_EVENT;

        GO_PERF_BEGIN_EVENT("MinZ Depth Downsampling");
        {
            start = go::timer::query_current_time_cycles();
            run_min_z_downsample(renderContext, assetContext);
            go::performance.cpu.depthDownsampling = go::timer::query_delta_time_milliseconds(start);
            gpuTimers.add_event();
        }
        GO_PERF_END_EVENT;

        start = go::timer::query_current_time_cycles();
        run_ssao_effect(renderContext, assetContext);
        go::performance.cpu.hbao = go::timer::query_delta_time_milliseconds(start);
        gpuTimers.add_event();

        GO_PERF_BEGIN_EVENT("Ambient Lighting");
        {
            start = go::timer::query_current_time_cycles();
            run_ambient_lighting_effect(renderContext, assetContext);
            go::performance.cpu.ambientLighting = go::timer::query_delta_time_milliseconds(start);
            gpuTimers.add_event();
        }
        GO_PERF_END_EVENT;

        GO_PERF_BEGIN_EVENT("Volumetric Lighting");
        {
            start = go::timer::query_current_time_cycles();
            run_volumetric_lighting_effect(renderContext, assetContext);
            go::performance.cpu.volumetricLighting = go::timer::query_delta_time_milliseconds(start);
            gpuTimers.add_event();
        }
        GO_PERF_END_EVENT;

        GO_PERF_BEGIN_EVENT("HDR Bloom");
        {
            start = go::timer::query_current_time_cycles();
            run_hdr_bloom_effect(renderContext, assetContext);
            go::performance.cpu.hdrBloom = go::timer::query_delta_time_milliseconds(start);
            gpuTimers.add_event();
        }
        GO_PERF_END_EVENT;

        start = go::timer::query_current_time_cycles();
        run_tonemapping_effect(renderContext, assetContext);
        go::performance.cpu.tonemapping = go::timer::query_delta_time_milliseconds(start);
        gpuTimers.add_event();

        GO_PERF_BEGIN_EVENT("LDR Post-Processing");
        {
            start = go::timer::query_current_time_cycles();
            run_fxaa_effect(renderContext, assetContext);
            go::performance.cpu.fxaa = go::timer::query_delta_time_milliseconds(start);
            gpuTimers.add_event();

            start = go::timer::query_current_time_cycles();
            run_film_grain_effect(renderContext, assetContext);
            go::performance.cpu.filmGrain = go::timer::query_delta_time_milliseconds(start);
            gpuTimers.add_event();

            start = go::timer::query_current_time_cycles();
            run_vignette_effect(renderContext, assetContext);
            go::performance.cpu.vignette = go::timer::query_delta_time_milliseconds(start);
            gpuTimers.add_event();
            // run_dof_effect
            // run_motion_blur_effect
        }
        GO_PERF_END_EVENT;

        gpuTimerFrequency.end();

        // Swap the timers
        m_gpuTimers.swap();
        m_gpuTimerFrequency.swap();
    }
    GO_PERF_END_EVENT;
}


void go::dfx_forward_plus_render_path::run_prepass_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::forward_plus::prepass_parameters params;

    params.assetContext = &assetContext;
    params.in.renderPoint = &renderContext.renderPoints.main;
    params.out.depthStencilBuffer = m_gbuffer.depthBuffer;

    // Disk lights
    params.in.numDiskLights = renderContext.lights.disk.shapes.size();
    params.in.diskLightProxyShapes = &m_buffers.diskLights.shapes;
    params.out.diskLightTileIndices = &m_buffers.diskLights.tileIndices;

    // Spherical lights
    params.in.numSphericalLights = renderContext.lights.spherical.shapes.size();
    params.in.sphericalLightProxyShapes = &m_buffers.sphericalLights.shapes;
    params.out.sphericalLightTileIndices = &m_buffers.sphericalLights.tileIndices;
    
    m_effects.forwardPlus.prepass.run(&params);
}


void go::dfx_forward_plus_render_path::run_shading_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::forward_plus::shading_parameters params;
    params.assetContext = &assetContext;
    
    // Render points
    params.in.renderPoint = &renderContext.renderPoints.main;
    
    // Disk lights
    params.in.diskLights.parameters = &m_buffers.diskLights.parameters;
    params.in.diskLights.shapes = &m_buffers.diskLights.shapes;
    params.in.diskLights.tileIndices = &m_buffers.diskLights.tileIndices;

    // Spherical lights
    params.in.sphericalLights.parameters = &m_buffers.sphericalLights.parameters;
    params.in.sphericalLights.shapes = &m_buffers.sphericalLights.shapes;
    params.in.sphericalLights.tileIndices = &m_buffers.sphericalLights.tileIndices;

    // Sky
    params.in.sky = &renderContext.sky;
    
    // G-Buffer
    params.out.colorRT = &m_gbuffer.color;
    params.out.albedoRT = &m_gbuffer.albedo;
    params.out.metallicRoughnessRT = &m_gbuffer.metallicRoughness;
    params.out.normalWSRT = &m_gbuffer.normalWS;
    params.out.depthVSRT = &m_gbuffer.depthVS;
    params.out.ambientOcclusionRT = &m_gbuffer.ambientOcclusion;

    params.out.depthStencilBuffer = m_gbuffer.depthBuffer;

    // Shadow maps
    params.in.shadowMaps.directionalLight = &m_shadowMaps.directionalLight;
    params.in.shadowMaps.pointLightAtlas = &m_shadowMaps.pointLightAtlas;
    params.in.shadowMaps.spotLightAtlas = &m_shadowMaps.spotLightAtlas;

    m_effects.forwardPlus.shading.run(&params);
}


void go::dfx_forward_plus_render_path::run_ssao_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::ssao_parameters params;
    
    params.in.angularBias = 0.5f;
    params.in.depthVSTexture = &m_gbuffer.depthVS;
    params.in.normalWSTexture = &m_gbuffer.normalWS;
    params.in.depthProjMatrix = renderContext.renderPoints.main.transform.projectionMatrix;
    params.in.depthViewMatrix = renderContext.renderPoints.main.transform.viewMatrix;
    params.in.metersToViewSpaceUnits = 1.0f;
    params.in.radius = 2.0f;
    params.in.strength = 3.0f;

    params.out.outputTexture = &m_gbuffer.ambientOcclusion;

    m_effects.ssao.run(&params);
}


void go::dfx_forward_plus_render_path::run_ambient_lighting_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    // Lookup the env BRDF
    auto &&envBRDFMat = assetContext.material_db().find("envbrdf");
    auto &&envBRDF = assetContext.texture_mgr().load(envBRDFMat->textureIDs[0], 0, go::resource_eviction_policy::permanent);

    dfx_effects::forward_plus::ambient_lighting_parameters params;

    params.in.ambientColorAboveWS = renderContext.sky.ambientColorAbove;
    params.in.ambientColorBelowWS = renderContext.sky.ambientColorBelow;
    params.in.aoTexture = &m_gbuffer.ambientOcclusion;
    params.in.normalWSTexture = &m_gbuffer.normalWS;
    params.in.albedoTexture = &m_gbuffer.albedo;
    params.in.metallicRoughnessTexture = &m_gbuffer.metallicRoughness;
    params.in.depthVSTexture = &m_gbuffer.depthVS;
    params.in.envBRDFTexture = *envBRDF;
    params.in.sceneSpecularEnvMap = renderContext.sky.specularEnvMap; // NOTE: the renderer modifies the binding

    // Additive blending
    params.out.colorTexture = &m_gbuffer.color;

    m_effects.forwardPlus.ambientLighting.run(&params);
}


void go::dfx_forward_plus_render_path::run_hdr_bloom_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::hdr_bloom_parameters params;

    auto &camProps = renderContext.renderPoints.main.cameraProperties;
    params.in.bloomThreshold = 0.9f;
    params.in.exposureAdaptionDelta = 0.001f;
    params.in.gaussianBlurEffect = &m_effects.gaussianBlur;
    params.in.hdrColorTexture = &m_gbuffer.color;
    params.in.lastLuminanceTexture = &m_hdrBuffers.lastLuminance;
    params.in.maxLuminance = 2.0f;
    params.in.middleGray = 0.5f;

    params.out.brightnessTexture = &m_hdrBuffers.brightness;
    params.out.hdrBloomTexture = &m_hdrBuffers.bloom;
    params.out.luminanceTexture = &m_hdrBuffers.luminance;
    params.out.blurredBrightnessScratchTexture = &m_hdrBuffers.blurredBrightnessScratch;
    params.out.blurredBrightnessTexture = &m_hdrBuffers.blurredBrightness;

    m_effects.hdrBloom.run(&params);
}


void go::dfx_forward_plus_render_path::run_tonemapping_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::tonemapping_parameters params;

    params.in.bloomScale = 0.5f;
    params.in.exposureBias = 2.5f;
    params.in.hdrTexture = &m_gbuffer.color;
    params.in.bloomTexture = &m_hdrBuffers.bloom;
    params.in.luminanceTexture = &m_hdrBuffers.luminance;

    // Load the color grading LUT
    auto &texDB = assetContext.texture_db();
    auto &texMgr = assetContext.texture_mgr();

    auto colorGradingSrcTex = texDB.find(renderContext.renderPoints.main.cameraProperties.colorGradingSourceTexture);
    params.in.colorLUTSrc = *texMgr.load(colorGradingSrcTex->id);
    
    auto colorGradingDestTex = texDB.find(renderContext.renderPoints.main.cameraProperties.colorGradingDestinationTexture);
    params.in.colorLUTDest = *texMgr.load(colorGradingDestTex->id);

    params.in.colorLUTBlendFactor = renderContext.renderPoints.main.cameraProperties.colorGradingBlendFactor;

    params.out.outputTexture = &m_tempBuffers.backBufferMirror;

    m_effects.tonemapping.run(&params);
}


void go::dfx_forward_plus_render_path::run_film_grain_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::film_grain_parameters params;

    params.in.strength = 0.95f;
    params.out.colorTexture = go::the_gfx_device->back_buffer();

    m_effects.filmGrain.run(&params);
}


void go::dfx_forward_plus_render_path::run_vignette_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::vignette_parameters params;

    params.in.radius = 0.6f;
    params.in.strength = 1.0f;
    params.out.outputTexture = go::the_gfx_device->back_buffer();

    m_effects.vignette.run(&params);
}


void go::dfx_forward_plus_render_path::run_shadow_maps_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::shadow_maps_parameters params;

    // Bind the shadow maps
    params.in.pointLightRenderPoints = &renderContext.renderPoints.pointLights;
    params.out.pointLightShadowMapAtlas = &m_shadowMaps.pointLightAtlas;

    params.in.spotLightRenderPoints = &renderContext.renderPoints.spotLights;
    params.out.spotLightShadowMapAtlas = &m_shadowMaps.spotLightAtlas;

    params.assetContext = &assetContext;
    params.in.directionalLightRenderPoint = renderContext.renderPoints.directionalLight.data();
    params.out.directionalLightShadowMap = &m_shadowMaps.directionalLight;
    
    m_effects.shadowMaps.run(&params);
}


void go::dfx_forward_plus_render_path::run_fxaa_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::fxaa_parameters params;

    params.in.colorTexture = &m_tempBuffers.backBufferMirror;
    params.out.colorTexture = go::the_gfx_device->back_buffer();

    m_effects.fxaa.run(&params);
}


void go::dfx_forward_plus_render_path::run_volumetric_lighting_effect(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::volumetric_lighting_parameters params;

    params.in.intensity = 1.0f;
    params.in.sampleCount = 48;
    params.in.viewDepthTexture = &m_gbuffer.depthVS;
    params.in.directionalLightShadowMap = &m_shadowMaps.directionalLight;
    params.in.bilateralBlurEffect = &m_effects.bilateralBlur;
    params.in.halfresUpsampleEffect = &m_effects.halfresUpsample;
    params.out.blurredOutputScratchTexture = &m_hdrBuffers.blurredBrightnessScratch; // NOTE: re-use unused HDR buffer
    params.out.volumetricLightingOutputTexture = &m_volumetricLightingBuffers.output;
    params.out.combinedOutputTexture = &m_gbuffer.color;

    m_effects.volumetricLighting.run(&params);
}


void go::dfx_forward_plus_render_path::run_min_z_downsample(
    const gfx_render_context &renderContext,
    gfx_asset_context &assetContext)
{
    dfx_effects::min_z_downsampling_parameters params;

    params.in.sourceMipLevel = 0;
    params.out.depthVS = &m_gbuffer.depthVS;
    m_effects.minZDownsampling.run(&params);
}