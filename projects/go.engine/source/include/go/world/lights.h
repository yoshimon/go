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
    \file world/lights.h
    \brief Contains type declaration for the different lights.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! A structure, that represents a base light descriptor.
struct base_light_desc
{
    //! The color of the light.
    DirectX::XMFLOAT3 color;
    //! The luminous power of the light (in lm).
    float luminousPower;
    //! Indicates whether the light casts shadows.
    bool32_t castsShadows;
};


//! A struct, that represents a descriptor for a spherical light.
struct spherical_light_desc : public base_light_desc
{
    //! The size of the light in world units (sphere radius).
    float size;
    //! The influence radius in world units.
    float influenceRadius;
};

//! A struct, that represents a descriptor for a disk light.
struct disk_light_desc : public base_light_desc
{
    //! The size of the light in world units (disk radius).
    float size;
    //! The outer cone angle.
    float coneOuterAngle;
    //! The inner cone angle.
    float coneInnerAngle;
};


//! A struct, that represents a descriptor for a directional light.
struct directional_light_desc : base_light_desc
{
};

GO_END_NAMESPACE