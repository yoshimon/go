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
    \file graphics/d3d11/blend_state.h
    \brief Contains a class which represents a GPU blend state.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

/*!
    Enumeration of possible blending modes.

    NOTE: only 8 different blend-modes are allowed!
 */
enum class gfx_blend_mode : uint8_t
{
    //! No blending.
    opaque = 0,
    //! Alpha blending.
    alpha,
    //! Additive blending.
    additive,
    //! Multiplicative blending.
    multiply,
    //! The number of elements in the enumeration.
    count
};


//! A table that contains the string identifiers for the different blend modes.
const char *const gfx_blend_mode_strings[] = { "opaque", "alpha", "additive" };


//! A class, which represents a graphics blend state. 
class GO_API gfx_blend_state
{
    GO_DECLARE_NON_COPYABLE(gfx_blend_state)
public:
    //! \overload
    gfx_blend_state() = default;
    /*!
        Constructor.

        \param type The type of the blend state.
     */
    gfx_blend_state(gfx_blend_mode type);
public:
    //! Releases all internal resources.
    void release();
    /*!
        Re-initializes the object.

        \param type The type of the blend state.
     */
    void reset(gfx_blend_mode type);
    /*!
        A pointer to the internal blend state object.

        \return A pointer to the internal D3D blend state.
     */
    ID3D11BlendState *d3d_blend_state() const noexcept;
private:
    //! The blend state.
    Microsoft::WRL::ComPtr<ID3D11BlendState> m_d3dState;
};

GO_END_NAMESPACE