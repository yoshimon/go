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


#include "go/graphics/d3d11/depth_stencil_state.h"


#include "go/core/log.h"
#include "go/graphics/d3d11/display.h"


// ================================================================================ //
// go::gfx_depth_stencil_state member functions
// ================================================================================ //

go::gfx_depth_stencil_state::gfx_depth_stencil_state(const gfx_depth_stencil_state_parameters &options)
{
    reset(options);
}


void go::gfx_depth_stencil_state::release()
{
    m_d3dState.Reset();
}


void go::gfx_depth_stencil_state::reset(const gfx_depth_stencil_state_parameters &options)
{
    CD3D11_DEPTH_STENCIL_DESC dsd(CD3D11_DEFAULT{});
    dsd.DepthEnable = options.depth.enableTest;
    dsd.DepthWriteMask = options.depth.enableWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    dsd.DepthFunc = (D3D11_COMPARISON_FUNC)options.depth.comparisonFunction;

    dsd.StencilEnable = options.stencil.enableTest;
    dsd.StencilReadMask = options.stencil.readMask;
    dsd.StencilWriteMask = options.stencil.writeMask;

    GO_THROW_HRESULT
    (
        GO_INTERNAL g_d3d11Device->CreateDepthStencilState(&dsd, &m_d3dState),
        "failed to create GPU depth-stencil state"
    );
}


ID3D11DepthStencilState *go::gfx_depth_stencil_state::d3d_depth_stencil_state() const
{
    return m_d3dState.Get();
}