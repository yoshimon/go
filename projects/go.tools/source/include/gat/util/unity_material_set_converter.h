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
#include "gat/util/unity_material_set.h"
#include "gat/files/config_file.h"
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

//! A class, which provides a simple interface for converting Unity material sets.
class unity_material_set_converter
{
public:
    /*!
        Converts the texture names bound to a material.

        Note: this will not save the textures. This will only rename the textures to match a folder structure.

        \param dstMat The material to convert the texture names in.
     */
    static void convert_material_texture_names(gat::material_file &dstMat);
    /*!
        Converts the textures bound to a material.

        Note: this function will not convert the texture names prior to conversion.

        \param cfg The configuration file.
        \param srcMat The source material to convert the textures from.
        \param dstMat The destination material to convert the textures to.
        \param assetDirPath The prefix to append to the source texture path.
        \param outAssetDirPath The prefix to append to the destination texture path.
        \param metaGenerators The meta generator set.
     */
    static void convert_material_textures(const config_file &cfg, const gat::material_file &srcMat,
        const gat::material_file &dstMat, const go::file_path &assetDirPath, const go::file_path &outAssetDirPath,
        asset_meta_generators &metaGenerators);
public:
    /*!
        Constructor.

        \param materialSet The material set to convert.
     */
    unity_material_set_converter(const unity_material_set &materialSet);
public:
    /*!
        Saves the converted assets to disk.

        \param cfg The application configuration.
        \param assetDirPath The asset directory path.
        \param outAssetDirPath The output material file path.
        \param metaGenerators The meta generator list.
     */
    void save(const config_file &cfg,
        const go::file_path &assetDirPath,
        const go::file_path &outAssetDirPath,
        asset_meta_generators &metaGenerators);
private:
    //! The material set to convert.
    unity_material_set m_matSet;
};

GAT_END_NAMESPACE