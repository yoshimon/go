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

/*! 
    \file graphics/d3d11/render_paths/dfx_forward_plus.h
    \brief Contains an implementation of a Forward+ render path with DFX.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/graphics/render_path.h"
#include "go/graphics/texture2d_atlas.h"
#include "go/graphics/d3d11/dfx/forward_plus_util.h"
#include "go/graphics/d3d11/timer.h"

#include "dfx/namespaces/effects/effects/bilateral_blur/effect.h"
#include "dfx/namespaces/effects/effects/dof/effect.h"
#include "dfx/namespaces/effects/effects/film_grain/effect.h"
#include "dfx/namespaces/effects/effects/fxaa/effect.h"
#include "dfx/namespaces/effects/effects/gaussian_blur/effect.h"
#include "dfx/namespaces/effects/effects/halfres_upsample/effect.h"
#include "dfx/namespaces/effects/effects/hdr_bloom/effect.h"
#include "dfx/namespaces/effects/effects/min_z_downsampling/effect.h"
#include "dfx/namespaces/effects/effects/overlay/effect.h"
#include "dfx/namespaces/effects/effects/shadow_maps/effect.h"
#include "dfx/namespaces/effects/effects/ssao/effect.h"
#include "dfx/namespaces/effects/effects/tonemapping/effect.h"
#include "dfx/namespaces/effects/effects/vignette/effect.h"
#include "dfx/namespaces/effects/effects/volumetric_lighting/effect.h"

#include "dfx/namespaces/forward_plus/effects/ambient_lighting/effect.h"
#include "dfx/namespaces/forward_plus/effects/prepass/effect.h"
#include "dfx/namespaces/forward_plus/effects/shading/effect.h"


GO_BEGIN_NAMESPACE

class gfx_renderer;

//! A class, which implements a Forward+ render path.
class GO_API dfx_forward_plus_render_path : public gfx_render_path
{
public:
    //! A structure, which holds all effects in the render-path.
    struct effects_t
    {
        //! The bilateral blur effect.
        dfx::effects::bilateral_blur bilateralBlur;
        //! The DoF effect.
        dfx::effects::dof dof;
        //! The film-grain effect.
        dfx::effects::film_grain filmGrain;
        //! The FXAA
        dfx::effects::fxaa fxaa;
        //! The Gaussian-blur effect.
        dfx::effects::gaussian_blur gaussianBlur;
        //! The half-resolution upsampling effect.
        dfx::effects::halfres_upsample halfresUpsample;
        //! The HDR-Bloom effect.
        dfx::effects::hdr_bloom hdrBloom;
        //! The MinZ downsampling effect.
        dfx::effects::min_z_downsampling minZDownsampling;
        //! The overlay effect.
        dfx::effects::overlay overlay;
        //! The shadow-maps effect.
        dfx::effects::shadow_maps shadowMaps;
        //! The SSAO effect.
        dfx::effects::ssao ssao;
        //! The tonemapping effect.
        dfx::effects::tonemapping tonemapping;
        //! The screen-space vignette effect.
        dfx::effects::vignette vignette;
        //! The volumetric-lighting effect.
        dfx::effects::volumetric_lighting volumetricLighting;
        //! A structure, holding the forward-plus specific effects.
        struct forward_plus_t
        {
            //! The pre-pass effect.
            dfx::forward_plus::prepass prepass;
            //! The shading effect.
            dfx::forward_plus::shading shading;
            //! The ambient lighting effect.
            dfx::forward_plus::ambient_lighting ambientLighting;
        } forwardPlus;
    };
    //! A structure, that contains the buffers used used by the path.
    struct buffers_t
    {
        struct
        {
            //! A buffer that holds the disk light proxy shapes.
            gfx_structured_buffer shapes;
            //! A buffer that holds the disk light proxy shapes.
            gfx_structured_buffer parameters;
            //! A buffer that holds the disk light tiles indices.
            gfx_structured_buffer tileIndices;
        } diskLights;
        struct
        {
            //! A buffer that holds the disk light proxy shapes.
            gfx_structured_buffer shapes;
            //! A buffer that holds the disk light proxy shapes.
            gfx_structured_buffer parameters;
            //! A buffer that holds the spherical light tile indices.
            gfx_structured_buffer tileIndices;
        } sphericalLights;
    };
    //! A structure, that contains the textures used used by the path.
    struct gbuffer_t
    {
        //! The albedo buffer.
        gfx_render_target albedo;
        //! The fragment color.
        gfx_render_target color;
        //! The normal in world-space.
        gfx_render_target normalWS;
        //! The depth in view-space.
        gfx_render_target depthVS;
        //! The metallic-roughness buffer.
        gfx_render_target metallicRoughness;
        //! The depth buffer.
        gfx_depth_stencil_buffer *depthBuffer;
        //! The ambient occlusion buffer.
        gfx_render_target ambientOcclusion;
    };
    //! A structure, containing the HDR buffers.
    struct hdr_buffers_t
    {
        //! The HDR bloom render-target.
        gfx_texture bloom;
        //! The brightness buffer.
        gfx_texture brightness;
        //! The last frames luminance texture.
        gfx_texture lastLuminance;
        //! The frames luminance texture.
        gfx_texture luminance;
        //! A scratch buffer.
        gfx_texture blurredBrightnessScratch;
        //! A scratch buffer.
        gfx_texture blurredBrightness;
    };
    //! A structure, containing temporary buffers.
    struct temp_buffers_t
    {
        //! Same size and format of the back buffer.
        gfx_render_target backBufferMirror;
    };
    //! A structure, containing the shadow maps.
    struct shadow_maps_t
    {
        //! The shadow map for the directional light.
        gfx_depth_stencil_buffer directionalLight;
        //! Cache of cube-shadow maps.
        gfx_depth_stencil_buffer_regular_atlas pointLightAtlas;
        //! Cache of spot-light shadow maps.
        gfx_depth_stencil_buffer_regular_atlas spotLightAtlas;
    };
    //! A structure, containing the buffers for the volumetric lighting effect.
    struct volumetric_lighting_buffers_t
    {
        //! The volumetric lighting buffer.
        gfx_render_target output;
    };
public:
    /*!
        Constructor.

        \param renderer The renderer. This has to be supplied since the render-path can be constructed
        while the renderer might still be under construction so there is no global renderer pointer available.
     */
    dfx_forward_plus_render_path(gfx_renderer *renderer);
    //! Destructor.
    ~dfx_forward_plus_render_path();
