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
    \file graphics/asset_management/shader_manager.h
    \brief Contains a class for managing shader resources.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/path_utility.h"
#include "go/design/simple_resource_manager.h"
#include "go/graphics/api/shader.h"


GO_BEGIN_NAMESPACE

class gfx_shader_manager;


//! A helper class for loading shader files.
struct GO_API gfx_shader_loader : public simple_resource_loader<gfx_shader, file_path>
{
    typedef gfx_shader_manager manager_t;
    /*!
        Loads a resource from file.

        \tparam Args The load parameter types.

        \param key The loader parameter.
        \param type The shader type.

        \return The shader resource.
     */
    static gfx_shader *load(manager_t *, const storage_key_t &key, gfx_shader_type type);
};


//! A class, which manages shader resources.
class GO_API gfx_shader_manager : public simple_resource_manager<gfx_shader_loader>
{
public:
    //! Constructor.
    gfx_shader_manager() = default;
};

GO_END_NAMESPACE