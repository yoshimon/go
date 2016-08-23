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


#include "dfx/namespaces/effects/effects/fxaa/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/effects/effects/fxaa/effect_types.h"
#include "dfx/namespaces/effects/effects/fxaa/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/effects/fxaa.h"

#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


// ================================================================================ //
// dfx::effects::fxaa functions
// ================================================================================ //

void dfx::effects::fxaa::on_initialize(void *userData)
{
}


void dfx::effects::fxaa::on_dispose()
{
}


void dfx::effects::fxaa::on_resize()
{
}


void dfx::effects::fxaa::on_begin_run(void *userData)
{
    // Reset the renderer
    go::the_gfx_renderer->bind_default_pipeline_state();

    // Update GPU constants
    auto parameters = (go::dfx_effects::fxaa_parameters *)userData;

    // Setup the next pass
    dfx_hlsl::effects::fxaa::g_colorTexture = parameters->in.colorTexture;

	// Bind shaders
	auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
	auto &shaderCache = renderPath->renderer_helper().shader_cache();
	m_pass0VS = shaderCache.common.fsTriangleVS;
	m_pass0PS = shaderCache.fxaaEffect.fxaaPS;
}


void dfx::effects::fxaa::on_do_pass0(void *userData)
{
    auto parameters = (go::dfx_effects::fxaa_parameters *)userData;

    GO_PERF_BEGIN_EVENT("FXAA");
    go::the_gfx_renderer->bind_render_target(0, parameters->out.colorTexture, 0);
    go::the_gfx_renderer->match_viewport(parameters->out.colorTexture);
    go::the_gfx_renderer->draw_triangle();
    GO_PERF_END_EVENT;
}
