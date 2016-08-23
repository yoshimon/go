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
    \file graphics/render_context.h
    \brief Contains a class that represents an executable render context.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"
#include "go/graphics/render_command.h"
#include "go/graphics/render_points.h"
#include "go/graphics/lights.h"
#include "go/graphics/sky.h"


GO_BEGIN_NAMESPACE

//! A structure, which holds information about the command buffers and their associated transformations.
struct gfx_render_context
{
    //! A structure, which holds the render points.
    struct
    {
        //! The render point for the main camera.
        gfx_camera_render_point main;
        //! The render points for the shadow-casting directional light.
        std::array<gfx_orthographic_render_point, DFX_NUM_DIRECTIONAL_LIGHT_CASCADES> directionalLight;
        //! The render points for the shadow-casting point lights.
        std::array<gfx_cube_render_point, DFX_MAX_NUM_SHADOW_CASTING_POINT_LIGHTS> pointLights;
        //! The render points for the shadow-casting spot lights.
        std::array<gfx_perspective_render_point, DFX_MAX_NUM_SHADOW_CASTING_SPOT_LIGHTS> spotLights;
    } renderPoints;
    //! A structure, which holds all lights in the scene.
    struct
    {
        //! The spherical lights.
        gfx_spherical_lights spherical;
        //! The disk lights.
        gfx_disk_lights disk;
        //! A structure, which holds indices to the visible lights.
        struct
        {
            //! The visible spherical lights.
            index_vector spherical;
            //! The visible disk lights.
            index_vector disk;
        } visible;
        //! A structure, which holds indices to the shadow-casting lights.
        struct
        {
            //! The shadow-casting spherical lights.
            index_vector spherical;
            //! The shadow-casting disk lights.
            index_vector disk;
        } shadowCasting;
    } lights;
    //! The sky descriptor.
    gfx_ambient_sky sky;
    //! Initializes the render context.
    void initialize();
    //! Sorts the command buffers and updates the per-instance data buffers of each command buffer.
    void finalize();
    //! Clears all command buffers.
    void clear_command_buffers();
    //! Resets all the light caches.
    void reset_light_caches();
    //! Resets the context for the next frame.
    void reset_frame();
};

GO_END_NAMESPACE