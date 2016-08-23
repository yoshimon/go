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
    \file graphics/d3d11/dfx/forward_plus/effects/ambient_lighting.h
    \brief Contains a structure with the parameters for the Forward+ ambient effect.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/graphics/d3d11/dfx/render_util.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

class gfx_texture;
class gfx_render_target;

namespace dfx_effects
{
    namespace forward_plus
    {
        //! A structure, which contains the ambient-lighting parameters.
        struct ambient_lighting_parameters
        {
            struct
            {
                //! The world-space normal texture.
                gfx_texture *normalWSTexture;
                //! The ambient-occlusion texture to mask the diffuse lighting with.
                gfx_texture *aoTexture;
                //! The metallic-roughness mask.
                gfx_texture *metallicRoughnessTexture;
                //! The albedo texture.
                gfx_texture *albedoTexture;
                //! The view-space depth texture.
                gfx_texture *depthVSTexture;
                //! The pre-integrated environment BRDF.
                gfx_texture *envBRDFTexture;
                //! The scene environment map.
                gfx_texture *sceneSpecularEnvMap;
                //! The ambient color from above in world-space.
                DirectX::XMFLOAT3 ambientColorAboveWS;
                //! The ambient color from above in world-space.
                DirectX::XMFLOAT3 ambientColorBelowWS;
            } in;
            struct
            {
                //! The output buffer to blend the result into.
                gfx_render_target *colorTexture;
            } out;
        };
    }
}

GO_END_NAMESPACE
