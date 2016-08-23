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


#include "dfx/namespaces/effects/effects/ssao/effect.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/effects/effects/ssao/effect_types.h"
#include "dfx/namespaces/effects/effects/ssao/effect_declarations.h"

#include "go/graphics/d3d11/dfx/namespaces/effects/ssao.h"

#include "gfsdk_ssao\lib\GFSDK_SSAO.h"


// The SSAO context
static GFSDK_SSAO_Context_D3D11 *s_AOContext = nullptr;


// ================================================================================ //
// dfx::effects::ssao functions
// ================================================================================ //

static void compute_ssao(go::dfx_effects::ssao_parameters *parameters)
{
    using namespace DirectX;

    GFSDK_SSAO_InputData_D3D11 inputParams;
    memset(&inputParams, 0, sizeof(inputParams));

    auto &&fmt = parameters->in.depthVSTexture->format();
    inputParams.DepthData.DepthTextureType = GFSDK_SSAO_VIEW_DEPTHS;
    inputParams.DepthData.pFullResDepthTextureSRV = parameters->in.depthVSTexture->d3d_srv();
    inputParams.DepthData.Viewport.Enable = true;
    inputParams.DepthData.Viewport.Width = (GFSDK_SSAO_UINT)fmt.width;
    inputParams.DepthData.Viewport.Height = (GFSDK_SSAO_UINT)fmt.height;
    inputParams.DepthData.Viewport.TopLeftX = 0;
    inputParams.DepthData.Viewport.TopLeftY = 0;
    inputParams.DepthData.Viewport.MinDepth = 0;
    inputParams.DepthData.Viewport.MaxDepth = 1.f;
    inputParams.DepthData.ProjectionMatrix.Data = &parameters->in.depthProjMatrix._11;
    inputParams.DepthData.ProjectionMatrix.Layout = GFSDK_SSAO_ROW_MAJOR_ORDER;
    inputParams.DepthData.MetersToViewSpaceUnits = parameters->in.metersToViewSpaceUnits;

    inputParams.NormalData.Enable = parameters->in.normalWSTexture != nullptr;
    inputParams.NormalData.DecodeBias = 0.f;
    inputParams.NormalData.DecodeScale = 1.f;
    inputParams.NormalData.pFullResNormalTextureSRV = parameters->in.normalWSTexture->d3d_srv();
    inputParams.NormalData.WorldToViewMatrix.Data = &parameters->in.depthViewMatrix._11;
    inputParams.NormalData.WorldToViewMatrix.Layout = GFSDK_SSAO_ROW_MAJOR_ORDER;

    GFSDK_SSAO_Parameters_D3D11 outputParams;
    outputParams.Radius = parameters->in.radius;
    outputParams.Bias = parameters->in.angularBias;
    outputParams.PowerExponent = parameters->in.strength;
    outputParams.Blur.Enable = true;
    outputParams.Blur.Radius = GFSDK_SSAO_BLUR_RADIUS_4;
    outputParams.Blur.Sharpness = 4.f;
    outputParams.CoarseAO = 1.0f;
    outputParams.DetailAO = 1.0f;
    outputParams.DepthClampMode = GFSDK_SSAO_DepthClampMode::GFSDK_SSAO_CLAMP_TO_EDGE;
    outputParams.DepthStorage = GFSDK_SSAO_DepthStorage::GFSDK_SSAO_FP32_VIEW_DEPTHS;
    outputParams.DepthThreshold.Enable = false;
    outputParams.Output.BlendMode = GFSDK_SSAO_BlendMode::GFSDK_SSAO_OVERWRITE_RGB;

    auto &&outputTexture = parameters->out.outputTexture;

    // Unbind RT from input
    go::the_gfx_renderer->bind_default_pipeline_state();
    go::the_gfx_renderer->bind_render_target(0, outputTexture, 0);
    go::the_gfx_renderer->update_pipeline_state(); // Flush

    s_AOContext->RenderAO(GO_INTERNAL g_d3d11ImmediateContext, &inputParams, &outputParams, outputTexture->d3d_rtv());
}


// ================================================================================ //
// dfx::effects::ssao functions
// ================================================================================ //

void dfx::effects::ssao::on_initialize(void *userData)
{
    GFSDK_SSAO_CustomHeap customHeap;
    customHeap.delete_ = ::operator delete;
    customHeap.new_ = ::operator new;

    GFSDK_SSAO_CreateContext_D3D11(GO_INTERNAL g_d3d11Device, &s_AOContext, &customHeap);
}


void dfx::effects::ssao::on_dispose()
{
    GO_SAFE_RELEASE(s_AOContext);
}


void dfx::effects::ssao::on_resize()
{
}


void dfx::effects::ssao::on_begin_run(void *userData)
{
}


void dfx::effects::ssao::on_do_pass0(void *userData)
{
    auto parameters = (go::dfx_effects::ssao_parameters *)userData;

    GO_PERF_BEGIN_EVENT("SSAO");
    compute_ssao(parameters);
    GO_PERF_END_EVENT;
}
