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
    \file graphics/d3d11/dfx/effects/ssao.h
    \brief Contains type declarations for the SSAO effect.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

class gfx_texture;
class gfx_render_target;

namespace dfx_effects
{
    //! A structure, which contains the SSAO parameters.
    struct ssao_parameters
    {
        struct
        {
            //! The strength of the effect.
            float strength;
            //! The radius of the AO sampling area.
            float radius;
            //! Angular bias to hide low tessellation artifacts.
            float angularBias;
            //! Conversion factor from meters to view-space units.
            float metersToViewSpaceUnits;
            //! The view-space depth texture.
            gfx_texture *depthVSTexture;
            //! The world-space normals texture.
            gfx_texture *normalWSTexture;
            //! The projection matrix used for the depth-buffer generation pass.
			DirectX::XMFLOAT4X4 depthProjMatrix;
			//! The view matrix used for the depth-buffer generation pass.
			DirectX::XMFLOAT4X4 depthViewMatrix;
        } in;
        struct
        {
            //! The output texture.
            gfx_render_target *outputTexture;
        } out;
    };
}

GO_END_NAMESPACE
