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


#include "dfx/namespaces/effects/effects/halfres_upsample/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/effects/effects/halfres_upsample/effect_types.h"
#include "dfx/namespaces/effects/effects/halfres_upsample/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/effects/halfres_upsample.h"

#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


// ================================================================================ //
// dfx::effects::halfres_upsample functions
// ================================================================================ //

void dfx::effects::halfres_upsample::on_initialize(void *userData)
{
}


void dfx::effects::halfres_upsample::on_dispose()
{
}


void dfx::effects::halfres_upsample::on_resize()
{
}


void dfx::effects::halfres_upsample::on_begin_run(void *userData)
{
	// Reset the renderer
	go::the_gfx_renderer->bind_default_pipeline_state();

	// Update GPU constants
	auto parameters = (go::dfx_effects::halfres_upsample_parameters *)userData;

	// Setup next pass
	dfx_hlsl::effects::halfres_upsample::g_sourceTexture = parameters->in.sourceTexture;
	dfx_hlsl::effects::halfres_upsample::g_viewDepthTexture = parameters->in.viewDepthTexture;

	// Bind shaders
	auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
	auto &shaderCache = renderPath->renderer_helper().shader_cache();
	m_pass0VS = shaderCache.common.fsTriangleVS;
	m_pass0PS = shaderCache.halfresUpsampleEffect.halfresUpsamplePS;
}


void dfx::effects::halfres_upsample::on_do_pass0(void *userData)
{
	auto parameters = (go::dfx_effects::halfres_upsample_parameters *)userData;

	// Upsample
	auto &additiveBlending = go::the_gfx_device->common_blend_states().additive;
	go::the_gfx_renderer->bind_blend_state(&additiveBlending);

	go::the_gfx_renderer->bind_render_target(0, parameters->out.destTexture, 0);
	go::the_gfx_renderer->match_viewport(parameters->out.destTexture);
	go::the_gfx_renderer->draw_triangle();
}