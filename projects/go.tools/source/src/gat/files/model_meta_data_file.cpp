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

#include "gat/files/model_meta_data_file.h"


#include "gat/util/exception.h"
#include "gat/util/file_util.h"
#include "gat/util/rapidxml_helper.h"
#include "gat/util/bounding_volume.h"


using namespace DirectX;


//===========================================================================
// gat::model_meta_data_file
//===========================================================================

gat::model_meta_data_file::model_meta_data_file(const go::file_path &filePath)
{
    load(filePath);
}


gat::model_meta_data_file::model_meta_data_file(const gmd_file &gmdFile,
    const go::file_path &gmdFilePath)
{
    this->gmdFilePath = gmdFilePath;

    // Calculate the default bounding box
    auto vAABBMin = XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, 0.f);
    auto vAABBMax = XMVectorSet(-FLT_MAX, -FLT_MAX, -FLT_MAX, 0.f);
    calculate_vertex_aabb(gmdFile.unskinnedVertices, vAABBMin, vAABBMax);
    calculate_vertex_aabb(gmdFile.skinnedVertices, vAABBMin, vAABBMax);

    DirectX::XMStoreFloat3(&staticAABB[0], vAABBMin);
    DirectX::XMStoreFloat3(&staticAABB[1], vAABBMax);
}


gat::model_meta_data_file::model_meta_data_file(const unity_model &inModel,
    const gmd_file &gmdFile, const go::file_path &gmdFilePath)
    : model_meta_data_file(gmdFile, gmdFilePath)
{
    // Convert the material names
    for(auto &matName : inModel.material_names())
    {
        materials.push_back(matName);
    }

    // TODO: animation collection
    // TODO: dynamic bounding boxes
    // NOTE: Change animation collection to single, instead of multiple files
}


void gat::model_meta_data_file::load(const go::file_path &filePath)
{
    rapidxml::xml_document<> doc;
    try
    {
        auto txt = file_util::read_text_file(filePath);
        doc.parse<0>((char *)txt.c_str());

        auto nRoot = doc.first_node("mdlmeta");
        throw_if_not<tchar_exception>(nRoot, "Invalid model meta data file.");

        // Static AABB
        auto nStaticAABB = xml_require_child(nRoot, "staticAABB");
        {
            auto aMin = xml_require_attribute(nStaticAABB, "min");
            auto aMax = xml_require_attribute(nStaticAABB, "max");

            staticAABB[0] = go::string_to_vector3f<XMFLOAT3>(aMin->value());
            staticAABB[1] = go::string_to_vector3f<XMFLOAT3>(aMax->value());
        }

        // Materials
        xml_opt_child_set_values(nRoot, "materials", "material", materials);

        // Animation collection
        xml_opt_child_node_value(nRoot, "animationCollection", animationCollection);

        // Save it out
        go::xml_save_document(doc, filePath);
    }
    catch(...)
    {
        throw tchar_exception(_T("Could not load model meta data file."));
    }
}


void gat::model_meta_data_file::save()
{
    go::file_path filePath(gmdFilePath);
    filePath.replace_extension(".mdlmeta");

    rapidxml::xml_document<> doc;
    try
    {
        auto nRoot = doc.allocate_node(rapidxml::node_element, "mdlmeta");
        doc.append_node(nRoot);

        // Static AABB
        auto nStaticAABB = go::xml_add_child(nRoot, "staticAABB");
        {
            auto aMin = go::xml_require_attribute(nStaticAABB, "min", go::vector3_to_string(staticAABB[0]).c_str());
            auto aMax = go::xml_require_attribute(nStaticAABB, "max", go::vector3_to_string(staticAABB[1]).c_str());
        }

        // Materials
        xml_add_opt_child_set_values(nRoot, "materials", "material", materials);

        // Animation collection
        if(!animationCollection.empty())
        {
            go::xml_add_child(nRoot, "animationCollection", animationCollection.c_str());
        }

        // Save it out
        go::xml_save_document(doc, filePath);
    }
    catch(...)
    {
        throw tchar_exception(_T("Could not save a model meta data file."));
    }
}