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
    \file graphics/model_cache.h
    \brief Contains a class that holds built-in models.

    \author Gokhan Ozdogan
    \version 1.0
 */


#pragma once


#include "go/core/common.h"
#include "go/graphics/sphere_model.h"


GO_BEGIN_NAMESPACE

//! A structure, which holds information about the model cache initialization parameters.
struct gfx_model_cache_startup_parameters
{
    //! The number of refinement steps to run on the default ico-sphere.
    uint32_t icoSphereRefinementCount;
};


//! A class, that holds instances of commonly used models.
class gfx_model_cache
{
public:
    /*!
        Constructor.

        \param parameters The startup parameters.
     */
    gfx_model_cache(const gfx_model_cache_startup_parameters &parameters);
public:
    /*!
        Resets the model cache.

        \param parameters The startup parameters.
     */
    void reset(const gfx_model_cache_startup_parameters &parameters);
    /*!
        Returns a pointer to the unit ico-sphere model.

        \return The unit ico-sphere model.
     */
    gfx_icosphere_model &unit_ico_sphere();
private:
    //! The unit ico-sphere.
    gfx_icosphere_model m_unitIcoSphere;
};

GO_END_NAMESPACE
