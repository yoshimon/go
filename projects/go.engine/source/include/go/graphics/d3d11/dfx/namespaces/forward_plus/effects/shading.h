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
    \file graphics/d3d11/dfx/forward_plus/effects/shading.h
    \brief Contains a structure with the parameters for the Forward+ shading effect.

    \author Gokhan Ozdogan
    \version 1.0
*/

#pragma once


#include "go/core/common.h"
#include "go/graphics/texture2d_atlas.h"
#include "go/graphics/d3d11/dfx/render_util.h"


GO_BEGIN_NAMESPACE

struct gfx_perspective_render_point;
struct gfx_ambient_sky;
class gfx_buffer;
class gfx_depth_stencil_buffer;
class gfx_render_target;


namespace dfx_effects
{
    namespace forward_plus
    {
        //! A structure, which contains the shading-related light buffers.
        struct scene_lights
        {
            //! The light parameters.
            gfx_structured_buffer *parameters;
            //! The light proxy shapes.
            gfx_structured_buffer *shapes;
            //! The tile-light indices.
            gfx_structured_buffer *tileIndices;
        };

        //! A structure, containing all shadow maps.
        struct shadow_maps
        {
            //! The shadow map for the directional light.
            gfx_depth_stencil_buffer *directionalLight;
            //! The shadow map atlas for point lights.
            gfx_depth_stencil_buffer_regular_atlas *pointLightAtlas;
            //! The shadow map atlas for spot lights.
            gfx_depth_stencil_buffer_regular_atlas *spotLightAtlas;
        };

        //! A structure, which contains the shading parameters.
        struct shading_parameters : public dfx_util::effect_parameters
        {
            struct
            {
                //! The render-point to use.
                const gfx_perspective_render_point *renderPoint;
                //! The sky descriptor.
                const gfx_ambient_sky *sky;
                //! The spherical lights.
                scene_lights sphericalLights;
                //! The disk lights.
                scene_lights diskLights;
                //! The shadow maps.
                shadow_maps shadowMaps;
            } in;
            struct
            {
                //! The depth-stencil buffer to use.
                gfx_depth_stencil_buffer *depthStencilBuffer;
                //! The world-space normal buffer.
                gfx_render_target *normalWSRT;
                //! The view-space depth buffer.
                gfx_render_target *depthVSRT;
                //! The albedo buffer.
                gfx_render_target *albedoRT;
                //! The metallic-roughness buffer.
                gfx_render_target *metallicRoughnessRT;
                //! The fragment color buffer.
                gfx_render_target *colorRT;
                //! The ambient occlusion buffer.
                gfx_render_target *ambientOcclusionRT;
            } out;
        };
    }
}

GO_END_NAMESPACE
