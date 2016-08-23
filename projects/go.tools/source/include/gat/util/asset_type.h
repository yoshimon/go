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

#pragma once

#include "gat/defines.h"


GAT_BEGIN_NAMESPACE

//! An enumeration of supported asset types.
enum class asset_type : int32_t
{
    //! The content represents a 3D model.
    model = 0,
    //! The content represents a material.
    material,
    //! The content represents a texture.
    texture,
    //! The content represents an animation.
    animation,
    //! The number of asset types.
    count
};


//! An array, containing the string representations of enum values.
extern const _TCHAR *asset_type_string[(int32_t)asset_type::count];


/*!
    Returns the asset type enum value for a string.

    \param value The enum value string name.

    \return The asset type enum value.
 */
asset_type asset_type_enum(const _TCHAR *value);

GAT_END_NAMESPACE