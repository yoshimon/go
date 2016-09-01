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
    \file world/sky.h
    \brief Contains a class which describes a sky.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

class gfx_texture;

//! A structure, which describes a sky.
struct gfx_ambient_sky
{
    //! The direction of the sunlight.
    DirectX::XMFLOAT3 sunDirection = DirectX::XMFLOAT3(0, -1, 0);
    //! The sunlight color.
    DirectX::XMFLOAT3 sunColor = DirectX::XMFLOAT3(1, 1, 1);
    //! Ambient color from above.
    DirectX::XMFLOAT3 ambientColorAbove = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);
    //! Ambient color from below.
    DirectX::XMFLOAT3 ambientColorBelow = DirectX::XMFLOAT3(0, 0, 0);
    //! The base color of the sky.
    DirectX::XMFLOAT3 color = DirectX::XMFLOAT3(0, 0, 0);
    //! The fog color.
    DirectX::XMFLOAT3 fogColor = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);
    //! The density of the fog.
    float fogDensity = 0.001f;
    //! The volumetric lighting intensity.
    float volumetricLightingIntensity = 1.0f;
    //! Volumetric light distance scale.
    float volumetricLightingDistanceScale = 0.1f;
    //! The sky material.
    std::string material;
    //! The sky environment map material to use.
    gfx_texture *specularEnvMap;
};

GO_END_NAMESPACE
