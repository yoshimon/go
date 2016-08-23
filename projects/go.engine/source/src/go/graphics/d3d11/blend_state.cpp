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


#include "go/graphics/d3d11/blend_state.h"


#include "go/core/log.h"
#include "go/graphics/d3d11/display.h"


// ================================================================================ //
// go::gfx_blend_state member functions
// ================================================================================ //

go::gfx_blend_state::gfx_blend_state(gfx_blend_mode type)
{
    reset(type);
}


void go::gfx_blend_state::release()
{
    m_d3dState.Reset();
}


void go::gfx_blend_state::reset(gfx_blend_mode type)
{
    release();

    CD3D11_BLEND_DESC bd(CD3D11_DEFAULT{});
	if(type == gfx_blend_mode::alpha)
    {
		bd.IndependentBlendEnable = TRUE;

		// Fragment color + albedo + metallic roughness
		for(int i = 0; i < 3; ++i)
		{
			auto &&rt = bd.RenderTarget[i];
			rt.BlendEnable = TRUE;
			rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			rt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

			rt.SrcBlendAlpha = D3D11_BLEND_ONE;
			rt.DestBlendAlpha = D3D11_BLEND_ZERO;

			rt.BlendOp = D3D11_BLEND_OP_ADD;
			rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;

			rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
    }
    else if(type == gfx_blend_mode::additive)
    {
        bd.IndependentBlendEnable = TRUE;
        bd.RenderTarget[0].BlendEnable = TRUE;
        bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        
        bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        
        bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        
        bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    else if(type == gfx_blend_mode::multiply)
    {
        bd.IndependentBlendEnable = TRUE;
        bd.RenderTarget[0].BlendEnable = TRUE;
        bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
        bd.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;

        bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

        bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

        bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    
    GO_THROW_HRESULT
    (
        GO_INTERNAL g_d3d11Device->CreateBlendState(&bd, &m_d3dState),
        "could not create blend state"
    );
}


ID3D11BlendState *go::gfx_blend_state::d3d_blend_state() const noexcept
{
    return m_d3dState.Get();
}