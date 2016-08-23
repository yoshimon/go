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


#include "dfx/namespaces/forward_plus/effects/ambient_lighting/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/forward_plus/effects/ambient_lighting/effect_types.h"
#include "dfx/namespaces/forward_plus/effects/ambient_lighting/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/forward_plus/effects/ambient_lighting.h"

#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

void update_gpu_constants(go::dfx_effects::forward_plus::ambient_lighting_parameters *parameters)
{
    // Upload parameters to GPU
    auto cb = dfx_hlsl::forward_plus::ambient_lighting::g_constants;
    auto p = (dfx_hlsl::forward_plus::ambient_lighting::constants *)cb->begin_update();
    {
        go::copy_float3(p->g_forwardPlusAmbientLightingColorAboveWS, parameters->in.ambientColorAboveWS);
        go::copy_float3(p->g_forwardPlusAmbientLightingColorBelowWS, parameters->in.ambientColorBelowWS);
    }
    cb->end_update();
}


// ================================================================================ //
// dfx::forward_plus::ambient_lighting functions
// ================================================================================ //

void dfx::forward_plus::ambient_lighting::on_initialize(void *userData)
{
    dfx_hlsl::forward_plus::ambient_lighting::g_constants = new go::gfx_constant_buffer(sizeof(dfx_hlsl::forward_plus::ambient_lighting::constants));
}


void dfx::forward_plus::ambient_lighting::on_dispose()
{
    GO_SAFE_DELETE(dfx_hlsl::forward_plus::ambient_lighting::g_constants);
}


void dfx::forward_plus::ambient_lighting::on_resize()
{
}


void dfx::forward_plus::ambient_lighting::on_begin_run(void *userData)
{
    // Reset the renderer
    go::the_gfx_renderer->bind_default_pipeline_state();

    // Update GPU constants
    auto parameters = (go::dfx_effects::forward_plus::ambient_lighting_parameters *)userData;
    update_gpu_constants(parameters);

    // Setup the next pass
    dfx_hlsl::forward_plus::ambient_lighting::g_aoTexture = parameters->in.aoTexture;
    dfx_hlsl::forward_plus::ambient_lighting::g_normalWSTexture = parameters->in.normalWSTexture;
	dfx_hlsl::forward_plus::ambient_lighting::g_metallicRoughnessTexture = parameters->in.metallicRoughnessTexture;
	dfx_hlsl::forward_plus::ambient_lighting::g_albedoTexture = parameters->in.albedoTexture;
	dfx_hlsl::forward_plus::ambient_lighting::g_depthVSTexture = parameters->in.depthVSTexture;
	dfx_hlsl::forward_plus::ambient_lighting::g_envBRDF = parameters->in.envBRDFTexture;
	dfx_hlsl::forward_plus::ambient_lighting::g_sceneEnvMap = parameters->in.sceneSpecularEnvMap;

    // Bind shaders
    auto renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &shaderCache = renderPath->renderer_helper().shader_cache();
    m_pass0VS = shaderCache.common.fsTriangleVS;
    m_pass0PS = shaderCache.ambientLightingPS;
}


void dfx::forward_plus::ambient_lighting::on_do_pass0(void *userData)
{
    auto parameters = (go::dfx_effects::forward_plus::ambient_lighting_parameters *)userData;

    GO_PERF_BEGIN_EVENT("Diffuse + Specular");
    // Additive blend state
    auto &additiveBlending = go::the_gfx_device->common_blend_states().additive;
    go::the_gfx_renderer->bind_blend_state(&additiveBlending);

    // RTs
    go::the_gfx_renderer->bind_render_target(0, parameters->out.colorTexture, 0);
    go::the_gfx_renderer->match_viewport(parameters->out.colorTexture);
    
    // Draw
    go::the_gfx_renderer->draw_triangle();
    GO_PERF_END_EVENT;
}
