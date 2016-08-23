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

#include "gat/files/asset_meta_file.h"


#include "gat/util/exception.h"
#include "gat/util/file_util.h"
#include "gat/util/bounding_volume.h"
#include "gat/files/gmd_file.h"
#include "gat/util/rapidxml_helper.h"


//===========================================================================
// Meta generators
//===========================================================================

static void model_meta_generator_proc(gat::asset_meta_file &metaFile, const std::vector<uint8_t> &)
{
    metaFile.assetType = gat::asset_type::model;
}


static void texture_meta_generator_proc(gat::asset_meta_file &metaFile, const std::vector<uint8_t> &)
{
    metaFile.assetType = gat::asset_type::texture;
}


static void material_meta_generator_proc(gat::asset_meta_file &metaFile, const std::vector<uint8_t> &)
{
    metaFile.assetType = gat::asset_type::material;
}


static void animation_meta_generator_proc(gat::asset_meta_file &metaFile, const std::vector<uint8_t> &)
{
    metaFile.assetType = gat::asset_type::animation;
}


// The order is important here
static gat::asset_meta_generator_proc asset_meta_generator_proc_table[] =
{
    model_meta_generator_proc,
    material_meta_generator_proc,
    texture_meta_generator_proc,
    animation_meta_generator_proc,
};


//===========================================================================
// gat::asset_meta_file
//===========================================================================

void gat::asset_meta_file::load(const go::file_path &filePath)
{
    rapidxml::xml_document<> doc;
    try
    {
        auto txt = file_util::read_text_file(filePath);
        doc.parse<0>((char *)txt.c_str());

        auto nRoot = doc.first_node("meta");
        if(!nRoot)
        {
            throw tchar_exception(_T("Invalid asset meta file."));
        }

        auto nAssetType = xml_require_child(nRoot, "assetType");
        auto nFileName = xml_require_child(nRoot, "fileName");
        auto nFileSHA256 = xml_require_child(nRoot, "fileSHA256");
        auto nFileSize = xml_require_child(nRoot, "fileSize");

        this->assetType = asset_type_enum(nAssetType->value());
        this->fileName = nFileName->value();
        this->fileSHA256 = nFileSHA256->value();
        this->fileSize = std::stoll(nFileSize->value());

        fullAssetPath = filePath.directory_path() + fileName;
    }
    catch(...)
    {
        throw tchar_exception(_T("Could not load meta data file."));
    }
}


void gat::asset_meta_file::save(const go::file_path &filePath)
{
    rapidxml::xml_document<> doc;
    try
    {
        auto nRoot = doc.allocate_node(rapidxml::node_element, "meta");
        doc.append_node(nRoot);
        
        go::xml_add_child(nRoot, "assetType", asset_type_string[(int32_t)assetType]);
        go::xml_add_child(nRoot, "fileName", fileName.c_str());
        go::xml_add_child(nRoot, "fileSHA256", fileSHA256.c_str());
        go::xml_add_child(nRoot, "fileSize", std::to_string(fileSize).c_str());

        go::xml_save_document(doc, filePath);

        fullAssetPath = filePath.directory_path() + fileName;
    }
    catch(...)
    {
        throw tchar_exception(_T("Could not save file meta data."));
    }
}


//===========================================================================
// gat::asset_meta_generators
//===========================================================================

void gat::asset_meta_generators::push_back(const gat::saved_data &savedData)
{
    for(auto &entry : savedData)
    {
        // Push a new generator in
        m_generators.emplace_back(entry, asset_meta_generator_proc_table[(int32_t)entry.assetType]);
    }
}


void gat::asset_meta_generators::generate_and_save()
{
    _tprintf(_T("Generating meta data...\n"));

    // Generate meta data for the converted asset
    for(auto &metaDataPair : m_generators)
    {
        // Grab the file path and the generator
        auto &metaDataFilePath = metaDataPair.first.filePath;
        auto &metaDataGen = metaDataPair.second;

        // Load the asset into memory for hashing
        auto assetFile = file_util::read_binary_file(metaDataFilePath);

        // Fill out some basic meta data
        asset_meta_file f;
        f.fileName = metaDataFilePath.file_name();
        f.fileSize = assetFile.size();
        f.fileSHA256 = file_util::compute_sha256_hash(assetFile.data(), assetFile.size());

        // Fill out the rest of the data
        metaDataGen(f, assetFile);
        
        // And save it to the same directory
        gat::tstring newExt = metaDataFilePath.file_extension();
        newExt += _T(".meta");

        go::file_path outFilePath(metaDataFilePath);
        outFilePath.replace_extension(newExt.c_str());
        f.save(outFilePath.c_str());
    }
}