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
    \file graphics/render_points.h
    \brief Contains classes that represents points in space to render from.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"
#include "go/graphics/render_command.h"
#include "go/graphics/camera_transform.h"
#include "go/graphics/camera_properties.h"


GO_BEGIN_NAMESPACE

//! Enumeration of possible light kinds.
enum class gfx_render_point_kind
{
    //! A regular camera.
    camera,
    //! A directional light.
    directional_light,
    //! A spherical light.
    spherical_light,
    //! A disk light.
    disk_light
};


//! A structure, which represents a render point.
struct gfx_render_point
{
	//! State flag.
	bool32_t isActive = false;
	/*!
		A unique identifier for the render point.

		The identifier has to be unique among a render point group (for example point lights).
	 */
	uint32_t id = 0;
	/*!
		Forces a redraw of this render point.

		If this is set to false, the backend may cache the previous results (shadow maps for example).
	 */
	bool32_t forceRedraw = true;
	/*!
		A weight for this render-point.

		The use of this value depends on the semantics of the render-point.
	 */
	float importance = 1.0f;
    //! The render point kind.
    gfx_render_point_kind kind;
};


//! A structure, which represents a render-point for a cubemap.
struct gfx_cube_render_point : public gfx_render_point
{
    //! The command buffers for each face.
    std::array<gfx_render_command_buffers, 6> faceCommandBuffers;
    //! The per-face transformation matrices.
	std::array<gfx_perspective_camera_transform, 6> faceTransforms;
    //! The active faces.
    std::array<bool32_t, 6> isFaceActive;
};


//! A structure, which represents a single perspective render-point.
struct gfx_perspective_render_point : public gfx_render_point
{
    //! The command buffers.
    gfx_render_command_buffers commandBuffers;
    //! The render point transformations.
    gfx_perspective_camera_transform transform;
};


//! A structure, which represents a single perspective camera render-point.
struct gfx_camera_render_point : public gfx_perspective_render_point
{
    //! The camera properties of the render-point.
    gfx_camera_properties cameraProperties;
};


//! A structure, which represents a single perspective render-point.
struct gfx_orthographic_render_point : public gfx_render_point
{
    //! The command buffers.
    gfx_render_command_buffers commandBuffers;
    //! The render point transformations.
    gfx_orthographic_camera_transform transform;
};

GO_END_NAMESPACE