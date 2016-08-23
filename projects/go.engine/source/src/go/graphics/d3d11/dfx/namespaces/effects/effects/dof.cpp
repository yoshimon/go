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


#include "dfx/namespaces/effects/effects/dof/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/effects/effects/dof/effect_types.h"
#include "dfx/namespaces/effects/effects/dof/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/effects/dof.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

void update_gpu_constants(go::dfx_effects::dof_parameters *parameters)
{
    // Upload parameters to GPU
    auto cb = dfx_hlsl::effects::dof::g_constants;
    auto p = (dfx_hlsl::effects::dof::constants *)cb->begin_update();
    {
        // Figure out DoF regions
        auto nearEnd = std::max(0.f, parameters->in.nearEnd);
        auto farStart = nearEnd + std::max(0.f, parameters->in.nearRange);
        auto farEnd = farStart + std::max(0.f, parameters->in.farRange);
        auto farRange = farEnd - farStart;

        p->g_dofCoCToPx = go::clamp(8.f * parameters->in.strength, 0.f, 8.f); // Max CoC in pixels due to blur filter length
        p->g_dofNearEnd = nearEnd;
        p->g_dofFarStart = farStart;
        p->g_dofFarRange = farRange;
    }
    cb->end_update();
}


// ================================================================================ //
// dfx::effects::dof functions
// ================================================================================ //

void dfx::effects::dof::on_initialize(void *userData)
{
}


void dfx::effects::dof::on_dispose()
{
}


void dfx::effects::dof::on_resize()
{
}


void dfx::effects::dof::on_begin_run(void *userData)
{
    // Reset the renderer
    go::the_gfx_renderer->bind_default_pipeline_state();

    // Update GPU constants
    auto parameters = (go::dfx_effects::dof_parameters *)userData;
    update_gpu_constants(parameters);

    // Setup next pass
    dfx_hlsl::effects::dof::g_depthVSTexture = parameters->in.depthVSTexture;
}


void dfx::effects::dof::on_do_pass0(void *userData)
{
    auto parameters = (go::dfx_effects::dof_parameters *)userData;

    GO_PERF_BEGIN_EVENT("Circle of Confusion");
    go::the_gfx_renderer->bind_render_target(0, parameters->out.cocTexture, 0);
    go::the_gfx_renderer->match_viewport(parameters->out.cocTexture);
    go::the_gfx_renderer->draw_triangle();
    GO_PERF_END_EVENT;

    // Setup next pass
    dfx_hlsl::effects::dof::g_colorTexture = parameters->in.colorTexture;
    dfx_hlsl::effects::dof::g_destTextureRW = parameters->out.scratchBuffer;
}


void dfx::effects::dof::on_do_pass1(void *userData)
{
    GO_PERF_BEGIN_EVENT("BlurX");
    auto &nativeFmt = go::the_gfx_device->back_buffer()->format();
    auto numThreadGroupsX = go::ceil_div(nativeFmt.width, DFX_DOF_BLUR_X_THREADS_X);
    auto numThreadGroupsY = go::ceil_div(nativeFmt.height, DFX_DOF_BLUR_X_THREADS_Y);
    go::the_gfx_renderer->dispatch_compute(numThreadGroupsX, numThreadGroupsY, 1);
    GO_PERF_END_EVENT;

    auto dofParameters = (go::dfx_effects::dof_parameters *)userData;

    // Setup next pass
    dfx_hlsl::effects::dof::g_colorTexture = dofParameters->out.scratchBuffer;
    dfx_hlsl::effects::dof::g_destTextureRW = dofParameters->out.completeDoF;
}


void dfx::effects::dof::on_do_pass2(void *userData)
{
    GO_PERF_BEGIN_EVENT("BlurY");
    auto &nativeFmt = go::the_gfx_device->back_buffer()->format();
    auto numThreadGroupsX = go::ceil_div(nativeFmt.width, DFX_DOF_BLUR_Y_THREADS_X);
    auto numThreadGroupsY = go::ceil_div(nativeFmt.height, DFX_DOF_BLUR_Y_THREADS_Y);
    go::the_gfx_renderer->dispatch_compute(numThreadGroupsX, numThreadGroupsY, 1);
    GO_PERF_END_EVENT;
}
