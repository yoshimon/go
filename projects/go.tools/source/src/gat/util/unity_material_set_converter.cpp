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

#include "gat/util/unity_material_set_converter.h"


#include "gat/util/converters.h"


//===========================================================================
// gat::unity_material_set_converter
//===========================================================================

gat::unity_material_set_converter::unity_material_set_converter(const unity_material_set &materialSet)
    : m_matSet(materialSet)
{
}


void gat::unity_material_set_converter::save(const config_file &cfg,
    const go::file_path &assetDirPath, const go::file_path &outAssetDirPath,
    asset_meta_generators &metaGenerators)
{
    saved_data savedData;

    // Unify texture formats to least-conflicting format
    {
        std::unordered_map<std::string, std::string> semanticCache;
        semanticCache.reserve(m_matSet.entries().size());
        for(auto &entry : m_matSet.entries())
        {
            size_t i = 0;
            for(auto &tex : entry.material.textures)
            {
                if(tex.is_empty())
                {
                    continue;
                }

                auto &texFormat = entry.material.textureFormats[i];

                // Lookup the current semantic
                auto pib = semanticCache.insert(std::make_pair(tex, texFormat));
                if(!pib.second)
                {
                    // Find least-conflicting semantic
                    auto &oldFormat = pib.first->second;
                    if(oldFormat != texFormat)
                    {
                        if(oldFormat == "BC1_UNORM" && texFormat == "BC3_UNORM_SRGB")
                        {
                            // Always prefer with-alpha formats over color-only if conflicting
                            semanticCache[tex] = texFormat;

                            // TODO: handle skipIfExisting
                        }
                        else if(oldFormat != "BC5_UNORM" && texFormat == "BC5_UNORM")
                        {
                            // Normals always have precedence over other formats
                            semanticCache[tex] = texFormat;
                        }
                    }
                }

                ++i;
            }
        }
    }

    // Convert materials and textures
    for(auto &entry : m_matSet.entries())
    {
        // Copy the material
        auto dstMat = entry.material;

        // Convert the textures in the material
        convert_material_texture_names(dstMat);
        convert_material_textures(cfg, entry.material, dstMat, assetDirPath, outAssetDirPath, metaGenerators);

        // Generate the filename based on the material name
        auto mtlFilePath = outAssetDirPath + dstMat.name;
        mtlFilePath.replace_extension(".mtl");

        // Save it out
        dstMat.save(mtlFilePath);

        // Register saved data
        savedData.emplace_back(asset_type::material, mtlFilePath);
    }

    metaGenerators.push_back(savedData);
}


void gat::unity_material_set_converter::convert_material_texture_names(gat::material_file &dstMat)
{
    for(auto &tex : dstMat.textures)
    {
        if(!tex.is_empty())
        {
            // Change the file extension
            tex.replace_extension(_T(".dds"));
        }
    }
}


void gat::unity_material_set_converter::convert_material_textures(const config_file &cfg,
    const gat::material_file &srcMat, const gat::material_file &dstMat,
    const go::file_path &assetDirPath, const go::file_path &outAssetDirPath,
    asset_meta_generators &metaGenerators)
{
    texture_converter_parameters params;
    params.in.skipIfExisting = false;

    for(size_t i = 0; i < _countof(srcMat.textures); ++i)
    {
        auto &srcTex = srcMat.textures[i];
        if(!srcTex.is_empty())
        {
            params.in.texFilePath = assetDirPath + srcTex;
            
            params.out.format = dstMat.textureFormats[i];
            params.out.texFilePath = outAssetDirPath + dstMat.textures[i];

            converters::convert_texture(cfg, params, metaGenerators);
        }
    }
}