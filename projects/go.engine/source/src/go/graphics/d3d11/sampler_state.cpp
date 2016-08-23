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


#include "go/graphics/d3d11/sampler_state.h"


#include "go/core/log.h"
#include "go/graphics/d3d11/display.h"


// ================================================================================ //
// go::gfx_sampler_state member functions
// ================================================================================ //

go::gfx_sampler_state::gfx_sampler_state(gfx_sampler_state_type type, gfx_texture_address_mode addressU,
    gfx_texture_address_mode addressV, gfx_texture_address_mode addressW, comparison_function comparisonFunc,
    float *borderColor)
{
    reset(type, addressU, addressV, addressW, comparisonFunc, borderColor);
}


void go::gfx_sampler_state::release()
{
    m_d3dState.Reset();
}


void go::gfx_sampler_state::reset(gfx_sampler_state_type type, gfx_texture_address_mode addressU,
    gfx_texture_address_mode addressV, gfx_texture_address_mode addressW, comparison_function comparisonFunc,
    float *borderColor)
{
    CD3D11_SAMPLER_DESC sd(CD3D11_DEFAULT{});
    sd.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)addressU;
    sd.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)addressV;
    sd.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)addressW;
    sd.Filter = (D3D11_FILTER)type;
    sd.MaxAnisotropy = 4;
    sd.ComparisonFunc = (D3D11_COMPARISON_FUNC)comparisonFunc;
    sd.MinLOD = 0.f;
    sd.MaxLOD = D3D11_FLOAT32_MAX;

    if(borderColor)
    {
    
        sd.BorderColor[0] = borderColor[0];
        sd.BorderColor[1] = borderColor[1];
        sd.BorderColor[2] = borderColor[2];
        sd.BorderColor[3] = borderColor[3];
    }

    GO_THROW_HRESULT
    (
        GO_INTERNAL g_d3d11Device->CreateSamplerState(&sd, &m_d3dState),
        "failed to create GPU sampler state"
    );
}