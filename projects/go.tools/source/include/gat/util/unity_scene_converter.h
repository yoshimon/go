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

#include "gat/defines.h"
#include "gat/files/asset_meta_file.h"
#include "gat/util/unity_scene.h"
#include "gat/files/config_file.h"
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

/*!
    A class, which provides a simple interface for converting models with Assimp.
 */
class unity_scene_converter
{
public:
    /*!
        Constructor.

        \param scene The scene to convert.
     */
    unity_scene_converter(const unity_scene &scene);
public:
    /*!
        Saves the converted assets to disk.

        \param cfg The application configuration.
        \param sceneFilePath The scene file path.
        \param materialSetFilePath The material set file path.
        \param outAssetDirPath The output asset directory path.
        \param metaGenerators The meta generator list.
     */
    void save(const config_file &cfg,
        const go::file_path &sceneFilePath,
        const go::file_path &materialSetFilePath,
        const go::file_path &outAssetDirPath,
        asset_meta_generators &metaGenerators);
private:
    /*!
        The scene to convert.
     */
    const unity_scene &m_scene;
};

GAT_END_NAMESPACE