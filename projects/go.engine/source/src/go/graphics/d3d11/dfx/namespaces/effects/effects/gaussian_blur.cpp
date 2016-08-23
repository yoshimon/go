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


#include "dfx/namespaces/effects/effects/gaussian_blur/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/effects/effects/gaussian_blur/effect_types.h"
#include "dfx/namespaces/effects/effects/gaussian_blur/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/effects/gaussian_blur.h"

#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

void update_gpu_constants(uint32_t sourceMipLevel)
{
    // Upload parameters to GPU
    auto cb = dfx_hlsl::effects::gaussian_blur::g_constants;
    auto p = (dfx_hlsl::effects::gaussian_blur::constants *)cb->begin_update();
    {
        p->g_gaussianBlurMipLevel = sourceMipLevel;
    }
    cb->end_update();
}


// ================================================================================ //
// dfx::effects::gaussian_blur functions
// ================================================================================ //

void dfx::effects::gaussian_blur::on_initialize(void *userData)
{
    dfx_hlsl::effects::gaussian_blur::g_constants = new go::gfx_constant_buffer(sizeof(dfx_hlsl::effects::gaussian_blur::constants));
}


void dfx::effects::gaussian_blur::on_dispose()
{
    GO_SAFE_DELETE(dfx_hlsl::effects::gaussian_blur::g_constants);
}


void dfx::effects::gaussian_blur::on_resize()
{
}


void dfx::effects::gaussian_blur::on_begin_run(void *userData)
{
    // Reset the renderer
    go::the_gfx_renderer->bind_default_pipeline_state();

    // Update GPU constants
    auto parameters = (go::dfx_effects::gaussian_blur_parameters *)userData;

    // Setup next pass
    dfx_hlsl::effects::gaussian_blur::g_sourceTexture = parameters->in.sourceTexture;
    dfx_hlsl::effects::gaussian_blur::g_destTextureRW = parameters->out.scratchTexture;

    // Bind shaders
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &shaderCache = renderPath->renderer_helper().shader_cache();
    m_pass0CS = shaderCache.gaussianBlurEffect.blurXCS;
    m_pass1CS = shaderCache.gaussianBlurEffect.blurYCS;
}


void dfx::effects::gaussian_blur::on_do_pass0(void *userData)
{
    auto parameters = (go::dfx_effects::gaussian_blur_parameters *)userData;

    // Rebind the proper target mip-level
    go::the_gfx_renderer->bind_compute_shader_uav
    (
        dfx_hlsl::effects::gaussian_blur::kRegister_destTextureRW,
        dfx_hlsl::effects::gaussian_blur::g_destTextureRW,
        parameters->in.scratchMipLevel
    );

    update_gpu_constants(parameters->in.sourceMipLevel);

    GO_PERF_BEGIN_EVENT("BlurX");
    auto &sourceFmt = parameters->in.sourceTexture->format();
    auto numThreadGroupsX = go::ceil_div(sourceFmt.width, DFX_GAUSSIAN_BLUR_X_THREADS_X);
    auto numThreadGroupsY = go::ceil_div(sourceFmt.height, DFX_GAUSSIAN_BLUR_X_THREADS_Y);
    go::the_gfx_renderer->dispatch_compute(numThreadGroupsX, numThreadGroupsY, 1);
    GO_PERF_END_EVENT;

    // Setup next pass
    dfx_hlsl::effects::gaussian_blur::g_sourceTexture = parameters->out.scratchTexture;
    dfx_hlsl::effects::gaussian_blur::g_destTextureRW = parameters->out.destTexture;
}


void dfx::effects::gaussian_blur::on_do_pass1(void *userData)
{
    auto parameters = (go::dfx_effects::gaussian_blur_parameters *)userData;

    // Rebind the proper target mip-level
    go::the_gfx_renderer->bind_compute_shader_uav
    (
        dfx_hlsl::effects::gaussian_blur::kRegister_destTextureRW,
        dfx_hlsl::effects::gaussian_blur::g_destTextureRW,
        parameters->in.destMipLevel
    );

    update_gpu_constants(parameters->in.scratchMipLevel);

    GO_PERF_BEGIN_EVENT("BlurY");
    auto &sourceFmt = parameters->in.sourceTexture->format();
    auto numThreadGroupsX = go::ceil_div(sourceFmt.width, DFX_GAUSSIAN_BLUR_Y_THREADS_X);
    auto numThreadGroupsY = go::ceil_div(sourceFmt.height, DFX_GAUSSIAN_BLUR_Y_THREADS_Y);
    go::the_gfx_renderer->dispatch_compute(numThreadGroupsX, numThreadGroupsY, 1);
    GO_PERF_END_EVENT;
}
