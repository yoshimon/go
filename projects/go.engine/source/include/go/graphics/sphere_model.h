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
    \file graphics/sphere_model.h
    \brief Contains classes that can represent spherical 3D models.

    \author Gokhan Ozdogan
    \version 1.0
 */


#pragma once


#include "go/core/common.h"
#include "go/graphics/model.h"


GO_BEGIN_NAMESPACE

//! A class, which represents a 3D ico-sphere model.
class GO_API gfx_icosphere_model : public gfx_model
{
    GO_DECLARE_NON_COPYABLE(gfx_icosphere_model)
public:
    /*!
        Constructor.

        \param refinementIterations The number of refinement iterations to run on the initial sphere.
     */
    gfx_icosphere_model(uint32_t refinementIterations);
    //! \overload
    gfx_icosphere_model() = default;
public:
    /*!
        Re-initializes the object.

        \param refinementIterations The number of refinement iterations to run on the initial sphere.
     */
    void reset(uint32_t refinementIterations);
    /*!
        Returns the level-of-detail indices of the mesh.

        \param level The LOD level to query.
        \param indexOffset The LOD index offset.
        \param indexCount The LOD index count.
     */
    void lod(uint32_t level, uint32_t &indexOffset, uint32_t &indexCount);
};

GO_END_NAMESPACE
