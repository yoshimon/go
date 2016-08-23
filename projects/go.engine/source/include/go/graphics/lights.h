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
    \file graphics/lights.h
    \brief Contains classes that represent lights.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "dfx/namespaces/lights/namespaces/proxy_shapes/namespace_types.h"
#include "dfx/namespaces/lights/namespaces/parameters/namespace_types.h"


GO_BEGIN_NAMESPACE

//! A structure, that holds information about spherical lights.
struct gfx_spherical_lights
{
    typedef dfx_hlsl::lights::proxy_shapes::sphere proxy_shape_t;
    typedef dfx_hlsl::lights::parameters::spherical parameters_t;
    typedef std::vector<proxy_shape_t> shape_array_t;
    typedef std::vector<parameters_t> parameters_array_t;
    //! A list of shape descriptors for each light.
    shape_array_t shapes;
    //! A list of parameter descriptors for each light.
    parameters_array_t parameters;
};


//! A structure, that holds information about disk lights.
struct gfx_disk_lights
{
    typedef dfx_hlsl::lights::proxy_shapes::sphere proxy_shape_t;
    typedef dfx_hlsl::lights::parameters::disk parameters_t;
    typedef std::vector<proxy_shape_t> shape_array_t;
    typedef std::vector<parameters_t> parameters_array_t;
    //! A list of shape descriptors for each light.
    shape_array_t shapes;
    //! A list of parameter descriptors for each light.
    parameters_array_t parameters;
};

GO_END_NAMESPACE