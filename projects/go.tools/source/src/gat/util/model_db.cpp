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

#include "gat/util/model_db.h"


#include "gat/files/model_meta_data_file.h"


//===========================================================================
// gat::model_db
//===========================================================================

gat::model_db::model_db(const go::file_path &outDataPath)
    : asset_datapath_db("modeldb", "models", "model", outDataPath)
{
}


gat::model_db::model_db(const go::file_path &filePath, const go::file_path &outDataPath)
    : model_db(outDataPath)
{
    load(filePath);
}


void gat::model_db::import(const asset_meta_file &metaFile, const std::string &name, const go::file_path &group)
{
    // Model meta data file path
    auto mdlMetaFilePath = metaFile.fullAssetPath;
    mdlMetaFilePath.replace_extension(".mdlmeta");

    // Load model meta data file
    model_meta_data_file mdlMetaFile(mdlMetaFilePath);

    // Register new record
    auto nModel = add_record(name, group);

    // Materials
    auto nMaterials = go::xml_add_child(nModel, "materials");
    if(mdlMetaFile.materials.size() == 1)
    {
        // Single material
        go::xml_require_attribute(nMaterials, "uniform", mdlMetaFile.materials.front().c_str());
    }
    else
    {
        // Material list
        for(auto &mat : mdlMetaFile.materials)
        {
            go::xml_add_child(nMaterials, "material", mat.c_str());
        }
    }

    // Animations
    if(!mdlMetaFile.animationCollection.empty())
    {
        go::xml_add_child(nModel, "animations", mdlMetaFile.animationCollection.c_str());
    }

    // Bounding volumes
    auto nBV = go::xml_add_child(nModel, "bounding_volume");

    // Static bounding box
    auto nStaticBV = go::xml_add_child(nBV, "static");

    auto nAABB = go::xml_add_child(nStaticBV, "bounding_box");
    auto minAABBStr = go::vector3_to_string(mdlMetaFile.staticAABB[0]);
    auto maxAABBStr = go::vector3_to_string(mdlMetaFile.staticAABB[1]);
    go::xml_require_attribute(nAABB, "min", minAABBStr.c_str());
    go::xml_require_attribute(nAABB, "max", maxAABBStr.c_str());

    // Animations (bounding box)
    if(!mdlMetaFile.animationCollection.empty())
    {
        auto nDynamicBV = go::xml_add_child(nBV, "dynamic");
    
        // TODO: add bounding box for animations
    }

    // Copy the GMD resource
    copy_asset(metaFile.fullAssetPath, name);
}