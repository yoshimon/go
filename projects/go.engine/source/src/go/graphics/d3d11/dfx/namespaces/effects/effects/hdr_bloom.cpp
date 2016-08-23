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

#include "dfx/namespaces/effects/effects/hdr_bloom/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "go/core/performance_counter.h"

#include "dfx/namespaces/effects/effects/hdr_bloom/effect_types.h"
#include "dfx/namespaces/effects/effects/hdr_bloom/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/effects/hdr_bloom.h"

#include "dfx/namespaces/effects/effects/gaussian_blur/effect.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/gaussian_blur.h"

#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


// The remaining number of blur passes.
uint32_t s_numBlurPasses;


// ================================================================================ //
// Helper functions
// ================================================================================ //

void update_gpu_constants(go::dfx_effects::hdr_bloom_parameters *parameters)
{
    // Upload parameters to GPU
    auto cb = dfx_hlsl::effects::hdr_bloom::g_constants;
    auto p = (dfx_hlsl::effects::hdr_bloom::constants *)cb->begin_update();
    {
        p->g_hdrBloomThreshold = parameters->in.bloomThreshold;
        p->g_hdrBloomExposureAdaptionDelta = 0.5f * go::performance.elapsedTime;
        p->g_hdrBloomMaxLuminance = parameters->in.maxLuminance;
        p->g_hdrBloomMiddleGray = parameters->in.middleGray;
    }
    cb->end_update();
}


void update_combine_gpu_constants(float destTexelSizeX, float destTexelSizeY, uint32_t sourceMipLevel)
{
    // Upload parameters to GPU
    auto cb = dfx_hlsl::effects::hdr_bloom::g_combine_constants;
    auto p = (dfx_hlsl::effects::hdr_bloom::combine_constants *)cb->begin_update();
    {
        p->g_hdrBloomCombineSourceMipLevel = sourceMipLevel;
        go::copy_float2(p->g_hdrBloomCombineTexelSize, destTexelSizeX, destTexelSizeY);
    }
    cb->end_update();
}


// ================================================================================ //
// dfx::effects::hdr_bloom member functions
// ================================================================================ //

void dfx::effects::hdr_bloom::on_initialize(void *userData)
{
    dfx_hlsl::effects::hdr_bloom::g_constants = new go::gfx_constant_buffer(sizeof(dfx_hlsl::effects::hdr_bloom::constants));
    dfx_hlsl::effects::hdr_bloom::g_combine_constants = new go::gfx_constant_buffer(sizeof(dfx_hlsl::effects::hdr_bloom::combine_constants));
}


void dfx::effects::hdr_bloom::on_dispose()
{
    GO_SAFE_DELETE(dfx_hlsl::effects::hdr_bloom::g_constants);
    GO_SAFE_DELETE(dfx_hlsl::effects::hdr_bloom::g_combine_constants);
}


void dfx::effects::hdr_bloom::on_resize()
{
}


void dfx::effects::hdr_bloom::on_begin_run(void *userData)
{
    // Reset the renderer
    go::the_gfx_renderer->bind_default_pipeline_state();

    // Update GPU constants
    auto parameters = (go::dfx_effects::hdr_bloom_parameters *)userData;
    update_gpu_constants(parameters);

    // Setup next pass
    dfx_hlsl::effects::hdr_bloom::g_colorTexture = parameters->in.hdrColorTexture;
    dfx_hlsl::effects::hdr_bloom::g_luminanceTextureRW = parameters->out.luminanceTexture;
    dfx_hlsl::effects::hdr_bloom::g_lastLuminanceTextureRW = parameters->in.lastLuminanceTexture;
    dfx_hlsl::effects::hdr_bloom::g_brightness1Texture = parameters->out.brightnessTexture;
    dfx_hlsl::effects::hdr_bloom::g_brightness1TextureRW = dfx_hlsl::effects::hdr_bloom::g_brightness1Texture;
    dfx_hlsl::effects::hdr_bloom::g_brightness2Texture = parameters->out.brightnessTexture;
    dfx_hlsl::effects::hdr_bloom::g_brightness2TextureRW = dfx_hlsl::effects::hdr_bloom::g_brightness2Texture;
    dfx_hlsl::effects::hdr_bloom::g_brightness3Texture = parameters->out.brightnessTexture;
    dfx_hlsl::effects::hdr_bloom::g_brightness3TextureRW = dfx_hlsl::effects::hdr_bloom::g_brightness3Texture;
    dfx_hlsl::effects::hdr_bloom::g_brightness4Texture = parameters->out.brightnessTexture;
    dfx_hlsl::effects::hdr_bloom::g_brightness4TextureRW = dfx_hlsl::effects::hdr_bloom::g_brightness4Texture;
    dfx_hlsl::effects::hdr_bloom::g_bloomTextureRW = parameters->out.hdrBloomTexture;
    dfx_hlsl::effects::hdr_bloom::g_tempBloomTexture = parameters->out.blurredBrightnessTexture;

    // Bind shaders
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &shaderCache = renderPath->renderer_helper().shader_cache();
    m_pass0CS = shaderCache.hdrBloomEffect.luminanceBrightnessCS;
    m_pass1CS = shaderCache.hdrBloomEffect.averageLuminanceCS;
    m_pass3CS = shaderCache.hdrBloomEffect.combineCS;
}


