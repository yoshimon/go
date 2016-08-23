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


#include "go/world/scene_manager.h"


#include "go/graphics/materials.h"


// ================================================================================ //
// go::scene_manager member functions
// ================================================================================ //

go::scene_manager::scene_manager(go::cdefault_value)
{
    gfx_asset_context_parameters assetContextParameters;
    assetContextParameters.materialDB = { GO_PATH_SYMBOL_WORLD_BASE "materials.gdb", GO_PATH_SYMBOL_ASSETS };
    assetContextParameters.modelDB = { GO_PATH_SYMBOL_WORLD_BASE "models.gdb", GO_PATH_SYMBOL_ASSETS };
    assetContextParameters.textureDB = { GO_PATH_SYMBOL_WORLD_BASE "textures.gdb", GO_PATH_SYMBOL_ASSETS };

    assetContextParameters.shaderDirPath = GO_PATH_SYMBOL_SHADER;
    assetContextParameters.defaultTexturesInfo = go::gfx_default_material_textures_info();

    reset(assetContextParameters);
}


void go::scene_manager::reset(const gfx_asset_context_parameters &assetContextParameters)
{
    m_scenes.clear();
    m_assetContext.reset(assetContextParameters);
}


go::scene *go::scene_manager::create_scene()
{
    m_scenes.push_back(std::make_unique<scene>(this));
    return m_scenes.back().get();
}


void go::scene_manager::destroy_scene(scene *scn)
{
    GO_FOR_EACH_INDEX(current, m_scenes)
    {
        if((*current.iterator).get() == scn)
        {
            m_scenes.erase(current.iterator);
            return;
        }
    }
}


void go::scene_manager::render(size_t index)
{
    if(index < m_scenes.size())
    {
        m_scenes[index]->render();
    }
}


void go::scene_manager::update()
{
    // Update the asset context
    m_assetContext.update();

    // Update all scenes
    for(auto &scn : m_scenes)
    {
        scn->update();
    }
}