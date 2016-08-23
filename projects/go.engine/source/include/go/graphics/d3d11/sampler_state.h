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

/*! 
    \file graphics/d3d11/sampler_state.h
    \brief Contains a class which represents a GPU sampler state.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! Enumeration of possible sampler state types.
enum class gfx_sampler_state_type
{
    //! Use point sampling for minification, magnification, and mip sampling.
    min_mag_mip_point = D3D11_FILTER_MIN_MAG_MIP_POINT,
    //! Use point sampling for minification and magnification; use linear interpolation for mip sampling.
    min_mag_point_mip_linear = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
    //! Use linear sampling for minifcation, magnification and mip sampling.
    min_mag_mip_linear = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	//! Use anisotropic 4x filtering.
	anisotropic4x = D3D11_FILTER_ANISOTROPIC,
	//! Use comparison filter with min-mag-mip point filtering.
    compare_min_mag_mip_point = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT,
    //! Use comparison filter with min-mag linear and mip point filtering.
    compare_min_mag_linear_mip_point = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT
};


//! Enumeration of possible texture address mode. 
enum class gfx_texture_address_mode
{
    /*!
        Tile the texture at every (u,v) integer junction. For example, for u values between 0
        and 3, the texture is repeated three times.
     */
    wrap = D3D11_TEXTURE_ADDRESS_WRAP,
    /*!
        Flip the texture at every (u,v) integer junction. For u values between 0 and 1, for
        example, the texture is addressed normally; between 1 and 2, the texture is flipped
        (mirrored); between 2 and 3, the texture is normal again; and so on.
     */
    mirror = D3D11_TEXTURE_ADDRESS_MIRROR,
    /*!
        Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or
        1.0, respectively.
     */
    clamp = D3D11_TEXTURE_ADDRESS_CLAMP,
    //! Texture coordinates outside the range [0.0, 1.0] are set to the border color.
    border = D3D11_TEXTURE_ADDRESS_BORDER,
    /*!
        Similar to mirror and clamp. Takes the absolute value of the texture coordinate (thus,
        mirroring around 0), and then clamps to the maximum value.
     */
    mirror_once = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
};


//! A class, which represents a graphics sampler state. 
class gfx_sampler_state
{
public:
    //! \overload
    gfx_sampler_state() = default;
    /*!
        Constructor.

        \param type The type of the sampler state.
        \param addressU Method to use for resolving a u texture coordinate that is outside the 0 to 1 range.
        \param addressV Method to use for resolving a u texture coordinate that is outside the 0 to 1 range.
        \param addressW Method to use for resolving a u texture coordinate that is outside the 0 to 1 range.
        \param comparisonFunc The function to apply after sampling.
        \param borderColor The border color.
     */
    gfx_sampler_state
    (
        gfx_sampler_state_type type,
        gfx_texture_address_mode addressU = gfx_texture_address_mode::wrap,
        gfx_texture_address_mode addressV = gfx_texture_address_mode::wrap,
        gfx_texture_address_mode addressW = gfx_texture_address_mode::wrap,
        comparison_function comparisonFunc = comparison_function::never,
        float *borderColor = nullptr
    );
public:
    //! Releases all internal resources.
    void release();
    /*!
        Re-initializes the object.

        \param type The type of the sampler state.
        \param addressU Method to use for resolving a u texture coordinate that is outside the 0 to 1 range.
        \param addressV Method to use for resolving a u texture coordinate that is outside the 0 to 1 range.
        \param addressW Method to use for resolving a u texture coordinate that is outside the 0 to 1 range.
        \param comparisonFunc The function to apply after sampling.
        \param borderColor The border color.
     */
    void reset
    (
        gfx_sampler_state_type type,
        gfx_texture_address_mode addressU = gfx_texture_address_mode::wrap,
        gfx_texture_address_mode addressV = gfx_texture_address_mode::wrap,
        gfx_texture_address_mode addressW = gfx_texture_address_mode::wrap,
        comparison_function comparisonFunc = comparison_function::never,
        float *borderColor = nullptr
    );
    /*!
        A pointer to the internal sampler state object.

        \return A pointer to the internal D3D sampler state.
     */
    ID3D11SamplerState *d3d_sampler_state() const { return m_d3dState.Get(); }
private:
    //! The sampler state.
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_d3dState;
};

GO_END_NAMESPACE