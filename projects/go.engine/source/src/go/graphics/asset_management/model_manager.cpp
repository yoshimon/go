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


#include "go/graphics/asset_management/model_manager.h"


#include "go/filesystem/views/native/gmd_file.h"
#include "go/core/string_utility.h"
#include "go/variables.h"


using namespace DirectX;


// ================================================================================ //
// Helper functions
// ================================================================================ //

void apply_mesh_materials(go::gfx_material_database &materialDB,
    go::gfx_model *model, const go::gfx_model_definition *modelDef)
{
    // Clamp the material index if it is larger than the number of meshes
    int capacity = (int)modelDef->materials.size() - 1;
    if(capacity >= 0)
    {
        int matIndex = 0;

        // Unskinned
        for(auto &m : model->unskinnedMeshes)
        {
            m.material = modelDef->materials[std::min(matIndex, capacity)];
            ++matIndex;
        }

        // Skinned
        for(auto &m : model->skinnedMeshes)
        {
            m.material = modelDef->materials[std::min(matIndex, capacity)];
            ++matIndex;
        }
    }
    else
    {
        auto defaultMat = materialDB.find(GO_CVAR(default_material_name).value());

        // Unskinned
        for(auto &m : model->unskinnedMeshes)
        {
            m.material = defaultMat;
        }

        // Skinned
        for(auto &m : model->skinnedMeshes)
        {
            m.material = defaultMat;
        }
    }
}


// ================================================================================ //
// GMD loader
// ================================================================================ //

struct gmd_loader : public go::async_io_callback
{
    bool handle_request_completed(const go::async_io_request *request) override
    {
        auto mgr = (go::gfx_model_loader::manager_ptr_t)request->sender;
        auto desc = (go::gfx_model_loader::resource_desc_ptr_t)request->userData;

        // NOTE: this is just a rough approximation for the size
        desc->size = request->dataSize;

        // Grab resource ptr & definition
        auto resource = &desc->resource;
        auto modelDef = resource->definition;

        // Load from memory into resource
        auto result = go::gmd_file::load((const uint8_t *)request->dataBuffer, request->dataSize, *resource);
        if(result)
        {
            // Apply materials to each mesh
            apply_mesh_materials(mgr->material_db(), resource, modelDef);

            // No skeleton => we are done
            mgr->loader_update_resource_success(desc);
        }
        else
        {
            // Load mesh failed
            handle_request_failed(request);
        }

        return result;
    }
    void handle_request_canceled(const go::async_io_request *request) override
    {
        auto mgr = (go::gfx_model_loader::manager_ptr_t)request->sender;
        auto desc = (go::gfx_model_loader::resource_desc_ptr_t)request->userData;
        mgr->loader_update_resource_failure(desc);
    }
    void handle_request_failed(const go::async_io_request *request) override
    {
        handle_request_canceled(request);
        GO_LOGFILE_WARNING("Could not load model '%s'!\n", request->fileName);
    }
    void handle_request_closed(const go::async_io_request *request) override
    {
    }
};


// ================================================================================ //
// go::gfx_model_loader member functions
// ================================================================================ //

go::gfx_model_loader::gfx_model_loader()
{
    m_asyncLoader = std::make_unique<gmd_loader>();
}


bool go::gfx_model_loader::load(manager_ptr_t mgr, resource_desc_ptr_t desc)
{
    // Lookup model definition
    auto modelDef = mgr->model_db().find(desc->id);
    if(!modelDef)
    {
        return false;
    }

    // Save definition
    auto &resource = desc->resource;
    resource.definition = modelDef;

    // TODO: size estimate

    // Load meshes
    if(!go::gmd_file::load(modelDef->filePath, resource))
    {
        return false;
    }
    
    // Apply materials to each submesh
    apply_mesh_materials(mgr->material_db(), &resource, modelDef);

    return true;
}


bool go::gfx_model_loader::load_async(manager_ptr_t mgr, resource_desc_ptr_t desc)
{
    // Lookup model definition
    auto modelDef = mgr->model_db().find(desc->id);
    if(!modelDef)
    {
        return false;
    }

    // Save model definition
    desc->resource.definition = modelDef;

    go::the_io_manager->read(m_asyncLoader.get(), modelDef->filePath, 0, 0, mgr, desc);

    return true;
}


bool go::gfx_model_loader::reload_async(manager_ptr_t mgr, resource_desc_ptr_t desc)
{
    return load_async(mgr, desc);
}


void go::gfx_model_loader::unload(resource_ptr_t resource)
{
    resource->release();
}


// ================================================================================ //
// go::gfx_model_manager member functions
// ================================================================================ //

go::gfx_model_manager::gfx_model_manager(gfx_model_database &modelDB, gfx_material_database &materialDB)
    : concurrent_resource_manager(modelDB.capacity(), 1), // 1 fallback model
    m_modelDB(modelDB), m_materialDB(materialDB)
{
    // Create an empty model
    allocate_fallback_resource(&m_defaultModel);
}


auto go::gfx_model_manager::model_db() noexcept -> gfx_model_database &
{
    return m_modelDB;
}


auto go::gfx_model_manager::material_db() noexcept -> gfx_material_database &
{
    return m_materialDB;
}


auto go::gfx_model_manager::default_model() noexcept -> gfx_model *
{
    return m_defaultModel;
}