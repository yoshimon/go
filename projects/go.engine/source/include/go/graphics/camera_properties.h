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
    \file graphics/camera_properties.h
    \brief Contains structures that represent camera properties.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

//! A structure, which holds information about a camera.
struct gfx_camera_properties
{
    //! The near-clip.
    float nearClip = 1.0f;
    //! The far-clip.
    float farClip = 80.0f;
    //! The vertical field-of-view.
    float fovY = 1.3;
    //! Color grading (normalized) blend factor.
    float colorGradingBlendFactor = 0.0f;
    //! The color grading source texture.
    std::string colorGradingSourceTexture = "assets\\color_lut\\default.dds";
    //! The color grading destination texture.
    std::string colorGradingDestinationTexture = "assets\\color_lut\\default.dds";
    //! The near DOF range size.
    float dofNearRange = 1.0f;
    //! The focus DOF range size.
    float dofFocusRange = 10.0f;
    //! The far DOF range size.
    float dofFarRange = 100.0f;
};

GO_END_NAMESPACE
