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


#include "go/stdafx.h"


#include "go/graphics/asset_management/texture_manager.h"


#include "go/filesystem/async.h"
#include "go/core/string_utility.h"
#include "go/graphics/api/device.h"


// ================================================================================ //
// async_loader loader
// ================================================================================ //

struct async_loader : public go::async_io_callback
{
    bool handle_request_completed(const go::async_io_request *request) override
    {
        auto mgr = (go::gfx_texture_loader::manager_ptr_t)request->sender;
        auto desc = (go::gfx_texture_loader::resource_desc_ptr_t)request->userData;

        auto result = desc->resource.reset(request->dataBuffer, request->dataSize, false);
        if(result)
        {
            mgr->loader_update_resource_success(desc);
        }
        else
        {
            handle_request_failed(request);
        }

        return result;
    }
    void handle_request_canceled(const go::async_io_request *request) override
    {
        auto mgr = (go::gfx_texture_loader::manager_ptr_t)request->sender;
        auto desc = (go::gfx_texture_loader::resource_desc_ptr_t)request->userData;

        mgr->loader_update_resource_failure(desc);
    }
    void handle_request_failed(const go::async_io_request *request) override
    {
        handle_request_canceled(request);
        GO_LOGFILE_WARNING("Could not load texture '%s'!\n", request->fileName);
    }
    void handle_request_closed(const go::async_io_request *request) override
    {
    }
};


// ================================================================================ //
// go::gfx_texture_loader member functions
// ================================================================================ //

go::gfx_texture_loader::gfx_texture_loader()
{
    m_asyncLoader = std::make_unique<async_loader>();
}


bool go::gfx_texture_loader::load(manager_ptr_t mgr, resource_desc_ptr_t desc)
{
    auto texDef = mgr->texture_db().find(desc->id);
    if(!texDef)
    {
        return false;
    }

    go::binary_file texFile(texDef->filePath);
    auto texData = texFile.read();
    return desc->resource.reset(texData.data(), texData.size(), false);
}


bool go::gfx_texture_loader::load_async(manager_ptr_t mgr, resource_desc_ptr_t desc)
{
    auto texDef = mgr->texture_db().find(desc->id);
    if(!texDef)
    {
        return false;
    }

    return go::the_io_manager->read(m_asyncLoader.get(), texDef->filePath, 0, 0, mgr, desc);
}


bool go::gfx_texture_loader::reload_async(manager_ptr_t mgr, resource_desc_ptr_t desc)
{
    return load_async(mgr, desc);
}


void go::gfx_texture_loader::unload(resource_ptr_t resource)
{
    resource->release();
}


// ================================================================================ //
// go::texture_manager member functions
// ================================================================================ //

go::gfx_texture_manager::gfx_texture_manager(gfx_texture_database &textureDB,
    const default_textures_info_t &defaultTexturesInfo)
    : base_t(textureDB.capacity(), defaultTexturesInfo.count), m_textureDB(textureDB)
{
    // Create all default textures
    m_defaultTextures.resize(defaultTexturesInfo.count);
    for(auto i = 0U; i < defaultTexturesInfo.count; ++i)
    {
        auto &defaultTex = m_defaultTextures[i];
        allocate_fallback_resource(&defaultTex);

        // Create the texture
        defaultTex->reset(defaultTexturesInfo.elements[i].format, defaultTexturesInfo.elements[i].initialData);
    }
}


auto go::gfx_texture_manager::texture_db() const noexcept -> gfx_texture_database &
{
    return m_textureDB;
}


auto go::gfx_texture_manager::default_textures() noexcept -> resource_ptr_t *
{
    return m_defaultTextures.data();
}