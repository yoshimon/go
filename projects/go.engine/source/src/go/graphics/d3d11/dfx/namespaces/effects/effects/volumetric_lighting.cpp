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


#include "dfx/namespaces/effects/effects/volumetric_lighting/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/effects/effects/volumetric_lighting/effect_types.h"
#include "dfx/namespaces/effects/effects/volumetric_lighting/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/effects/volumetric_lighting.h"

#include "dfx/namespaces/effects/effects/bilateral_blur/effect.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/bilateral_blur.h"

#include "dfx/namespaces/effects/effects/halfres_upsample/effect.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/halfres_upsample.h"

#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

void update_gpu_constants(go::dfx_effects::volumetric_lighting_parameters *parameters)
{
	// Upload parameters to GPU
	auto cb = dfx_hlsl::effects::volumetric_lighting::g_constants;
	auto p = (dfx_hlsl::effects::volumetric_lighting::constants *)cb->begin_update();
	{
		p->g_volumetricLightingIntensity = parameters->in.intensity;
		p->g_volumetricLightingSampleCount = (float)parameters->in.sampleCount;
	}
	cb->end_update();
}


// ================================================================================ //
// dfx::effects::volumetric_lighting functions
// ================================================================================ //

void dfx::effects::volumetric_lighting::on_initialize(void *userData)
{
	dfx_hlsl::effects::volumetric_lighting::g_constants = new go::gfx_constant_buffer(sizeof(dfx_hlsl::effects::volumetric_lighting::constants));
}


void dfx::effects::volumetric_lighting::on_dispose()
{
	GO_SAFE_DELETE(dfx_hlsl::effects::volumetric_lighting::g_constants);
}


void dfx::effects::volumetric_lighting::on_resize()
{
}


void dfx::effects::volumetric_lighting::on_begin_run(void *userData)
{
	// Reset the renderer
	go::the_gfx_renderer->bind_default_pipeline_state();

	// Update GPU constants
	auto parameters = (go::dfx_effects::volumetric_lighting_parameters *)userData;
	update_gpu_constants(parameters);

	// Setup next pass
	dfx_hlsl::effects::volumetric_lighting::g_directionalSM = parameters->in.directionalLightShadowMap;
	dfx_hlsl::effects::volumetric_lighting::g_viewDepthTexture = parameters->in.viewDepthTexture;
	dfx_hlsl::effects::volumetric_lighting::g_volumetricLightingRW = parameters->out.volumetricLightingOutputTexture;
    dfx_hlsl::effects::volumetric_lighting::g_volumetricLighting = dfx_hlsl::effects::volumetric_lighting::g_volumetricLightingRW;

	// Bind shaders
	auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
	auto &shaderCache = renderPath->renderer_helper().shader_cache();
    m_pass0CS = shaderCache.volumetricLightingEffect.volumetricLightingCS;
    m_pass2VS = shaderCache.common.fsTriangleVS;
    m_pass2PS = shaderCache.volumetricLightingEffect.applyTransmittancePS;
}


void dfx::effects::volumetric_lighting::on_do_pass0(void *userData)
{
	auto parameters = (go::dfx_effects::volumetric_lighting_parameters *)userData;

	GO_PERF_BEGIN_EVENT("Volumetric Lighting");
	auto &fmt = parameters->out.volumetricLightingOutputTexture->format();
	auto numThreadGroupsX = go::ceil_div(fmt.width, DFX_VOLUMETRIC_LIGHTING_THREADS_X);
	auto numThreadGroupsY = go::ceil_div(fmt.height, DFX_VOLUMETRIC_LIGHTING_THREADS_Y);
	go::the_gfx_renderer->dispatch_compute(numThreadGroupsX, numThreadGroupsY, 1);
	GO_PERF_END_EVENT;
}


void dfx::effects::volumetric_lighting::on_do_pass1(void *userData)
{
	auto parameters = (go::dfx_effects::volumetric_lighting_parameters *)userData;
    go::dfx_effects::bilateral_blur_parameters blurParams;

    for(int32_t i = 0; i < 2; ++i)
    {
        GO_PERF_BEGIN_EVENT("Bilateral Blur");
        blurParams.in.sourceTexture = parameters->out.volumetricLightingOutputTexture;
        blurParams.in.sourceTextureMipLevel = 0;
        blurParams.in.depthVSTexture = parameters->in.viewDepthTexture;
        blurParams.in.depthVSTextureMipLevel = 1; // NOTE: half-size output
        blurParams.out.scratchTexture = parameters->out.blurredOutputScratchTexture;
        blurParams.out.destTexture = parameters->out.volumetricLightingOutputTexture;
        parameters->in.bilateralBlurEffect->run(&blurParams);
        GO_PERF_END_EVENT;
    }
}


void dfx::effects::volumetric_lighting::on_do_pass2(void *userData)
{
    auto parameters = (go::dfx_effects::volumetric_lighting_parameters *)userData;

    GO_PERF_BEGIN_EVENT("Radiance Transmittance");
    auto &&blendStates = go::the_gfx_device->common_blend_states();
    go::the_gfx_renderer->bind_blend_state(&blendStates.multiply);
    go::the_gfx_renderer->bind_render_target(0, parameters->out.combinedOutputTexture, 0);
    go::the_gfx_renderer->match_viewport(parameters->out.combinedOutputTexture);
    go::the_gfx_renderer->draw_triangle();
    GO_PERF_END_EVENT;
}



void dfx::effects::volumetric_lighting::on_do_pass3(void *userData)
{
	auto parameters = (go::dfx_effects::volumetric_lighting_parameters *)userData;

	GO_PERF_BEGIN_EVENT("Scattered Lighting");
	go::dfx_effects::halfres_upsample_parameters upsampleParams;
	upsampleParams.in.sourceTexture = parameters->out.volumetricLightingOutputTexture;
	upsampleParams.in.viewDepthTexture = parameters->in.viewDepthTexture;
	upsampleParams.out.destTexture = parameters->out.combinedOutputTexture;
	parameters->in.halfresUpsampleEffect->run(&upsampleParams);
	GO_PERF_END_EVENT;
}
