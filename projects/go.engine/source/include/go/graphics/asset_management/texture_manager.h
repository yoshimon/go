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
    \file graphics/asset_management/texture_manager.h
    \brief Contains a class for managing texture resources.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/path_utility.h"
#include "go/design/concurrent_resource_manager.h"
#include "go/graphics/asset_management/texture_database.h"
#include "go/graphics/asset_management/material_database.h"
#include "go/graphics/api/texture.h"
#include "go/filesystem/async.h"


GO_BEGIN_NAMESPACE

class gfx_texture_manager;


//! A helper class for loading texture files.
class GO_API gfx_texture_loader : public concurrent_resource_loader<gfx_texture, gfx_texture_manager>
{
public:
    //! Constructor.
    gfx_texture_loader();
public:
    bool load(manager_ptr_t mgr, resource_desc_ptr_t desc) override;
    bool load_async(manager_ptr_t mgr, resource_desc_ptr_t desc) override;
    bool reload_async(manager_ptr_t mgr, resource_desc_ptr_t desc) override;
    void unload(resource_ptr_t desc) override;
private:
    //! The async texture loader.
    std::unique_ptr<go::async_io_callback> m_asyncLoader;
};


//! A class, which manages GPU texture resources.
class GO_API gfx_texture_manager : public concurrent_resource_manager<gfx_texture_loader>
{
public:
    typedef concurrent_resource_manager<gfx_texture_loader> base_t;
public:
    //! A structure, which holds information about a default texture.
    struct default_texture_info_t
    {
        //! An array of data pointers for each texture.
        void *initialData;
        //! An array of texture formats.
        gfx_texture_format format;
    };
    //! A structure, which holds information about the default textures to create.
    struct default_textures_info_t
    {
        //! The default textures.
        default_texture_info_t *elements;
        //! The number of default textures.
        size_t count;
    };
public:
    /*!
        Constructor.

        \param textureDB The texture database.
        \param defaultTexturesInfo Information about the default textures.
     */
    gfx_texture_manager(gfx_texture_database &textureDB, const default_textures_info_t &defaultTexturesInfo);
public:
    /*!
        Returns the texture database that is bound to the manager.

        \return The texture database.
     */
    gfx_texture_database &texture_db() const noexcept;
    /*!
        An array to the default textures.

        This has to match the material slot identifiers.

        \return The array of default textures.
     */
    resource_ptr_t *default_textures() noexcept;
private:
    //! The texture database.
    gfx_texture_database &m_textureDB;
    //! All default textures.
    std::vector<resource_ptr_t> m_defaultTextures;
};

GO_END_NAMESPACE