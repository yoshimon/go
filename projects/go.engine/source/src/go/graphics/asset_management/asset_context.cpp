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


#include "go/graphics/asset_management/asset_context.h"


// ================================================================================ //
// go::gfx_asset_context member functions
// ================================================================================ //

void go::gfx_asset_context::update()
{
    if(m_modelMgr)
    {
        m_modelMgr->update();
        m_textureMgr->update();
    }
}


void go::gfx_asset_context::reset(const gfx_asset_context_parameters &parameters)
{
    // Free everything
    m_textureMgr.reset();
    m_textureDB.reset();

    m_modelMgr.reset();
    m_modelDB.reset();

    m_materialMgr.reset();
    m_materialDB.reset();

    // Textures
    if(!parameters.textureDB.filePath.is_empty())
    {
        m_textureDB = std::make_unique<gfx_texture_database>();
        m_textureDB->load(parameters.textureDB);
        m_textureMgr = std::make_unique<gfx_texture_manager>(*m_textureDB.get(), parameters.defaultTexturesInfo);
    }

    // Materials
    if(!parameters.materialDB.filePath.is_empty())
    {
        m_materialDB = std::make_unique<gfx_material_database>(*m_textureDB.get());
        m_materialDB->load(parameters.materialDB);
        m_materialMgr = std::make_unique<gfx_material_manager>(*m_materialDB.get(), *m_textureMgr.get());
    }

    // Models
    if(!parameters.modelDB.filePath.is_empty())
    {
        m_modelDB = std::make_unique<gfx_model_database>(*m_materialDB.get());
        m_modelDB->load(parameters.modelDB);
        m_modelMgr = std::make_unique<gfx_model_manager>(*m_modelDB.get(), *m_materialDB.get());
    }

    // Shaders
    if(!parameters.shaderDirPath.is_empty())
    {
        m_shaderMgr = std::make_unique<gfx_shader_manager>();

        go::file_system::enumerate_files(
            parameters.shaderDirPath, // Look through the shader directory
            std::regex(".cso"), // Find compiled shaders
            file_system::enum_predicate::no_directories, // No directories
            [this](const char *fileName, bool, uint64_t)
        {
            // Deduce the shader type from its prefix
            file_path fp(fileName);
            auto len = fp.length();
            // Looking for *<x>s.cso, x = shader stage letter
            if(len >= 6)
            {
                if(fp[len - 5] == 's')
                {
                    switch(fp[len - 6])
                    {
                    case 'v': m_shaderMgr->load(fp, gfx_shader_type::vertex); break;
                    case 'p': m_shaderMgr->load(fp, gfx_shader_type::pixel); break;
                    case 'c': m_shaderMgr->load(fp, gfx_shader_type::compute); break;
                    case 'g': m_shaderMgr->load(fp, gfx_shader_type::geometry); break;
                    }
                }
            }
            return true;
        });

        // Model cache
        {
            go::gfx_model_cache_startup_parameters mcsp;
            mcsp.icoSphereRefinementCount = 4; // 8 segments in MAX
            m_modelCache = std::make_unique<gfx_model_cache>(mcsp);
        }
    }

    // Store parameters
    m_parameters = parameters;
}