void dfx::effects::hdr_bloom::on_do_pass0(void *userData)
{
    auto parameters = (go::dfx_effects::hdr_bloom_parameters *)userData;

    GO_PERF_BEGIN_EVENT("Luminance & Brightness");
    auto &fmt = parameters->in.hdrColorTexture->format();
    // NOTE: the pass runs at full-res but it is dispatched with quarter pixel count
    auto numThreadGroupsX = go::ceil_div(fmt.width / 4, DFX_HDR_BLOOM_LUM_AND_BRIGHT_THREADS);
    auto numThreadGroupsY = go::ceil_div(fmt.height / 4, DFX_HDR_BLOOM_LUM_AND_BRIGHT_THREADS);

    // TODO: the pipeline state tracking is buggy
    //go::the_gfx_renderer->bind_compute_shader_resource(2, dfx_hlsl::effects::hdr_bloom::g_colorTexture, 0);
    
    go::the_gfx_renderer->dispatch_compute(numThreadGroupsX, numThreadGroupsY, 1);
    GO_PERF_END_EVENT;
}


void dfx::effects::hdr_bloom::on_do_pass1(void *userData)
{
    GO_PERF_BEGIN_EVENT("Average Luminance");
    go::the_gfx_renderer->dispatch_compute(1, 1, 1);
    GO_PERF_END_EVENT;

    // Number of recursive blur passes that follow
    s_numBlurPasses = go::dfx_effects::hdr_bloom_parameters::kNumBlurPasses;
}


void dfx::effects::hdr_bloom::on_do_pass2(void *userData)
{
    GO_ASSERT(s_numBlurPasses > 0);

    auto parameters = (go::dfx_effects::hdr_bloom_parameters *)userData;

    // Number of remaining blur passes
    --s_numBlurPasses;

    GO_PERF_BEGIN_EVENT("Blur");
    // Run the gaussian blur
    go::dfx_effects::gaussian_blur_parameters gaussianBlurParams;
    gaussianBlurParams.in.sourceTexture = s_numBlurPasses == 3 ? parameters->out.brightnessTexture : parameters->out.hdrBloomTexture;
    gaussianBlurParams.in.sourceMipLevel = s_numBlurPasses;

    gaussianBlurParams.out.scratchTexture = parameters->out.blurredBrightnessScratchTexture;
    gaussianBlurParams.in.scratchMipLevel = 0;
    
    gaussianBlurParams.out.destTexture = parameters->out.blurredBrightnessTexture;
    gaussianBlurParams.in.destMipLevel = s_numBlurPasses;
    
    parameters->in.gaussianBlurEffect->run(&gaussianBlurParams);
    GO_PERF_END_EVENT;
}


void dfx::effects::hdr_bloom::on_do_pass3(void *userData)
{
    if(s_numBlurPasses > 0)
    {
        GO_PERF_BEGIN_EVENT("Combine (Add)");
        auto sourceMipLevel = s_numBlurPasses;
        auto destMipLevel = sourceMipLevel - 1;

		// Bind the right mip-level
		go::the_gfx_renderer->bind_compute_shader_uav
		(
			dfx_hlsl::effects::hdr_bloom::kRegister_bloomTextureRW,
			dfx_hlsl::effects::hdr_bloom::g_bloomTextureRW,
			destMipLevel
		);

        auto &bloomFmt = dfx_hlsl::effects::hdr_bloom::g_bloomTextureRW->format();

        auto destWidth = bloomFmt.width / (int32_t)(std::pow(2, destMipLevel));
        auto destHeight = bloomFmt.height / (int32_t)(std::pow(2, destMipLevel));

        auto destTexelSizeX = 1.0f / destWidth;
        auto destTexelSizeY = 1.0f / destHeight;

        update_combine_gpu_constants(destTexelSizeX, destTexelSizeY, sourceMipLevel);

        auto numThreadGroupsX = go::ceil_div(destWidth, DFX_HDR_BLOOM_COMBINE_ADD_THREADS_X);
        auto numThreadGroupsY = go::ceil_div(destHeight, DFX_HDR_BLOOM_COMBINE_ADD_THREADS_Y);
        go::the_gfx_renderer->dispatch_compute(numThreadGroupsX, numThreadGroupsY, 1);
        GO_PERF_END_EVENT;

        // Finish this run
        end_pass3();

        // And recurse
        begin_pass2();
        on_do_pass2(userData);
        end_pass2();

        begin_pass3();
        on_do_pass3(userData);
    }
}