public:
    void render(gfx_render_context &, gfx_asset_context &) override;
    void on_resize() override;
public:
    /*!
        Returns the renderer-helper for the Forward+ pipeline.

        \return The renderer-helper.
     */
    const dfx_util::forward_plus_renderer &renderer_helper() const noexcept { return m_helperRenderer; }
    /*!
        Returns the effects of the render-path.

        \return The effects in the render-path.
     */
    effects_t &effects();
    /*!
        Returns the number of thread groups for the light culling pass.

        \return The number of thread-groups for the light culling pass.
     */
    DirectX::XMUINT2 light_culling_thread_groups() const;
private:
    //! Resets all render-targets.
    void reset_render_targets();
    //! Resets all buffers.
    void reset_buffers();
    //! Resizes all viewport-dependent buffers.
    void resize_buffers();
    /*!
        Updates per-frame constant data.

        \param context The render-context.
     */
    void update_per_frame_constants(const gfx_render_context &context);
    /*!
        Updates light buffers.

        \param context The render-context.
     */
    void update_lights(const gfx_render_context &context);
    /*!
        Allocates the shadow maps for a given lighting setup.
     
        \param context The render-context.
     */
    void allocate_shadow_maps(gfx_render_context &context);
    /*!
        Temporally uploads the material database to the GPU.

        Only invalidates the buffers if the material database changes.
     */
    void temporal_upload_material_db(go::gfx_material_database *materialDB);
    /*!
        Runs the pre-pass effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_prepass_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the shading effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_shading_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the SSAO effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_ssao_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the ambient lighting effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_ambient_lighting_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the HDR bloom effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_hdr_bloom_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the tonemapping effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_tonemapping_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the film grain effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_film_grain_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the vignette effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_vignette_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the shadow maps effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_shadow_maps_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the FXAA effect.

        \param renderContext The render context.
        \param assetContext The asset context.
    */
    void run_fxaa_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the volumetric lighting effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_volumetric_lighting_effect(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
    /*!
        Runs the MinZ depth downsample effect effect.

        \param renderContext The render context.
        \param assetContext The asset context.
     */
    void run_min_z_downsample(const gfx_render_context &renderContext, gfx_asset_context &assetContext);
private:
    //! The effects in this path.
    effects_t m_effects;
    //! The buffers used by the effect.
    buffers_t m_buffers;
    //! The G-Buffer.
    gbuffer_t m_gbuffer;
    //! HDR buffers.
    hdr_buffers_t m_hdrBuffers;
    //! Temporary buffers for ping-pong rendering between two passes.
    temp_buffers_t m_tempBuffers;
    //! The shadow maps.
    shadow_maps_t m_shadowMaps;
    //! The buffers to use for the volumetric lighting effects.
    volumetric_lighting_buffers_t m_volumetricLightingBuffers;
    //! Helper to render with Forward+.
    dfx_util::forward_plus_renderer m_helperRenderer;
    //! The material database that was used previousl.
    go::gfx_material_database *m_currentMaterialDB;
    //! The double-buffered effect timers.
    go::double_buffered<gfx_timers> m_gpuTimers;
    //! The timer frequency for the GPU clock.
    go::double_buffered<gfx_timer_frequency> m_gpuTimerFrequency;
};

GO_END_NAMESPACE