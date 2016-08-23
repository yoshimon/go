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
    \file graphics/skeletal_animation.h
    \brief Contains classes for representing skeletal animations.

    \author Gokhan Ozdogan
    \version 1.0
 */


#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

//! A structure, which represents a gfx_bone in a skeletal animation.
struct gfx_bone
{
    //! The parent gfx_bone index or -1 if root.
    int32_t parent;
    //! The gfx_bone scale. (Unused)
    float scale;
    //! The local transformation.
    float localTransform[4][3];
};

typedef std::vector<gfx_bone> gfx_bone_vector;


//! A structure, which represents a sample in a skeletal animation.
struct gfx_skeletal_animation_sample
{
    //! The rotation sample.
    DirectX::XMFLOAT4 rotation;
    //! The position sample.
    DirectX::XMFLOAT3 position;
};


typedef std::vector<gfx_skeletal_animation_sample> gfx_skeletal_animation_sample_vector;

typedef uint32_t gfx_skeletal_animation_id;

//! A structure, which represents a skeletal animation.
struct gfx_skeletal_animation
{
    //! The animation ID inside the collection.
    gfx_skeletal_animation_id id;
    //! The playback speed.
    int32_t fps;
    //! Number of animation samples per gfx_bone.
    int32_t numSamplesPerBone;
    //! The animation samples per gfx_bone.
    gfx_skeletal_animation_sample_vector samples;
};

GO_END_NAMESPACE