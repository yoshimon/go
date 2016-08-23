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


#include "go/graphics/asset_management/material_manager.h"


#include "go/graphics/api/renderer.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

void go::gfx_material_manager::load_material_textures(
    const gfx_material_definition *material,
    resource_eviction_policy evictionPolicy)
{
    // Thread-safe less-than interlocked
    auto frame = the_gfx_renderer->frame_counter(); // TODO: get rid of this counter, use global stats
    auto lastUsage = m_lastUsageTimes[material->id];
    if(lastUsage < frame)
    {
        if(InterlockedCompareExchange(&m_lastUsageTimes[material->id], frame, lastUsage) == lastUsage)
        {
            auto shaderID = material->shaderID;
            
            // Lookup the actual material
            auto &shaderInfo = go::gfx_material_shader_info[(uint32_t)shaderID];
            auto &matInfo = go::gfx_material_info[(uint32_t)shaderInfo.type];

            // Calculate the flattened fallback handle by going
            // through each material type and adding the number of textures together
            // NOTE: this could be pre-calculated
            auto fallbackHandleBase = 0;
            for(auto i = 0U; i < (uint32_t)shaderInfo.type; ++i)
            {
                fallbackHandleBase += go::gfx_material_info[i].usedTextureCount;
            }

            for(auto &id : material->textureIDs)
            {
                m_textureMgr.load_async(id, fallbackHandleBase, evictionPolicy);
                ++fallbackHandleBase;
            }
        }
    }
}