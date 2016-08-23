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


#include "go/graphics/d3d11/rasterizer_state.h"


#include "go/core/log.h"
#include "go/graphics/d3d11/device.h"


// ================================================================================ //
// go::gfx_rasterizer_state member functions
// ================================================================================ //

go::gfx_rasterizer_state::gfx_rasterizer_state(const gfx_rasterizer_parameters &options)
{
    reset(options);
}


void go::gfx_rasterizer_state::release()
{
    m_d3dRastizerState.Reset();
}


void go::gfx_rasterizer_state::reset(const gfx_rasterizer_parameters &options)
{
    release();

    // RS
	CD3D11_RASTERIZER_DESC desc(CD3D11_DEFAULT{});
    desc.DepthClipEnable = options.depthClipEnable;
    desc.CullMode = (D3D11_CULL_MODE)options.cullMode;
    desc.DepthBias = options.depthBias;
    desc.DepthBiasClamp = options.depthBiasClamp;
    desc.SlopeScaledDepthBias = options.slopeScaledDepthBias;
    desc.FrontCounterClockwise = options.frontCounterClockwise;

    GO_THROW_HRESULT
    (
        GO_INTERNAL g_d3d11Device->CreateRasterizerState(&desc, &m_d3dRastizerState),
        "Could not create rasterizer state."
    );
}


ID3D11RasterizerState *go::gfx_rasterizer_state::d3d_rasterizer_state() const noexcept
{
    return m_d3dRastizerState.Get();
}