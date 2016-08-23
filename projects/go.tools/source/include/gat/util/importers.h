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

#pragma once

#include "gat/tools/tool_base.h"
#include "gat/files/asset_meta_file.h"


GAT_BEGIN_NAMESPACE

/*!
    A structure, which represents the parameters for the asset importer.
 */
struct asset_import_parameters
{
    struct
    {
        /*!
            The meta file path.
         */
        go::file_path metaFilePath;
    } in;
    struct
    {
        /*!
            The group tag.
         */
        go::file_path group;
        /*!
            The database name of the asset.
         */
        std::string name;
        /*!
            The assets path.
         */
        go::file_path assetsDirPath;
    } out;
};


/*!
    A structure, which represents the parameters for the scene importer.
 */
struct scene_assets_import_parameters
{
    struct
    {
        /*!
            The scene file path.
         */
        go::file_path sceneFilePath;
        /*!
            The name of the scene to rename.

            This is equal to the name of the group to delete.
         */
        go::file_path sceneRename;
        /*!
            Replaces the declarations of a previous import of this scene.
         */
        bool replaceExisting;
    } in;
    struct
    {
        /*!
            The scene file path.
         */
        go::file_path sceneFilePath;
        /*!
            The assets directory path.
         */
        go::file_path assetsDirPath;
    } out;
};


/*!
    A structure, containing functions for importing assets.
 */
struct importers
{
    /*!
        Imports an asset into the engine.

        \param cfg The configuration file.
        \param parameters The importers parameters.
     */
    static void import_asset(const config_file &cfg, const asset_import_parameters &parameters);
    /*!
        \overload
     */
    static void import_asset(int argc, const _TCHAR *argv[], const config_file &cfg);
    /*!
        Imports all assets that belong to a scene into the engine.

        \param cfg The configuration file.
        \param parameters The import parameters.
     */
    static void import_scene_assets(const config_file &cfg, const scene_assets_import_parameters &parameters);
    /*!
        \overload
     */
    static void import_scene_assets(int argc, const _TCHAR *argv[], const config_file &cfg);
};

GAT_END_NAMESPACE