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
    \file graphics/d3d11/dfx/effects/shadow_maps.h
    \brief Contains type declarations for the shadow map effect.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/graphics/texture2d_atlas.h"
#include "go/graphics/d3d11/dfx/render_util.h"


GO_BEGIN_NAMESPACE

class gfx_depth_stencil_buffer;
struct gfx_orthographic_render_point;
struct gfx_cube_render_point;
struct gfx_perspective_render_point;

namespace dfx_effects
{
    //! A structure, which contains the FXAA parameters.
    struct shadow_maps_parameters : public dfx_util::effect_parameters
    {
        struct
        {
            //! The render-point to use for the directional light.
            const gfx_orthographic_render_point *directionalLightRenderPoint;
            //! The point-light render-points to use.
            const std::array<gfx_cube_render_point, DFX_MAX_NUM_SHADOW_CASTING_POINT_LIGHTS> *pointLightRenderPoints;
            //! The point-light render-points to use.
            const std::array<gfx_perspective_render_point, DFX_MAX_NUM_SHADOW_CASTING_SPOT_LIGHTS> *spotLightRenderPoints;
        } in;
        struct
        {
            //! The output depth-stencil buffer for the directional light.
            gfx_depth_stencil_buffer *directionalLightShadowMap;
            //! The shadow map atlas.
            gfx_depth_stencil_buffer_regular_atlas *pointLightShadowMapAtlas;
            //! The shadow map atlas.
            gfx_depth_stencil_buffer_regular_atlas *spotLightShadowMapAtlas;
        } out;
    };
}

GO_END_NAMESPACE