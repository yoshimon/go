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
    \file graphics/d3d11/rasterizer_state.h
    \brief Contains a class which represents a GPU rasterizer state.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! Enumeration of possible cull modes.
enum class gfx_rasterizer_cull_mode
{
    //! No culling.
    none = D3D11_CULL_NONE,
    //! Front-face culling-
    front = D3D11_CULL_FRONT,
    //! Back-face culling.
    back = D3D11_CULL_BACK
};


//! A structure, which describes the rasterizer state.
struct gfx_rasterizer_parameters
{
    //! The cull mode to use.
    gfx_rasterizer_cull_mode cullMode;
    //! Determines if a triangle is front-facing or back-facing.
    bool frontCounterClockwise;
    //! Specifies whether depth-clipping should be enabled.
    bool depthClipEnable;
    //! Specifies the depth value added to a given pixel.
    int depthBias;
    //! Specifies the maximum depth bias of a pixel.
    float depthBiasClamp;
    //! Specifies a scalar on a given pixel's slope.
    float slopeScaledDepthBias;
};


//! A class, which represents a rasterizer state.
class GO_API gfx_rasterizer_state
{
    GO_DECLARE_NON_COPYABLE(gfx_rasterizer_state)
public:
    //! \overload
    gfx_rasterizer_state() = default;
    /*!
        Constructor.

        \param options The rasterizer parameters.
     */
    gfx_rasterizer_state(const gfx_rasterizer_parameters &options);
public:
    //! Releases all internal resources.
    void release();
    /*!
        Re-initializes the object.

        \param descriptor The state descriptor.
     */
    void reset(const gfx_rasterizer_parameters &descriptor);
    /*!
        A pointer to the internal state object.

        \return A pointer to the internal D3D state.
     */
    ID3D11RasterizerState *d3d_rasterizer_state() const noexcept;
private:
    //! The rasterizer state.
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_d3dRastizerState;
};

GO_END_NAMESPACE