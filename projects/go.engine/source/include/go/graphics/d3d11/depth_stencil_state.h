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
    \file graphics/d3d11/depth_stencil_state.h
    \brief Contains a class that represents a depth-stencil state.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! A structure, which describes the depth state.
struct gfx_depth_state_parameters
{
    //! Enables depth testing.
    bool enableTest;
    //! Enables depth writing.
    bool enableWrite;
    //! The depth comparison operator.
    comparison_function comparisonFunction;
};


//! A structure, which describes the stencil state.
struct gfx_stencil_state_parameters
{
    //! Enables stencil testing.
    bool enableTest;
    //! Identify a portion of the depth-stencil buffer for reading stencil data.
    uint8_t readMask;
    //! Identify a portion of the depth-stencil buffer for writing stencil data.
    uint8_t writeMask;
};


//! A structure, which describes the depth-stencil state.
struct gfx_depth_stencil_state_parameters
{
    //! Depth state.
    gfx_depth_state_parameters depth;
    //! Stencil state.
    gfx_stencil_state_parameters stencil;
};


//! A class, which represents a depth-stencil state.
class GO_API gfx_depth_stencil_state
{
public:
    //! \overload
    gfx_depth_stencil_state() = default;
    /*!
        Constructor.

        \param options The state options.
     */
    gfx_depth_stencil_state(const gfx_depth_stencil_state_parameters &options);
public:
    //! Releases all internal resources.
    void release();
    /*!
        Re-initializes the object.

        \param options The state options.
     */
    void reset(const gfx_depth_stencil_state_parameters &options);
    /*!
        A pointer to the internal depth-stencil state object.

        \return A pointer to the internal D3D depth-stencil state.
     */
    ID3D11DepthStencilState *d3d_depth_stencil_state() const;
private:
    //! The depth-stencil state.
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_d3dState;
};

GO_END_NAMESPACE