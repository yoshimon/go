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
    \file graphics/d3d11/dfx/forward_plus/effects/prepass.h
    \brief Contains a structure with the parameters for the Forward+ prepass effect.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/graphics/d3d11/dfx/render_util.h"


GO_BEGIN_NAMESPACE

class gfx_shader_resource;
struct gfx_perspective_render_point;
class gfx_depth_stencil_buffer;

namespace dfx_effects
{
    namespace forward_plus
    {
        //! A structure, which contains the prepass parameters.
        struct prepass_parameters : public dfx_util::effect_parameters
        {
            struct
            {
                //! A buffer, containing the proxy shapes for the spherical lights.
                gfx_shader_resource *sphericalLightProxyShapes;
                //! The number of spherical lights.
                uint32_t numSphericalLights;
                //! A buffer, containing the proxy shapes for the disk lights.
                gfx_shader_resource *diskLightProxyShapes;
                //! The number of disk lights.
                uint32_t numDiskLights;

                //! The render-point to use.
                const gfx_perspective_render_point *renderPoint;
            } in;
            struct
            {
                //! The light-tile mapping buffer for spherical lights.
                gfx_shader_resource *sphericalLightTileIndices;
                //! The light-tile mapping buffer for disk lights.
                gfx_shader_resource *diskLightTileIndices;

                //! The depth-stencil buffer to use.
                gfx_depth_stencil_buffer *depthStencilBuffer;
            } out;
        };
    }
}

GO_END_NAMESPACE
