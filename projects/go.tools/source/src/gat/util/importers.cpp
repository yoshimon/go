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

#include "gat/util/importers.h"


#include "gat/util/cmd_help.h"
#include "gat/util/model_db.h"
#include "gat/util/material_db.h"
#include "gat/util/texture_db.h"
#include "gat/util/file_util.h"
#include "gat/util/exception.h"
#include <filesystem>


namespace fs = std::experimental::filesystem;


//===========================================================================
// gat::importers
//===========================================================================

void gat::importers::import_asset(int argc, const _TCHAR *argv[], const config_file &cfg)
{
    asset_import_parameters params;

    params.in.metaFilePath = get_parameter_value(argc, argv, _T("-imeta"), _T(""));
    params.out.group = get_parameter_value(argc, argv, "-ogrp", "");
    params.out.name = get_parameter_value(argc, argv, "-oname", "");
    params.out.assetsDirPath = get_parameter_value(argc, argv, "-oassets", "");

    import_asset(cfg, params);
}


void gat::importers::import_asset(const config_file &cfg, const asset_import_parameters &parameters)
{
    throw_if_not<tchar_exception>(!parameters.out.assetsDirPath.is_empty(), _T("Invalid assets path."));

    // The base world directory
    go::file_path worldDir = cfg.query(config_prop_worlddir);

    // The database folders
    auto mdlFolder = cfg.query(config_prop_mdlfolder);
    auto texFolder = cfg.query(config_prop_texfolder);
    auto anmFolder = cfg.query(config_prop_anmfolder);

    auto mdlDB = cfg.query(config_prop_mdldb);
    auto mtlDB = cfg.query(config_prop_mtldb);
    auto texDB = cfg.query(config_prop_texdb);
    auto anmDB = cfg.query(config_prop_anmdb);

    // Database files
    auto inMdlDB = worldDir + mdlDB;
    auto outMdlDB = parameters.out.assetsDirPath + mdlDB;
    auto mdlDBData = parameters.out.assetsDirPath + mdlFolder;

    auto inMtlDB = worldDir + mtlDB;
    auto outMtlDB = parameters.out.assetsDirPath + mtlDB;

    auto inTexDB = worldDir + texDB;
    auto outTexDB = parameters.out.assetsDirPath + texDB;
    auto texDBData = parameters.out.assetsDirPath + texFolder;
    
    auto inAnmDB = worldDir + anmDB;
    auto outAnmDB = parameters.out.assetsDirPath + anmDB;
    auto anmDBData = parameters.out.assetsDirPath + anmFolder;

    // Load the meta file
    asset_meta_file f(parameters.in.metaFilePath);

    // Load the DB
    switch(f.assetType)
    {
    case asset_type::model:
    {
        model_db db(inMdlDB, mdlDBData);
        db.import(f, parameters.out.name, parameters.out.group);
        db.save(outMdlDB);
        break;
    }
    case asset_type::material:
    {
        material_db db(inMtlDB);
        db.import(f, parameters.out.name, parameters.out.group);
        db.save(outMtlDB);
        break;
    }
    case asset_type::texture:
    {
        texture_db db(inTexDB, texDBData);
        db.import(f, parameters.out.name, parameters.out.group);
        db.save(outTexDB);
        break;
    }
    case asset_type::animation:
    {
        break;
    }
    }
}


void gat::importers::import_scene_assets(int argc, const _TCHAR *argv[], const config_file &cfg)
{
    scene_assets_import_parameters params;

    params.in.sceneFilePath = get_parameter_value(argc, argv, _T("-iscn"), _T(""));
    params.in.sceneRename = get_parameter_value(argc, argv, _T("-rename"), _T(""));
    params.in.replaceExisting = !params.in.sceneRename.is_empty() || is_parameter_set(argc, argv, _T("-replace"));
    params.out.sceneFilePath = get_parameter_value(argc, argv, _T("-oscn"), _T(""));
    params.out.assetsDirPath = get_parameter_value(argc, argv, _T("-oassets"), _T(""));

    import_scene_assets(cfg, params);
}


void gat::importers::import_scene_assets(const config_file &cfg, const scene_assets_import_parameters &parameters)
{
    asset_import_parameters params;
    params.out.group = parameters.out.sceneFilePath.file_name();
    params.out.group.replace_extension("");
    params.out.assetsDirPath = parameters.out.assetsDirPath;

    // The base world directory
    go::file_path worldDir = cfg.query(config_prop_worlddir);

    // Database files
    auto inMdlDB = worldDir + cfg.query(config_prop_mdldb);
    auto outMdlDB = parameters.out.assetsDirPath + inMdlDB;

    auto inMtlDB = worldDir + cfg.query(config_prop_mtldb);
    auto outMtlDB = parameters.out.assetsDirPath + inMtlDB;
    
    auto inTexDB = worldDir + cfg.query(config_prop_texdb);
    auto outTexDB = parameters.out.assetsDirPath + inTexDB;
    
    auto inAnmDB = worldDir + cfg.query(config_prop_anmdb);
    auto outAnmDB = parameters.out.assetsDirPath + inAnmDB;

    // Open the databases
    model_db mdlDBInst(inMdlDB, worldDir);
    material_db mtlDBInst(inMtlDB);
    texture_db texDBInst(inTexDB, worldDir);

    if(parameters.in.replaceExisting)
    {
        auto oldName = parameters.in.sceneRename.is_empty() ? params.out.group : parameters.in.sceneRename;
        mdlDBInst.remove_group(oldName);
        mtlDBInst.remove_group(oldName);
        texDBInst.remove_group(oldName);
    }

    // Load all *.meta files from the subdirectories
    auto assetDirPath = parameters.in.sceneFilePath.directory_path();
    for(auto &p : fs::recursive_directory_iterator(assetDirPath.c_str()))
    {
        if(fs::is_regular_file(p))
        {
            params.in.metaFilePath = p.path().generic_string();
            if(!tstrcmp(params.in.metaFilePath.file_extension(), ".meta"))
            {
                // Load the meta file
                asset_meta_file f(params.in.metaFilePath);
                
                // Figure out the output asset name
                std::string fullAssetPath = f.fullAssetPath;
                fullAssetPath.erase(0, assetDirPath.length() + 1);
                params.out.name = fullAssetPath;

                // Load the DB
                switch(f.assetType)
                {
                case asset_type::model: mdlDBInst.import(f, params.out.name, params.out.group); break;
                case asset_type::material: mtlDBInst.import(f, params.out.name, params.out.group); break;
                case asset_type::texture: texDBInst.import(f, params.out.name, params.out.group); break;
                case asset_type::animation: break;
                }
            }
        }
    }

    // Copy the scene file
    fs::create_directories(parameters.out.sceneFilePath.directory_path().c_str());
    fs::copy_file(parameters.in.sceneFilePath.c_str(), parameters.out.sceneFilePath.c_str(), fs::copy_options::overwrite_existing);

    mdlDBInst.save(outMdlDB);
    mtlDBInst.save(outMtlDB);
    texDBInst.save(outTexDB);
}