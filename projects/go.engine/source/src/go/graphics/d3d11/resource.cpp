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


#include "go/graphics/d3d11/resource.h"


// ================================================================================ //
// go::gfx_shader_resource member functions
// ================================================================================ //

void go::gfx_shader_resource::release()
{
    // TODO: unbind from renderer
    for(auto &m : internalOutputBindMask)
    {
        m.clear_all();
    }

    for(auto &m : internalPendingOutputBindMask)
    {
        m.clear_all();
    }

    for(auto &m : internalInputBindMask)
    {
        m.clear_all();
    }

    for(auto &m : internalPendingInputBindMask)
    {
        m.clear_all();
    }
}