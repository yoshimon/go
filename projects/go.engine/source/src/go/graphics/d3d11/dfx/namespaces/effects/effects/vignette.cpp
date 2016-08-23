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


#include "dfx/namespaces/effects/effects/vignette/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/effects/effects/vignette/effect_types.h"
#include "dfx/namespaces/effects/effects/vignette/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/effects/vignette.h"

#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

void update_gpu_constants(go::dfx_effects::vignette_parameters *parameters)
{
    // Upload parameters to GPU
    auto cb = dfx_hlsl::effects::vignette::g_constants;
    auto p = (dfx_hlsl::effects::vignette::constants *)cb->begin_update();
    {
        p->g_vignetteRadius = parameters->in.radius;
        p->g_vignetteStrength = parameters->in.strength;
    }
    cb->end_update();
}


// ================================================================================ //
// dfx::effects::vignette functions
// ================================================================================ //

void dfx::effects::vignette::on_initialize(void *userData)
{
    dfx_hlsl::effects::vignette::g_constants = new go::gfx_constant_buffer(sizeof(dfx_hlsl::effects::vignette::constants));
}


void dfx::effects::vignette::on_dispose()
{
    GO_SAFE_DELETE(dfx_hlsl::effects::vignette::g_constants);
}


void dfx::effects::vignette::on_resize()
{
}


void dfx::effects::vignette::on_begin_run(void *userData)
{
    // Reset the renderer
    go::the_gfx_renderer->bind_default_pipeline_state();

    // Update GPU constants
    auto parameters = (go::dfx_effects::vignette_parameters *)userData;
    update_gpu_constants(parameters);

    // Bind shaders
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &shaderCache = renderPath->renderer_helper().shader_cache();
    m_pass0VS = shaderCache.common.fsTriangleVS;
    m_pass0PS = shaderCache.vignetteEffect.vignettePS;
}


void dfx::effects::vignette::on_do_pass0(void *userData)
{
    auto parameters = (go::dfx_effects::vignette_parameters *)userData;

    GO_PERF_BEGIN_EVENT("Vignette");
    auto &blending = go::the_gfx_device->common_blend_states().multiply;
    go::the_gfx_renderer->bind_blend_state(&blending);

    go::the_gfx_renderer->bind_render_target(0, parameters->out.outputTexture, 0);
    go::the_gfx_renderer->match_viewport(parameters->out.outputTexture);
    go::the_gfx_renderer->draw_triangle();
    GO_PERF_END_EVENT;
}
