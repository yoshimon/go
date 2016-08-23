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

#include "gat/util/material_db.h"


#include "gat/files/material_file.h"


//===========================================================================
// gat::material_db
//===========================================================================

gat::material_db::material_db()
    : asset_db("materialdb", "materials", "material")
{
}


gat::material_db::material_db(const go::file_path &filePath)
    : material_db()
{
    load(filePath);
}


void gat::material_db::import(const asset_meta_file &metaFile, const std::string &name,
    const go::file_path &group)
{
    // Load the material file
    material_file mtlFile(metaFile.fullAssetPath);
    auto nMaterial = add_record(mtlFile.name, group);

    auto &&shaderInfo = go::gfx_material_shader_info[(uint32_t)mtlFile.shaderID];
    auto &&matInfo = go::gfx_material_info[(uint32_t)shaderInfo.type];

    go::xml_require_attribute(nMaterial, "shader", shaderInfo.name);
    go::xml_require_attribute(nMaterial, "blendmode", go::gfx_blend_mode_strings[(uint32_t)mtlFile.blendMode]);
    go::xml_require_bool_attribute(nMaterial, "isTwoSided", mtlFile.isTwoSided);

    // Textures
    auto &&nTextures = go::xml_require_child(nMaterial, "textures");
    auto &&textureSlot = 0;
    for(auto &&tex : mtlFile.textures)
    {
        if(!tex.is_empty())
        {
            auto &&nTex = go::xml_add_child(nTextures, "texture", tex.c_str());
            go::xml_require_attribute(nTex, "semantic", matInfo.textureSemantics[textureSlot]);
        }

        ++textureSlot;
    }

    // Constants
    auto &&nConstants = go::xml_require_child(nMaterial, "constants");
	for(auto &&c : mtlFile.constants)
	{
		go::xml_add_child(nConstants, c.first.c_str(), c.second.c_str());
	}
}