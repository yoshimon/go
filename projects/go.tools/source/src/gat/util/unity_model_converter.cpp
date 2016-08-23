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

#include "gat/util/unity_model_converter.h"


#include "gat/files/model_meta_data_file.h"


//===========================================================================
// gat::unity_scene_converter
//===========================================================================

gat::unity_model_converter::unity_model_converter(const unity_model &model)
    : m_model(model)
{
}


void gat::unity_model_converter::save(const config_file &cfg, const go::file_path &modelFilePath,
    asset_meta_generators &metaGenerators)
{
    gat::saved_data savedData;

    auto &model = m_model;

    // Simply copy the data into the GMD file
    gat::gmd_file gmdFile;
    gmdFile.unskinnedVertices = model.unskinned_vertices();
    gmdFile.unskinnedMeshes = model.unskinned_meshes();
    gmdFile.skinnedVertices = model.skinned_vertices();
    gmdFile.skinnedMeshes = model.skinned_meshes();
    gmdFile.indices = model.indices();

    // TODO: handle skinned models
    // gmdFile.bones.reserve(256);

    // Convert handedness
    for(auto &v : gmdFile.unskinnedVertices)
    {
        v.uv.y = 1.f - v.uv.y;
    }

    gmdFile.save(modelFilePath);
    savedData.emplace_back(asset_type::model, modelFilePath);

    // Save additional meta-data for this model
    model_meta_data_file metaDataFile(model, gmdFile, modelFilePath);
    metaDataFile.save();

    metaGenerators.push_back(savedData);
}