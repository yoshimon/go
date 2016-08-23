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
    \file graphics/d3d11/dfx/effects/gaussian_blur.h
    \brief Contains type declarations for the gaussian blur effect.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

class gfx_texture;

namespace dfx_effects
{
    //! A structure, which contains the gaussian blur parameters.
    struct gaussian_blur_parameters
    {
        struct
        {
            //! The texture to blur.
            gfx_texture *sourceTexture;
            //! The source mip-level to blur.
            uint32_t sourceMipLevel;
            //! The scratch mip-level.
            uint32_t scratchMipLevel;
            //! The output mip-level.
            uint32_t destMipLevel;
        } in;
        struct
        {
            //! A temporary buffer to write the horizontal blur result into.
            gfx_texture *scratchTexture;
            //! The destination render-target.
            gfx_texture *destTexture;
        } out;
    };
}

GO_END_NAMESPACE
