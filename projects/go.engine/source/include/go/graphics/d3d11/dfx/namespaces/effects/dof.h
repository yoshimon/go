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
    \file graphics/d3d11/dfx/effects/dof.h
    \brief Contains type declarations for the depth-of-field effect.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

class gfx_texture;
class gfx_depth_stencil_buffer;
class gfx_render_target;

namespace dfx_effects
{
    //! A structure, which contains the DoF parameters.
    struct dof_parameters
    {
        struct
        {
            //! The texture to apply the DoF effect on.
            go::gfx_texture *colorTexture;
            //! The view-space depth texture.
            go::gfx_depth_stencil_buffer *depthVSTexture;
            //! The conversion factor from CoC to pixels.
            float cocToPx;
            //! The end of the near-region.
            float nearEnd;
            //! The range of the near-region.
            float nearRange;
            //! The range of the far-region.
            float farRange;
            //! The strength of the DoF effect.
            float strength;
        } in;
        struct
        {
            //! The CoC render-target.
            go::gfx_render_target *cocTexture;
            //! A temporary buffer to write the horizontal DoF result into.
            go::gfx_texture *scratchBuffer;
            //! The target texture to write the DoF effect to.
            go::gfx_texture *completeDoF;
        } out;
    };
}

GO_END_NAMESPACE
