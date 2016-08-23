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

#include "gat/util/asset_type.h"


#include "gat/util/exception.h"


const _TCHAR *gat::asset_type_string[] =
{
    //! The content represents a 3D model.
    _T("model"),
    //! The content represents a material.
    _T("material"),
    //! The content represents a texture.
    _T("texture"),
    //! The content represents an animation.
    _T("animation")
};


//===========================================================================
// gat::asset_type_enum
//===========================================================================

gat::asset_type gat::asset_type_enum(const _TCHAR *value)
{
    auto i = 0;
    for(auto &s : asset_type_string)
    {
        if(!tstrcmp(s, value))
        {
            return (asset_type)i;
        }

        ++i;
    }

    _tprintf("'%s' is not a valid asset type.\n", value);
    throw tchar_exception(_T("Invalid asset type detected."));
}