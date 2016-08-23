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
    \file graphics/camera_transform.h
    \brief Contains classes which represent camera transformations.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/graphics/frustum.h"


GO_BEGIN_NAMESPACE

//! A structure, that holds information about a set of camera transforms.
struct gfx_camera_transform
{
    //! The view matrix.
    DirectX::XMFLOAT4X4 viewMatrix;
    //! The projection matrix.
    DirectX::XMFLOAT4X4 projectionMatrix;
    //! The view-projection matrix.
    DirectX::XMFLOAT4X4 viewProjectionMatrix;
};


//! A structure, that holds information about a perspective camera transform.
struct gfx_perspective_camera_transform : public gfx_camera_transform
{
    //! The camera frustum.
    gfx_perspective_frustum frustum;
};


//! A structure, that holds information about an orthographic camera transform.
struct gfx_orthographic_camera_transform : public gfx_camera_transform
{
    //! The camera frustum.
    gfx_orthographic_frustum frustum;
};

GO_END_NAMESPACE