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


#include "dfx/namespaces/effects/effects/min_z_downsampling/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/effects/effects/min_z_downsampling/effect_types.h"
#include "dfx/namespaces/effects/effects/min_z_downsampling/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/effects/min_z_downsampling.h"

#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"


// ================================================================================ //
// dfx::effects::min_z_downsampling functions
// ================================================================================ //

void dfx::effects::min_z_downsampling::on_initialize(void *userData)
{
    dfx_hlsl::effects::min_z_downsampling::g_constants = new go::gfx_constant_buffer(sizeof(dfx_hlsl::effects::min_z_downsampling::constants));
}


void dfx::effects::min_z_downsampling::on_dispose()
{
    GO_SAFE_DELETE(dfx_hlsl::effects::min_z_downsampling::g_constants);
}


void dfx::effects::min_z_downsampling::on_resize()
{
}


void dfx::effects::min_z_downsampling::on_begin_run(void *userData)
{
    // Reset the renderer
    go::the_gfx_renderer->bind_default_pipeline_state();

    // Update GPU constants
    auto &&parameters = (go::dfx_effects::min_z_downsampling_parameters *)userData;

    // We can't bind the resources yet
    dfx_hlsl::effects::min_z_downsampling::g_depthTexture = nullptr;
    dfx_hlsl::effects::min_z_downsampling::g_depthTextureRW = nullptr;

    // Bind shaders
    auto &&renderPath = (go::dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    auto &&shaderCache = renderPath->renderer_helper().shader_cache();
    m_pass0CS = shaderCache.minZDownsamplingEffect.minZDownsamplingCS;
}


void dfx::effects::min_z_downsampling::on_do_pass0(void *userData)
{
    auto &&parameters = (go::dfx_effects::min_z_downsampling_parameters *)userData;

    // Deduce the mip levels
    auto &&srcMipLevel = parameters->in.sourceMipLevel;
    auto &&destMipLevel = srcMipLevel + 1;

    // HACK: bypass the per-resource transition tracking and bind mip level 0 manually
    // This would require sub-resource tracking
    go::the_gfx_renderer->bind_compute_shader_uav(1, parameters->out.depthVS, destMipLevel);
    go::the_gfx_renderer->update_pipeline_state();
    ID3D11ShaderResourceView *srvs[] = { parameters->out.depthVS->d3d_srv_layer() };
    GO_INTERNAL g_d3d11ImmediateContext->CSSetShaderResources(0, 1, srvs);

    // Update the shader constants
    dfx_hlsl::effects::min_z_downsampling::g_constants->update(&srcMipLevel, sizeof(srcMipLevel));

    // Dispatch at half size
    auto &&depthTex = parameters->out.depthVS;
    auto &&fmt = depthTex->format();

    auto &&dispatchX = go::ceil_div(go::ceil_div(fmt.width, 2), 8);
    auto &&dispatchY = go::ceil_div(go::ceil_div(fmt.height, 2), 8);

    // Manually dispatch: this avoids unbinding the UAV that was bound first
    GO_INTERNAL g_d3d11ImmediateContext->Dispatch(dispatchX, dispatchY, 1);

    // Manually unbind
    srvs[0] = nullptr;
    GO_INTERNAL g_d3d11ImmediateContext->CSSetShaderResources(0, 1, srvs);
    go::the_gfx_renderer->bind_compute_shader_resource(0, nullptr, 0);
}
