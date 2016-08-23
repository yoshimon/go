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
    \file graphics/asset_management/material_manager.h
    \brief Contains classes for managing different materials.

    \author Gokhan Ozdogan
    \version 1.0
 */


#pragma once


#include "go/core/common.h"
#include "go/graphics/asset_management/texture_manager.h"


GO_BEGIN_NAMESPACE

class gfx_material_database;
struct gfx_material_definition;


//! A class, which manages a set of materials.
class GO_API gfx_material_manager
{
public:
    typedef gfx_texture_manager texture_manager_t;
public:
    /*!
        Constructor.

        \param materialDB The material database.
        \param textureMgr The texture manager.
     */
    gfx_material_manager(gfx_material_database &materialDB, texture_manager_t &textureMgr)
        : m_materialDB(materialDB), m_textureMgr(textureMgr)
    {
        // Initialize the material data
        m_lastUsageTimes.resize(materialDB.capacity());
        // NOTE: the default constructor will initialize the times to zero
    }
public:
    /*!
        Returns the material database that was associated with this manager.

        \return The material database that was associated with this manager.
     */
    gfx_material_database &material_db() noexcept { return m_materialDB; }
    /*!
        Returns the texture manager that was associated with this manager.

        \return The texture manager that was associated with this manager.
     */
    texture_manager_t &texture_mgr() noexcept { return m_textureMgr; }
    /*!
        Loads the textures of a material into the associated texture manager.

        It is safe to call this function concurrently.

        \param material The material to load.
        \param evictionPolicy The eviction policy to use.
     */
    void load_material_textures(
        const gfx_material_definition *material,
        resource_eviction_policy evictionPolicy = resource_eviction_policy::default
    );
private:
    //! The material database.
    gfx_material_database &m_materialDB;
    //! The texture database.
    texture_manager_t &m_textureMgr;
    //! Information about the last time a material was used.
    std::vector<uint32_t> m_lastUsageTimes;
};

GO_END_NAMESPACE
