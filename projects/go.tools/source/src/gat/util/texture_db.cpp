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

#include "gat/util/texture_db.h"


#include <filesystem>


namespace fs = std::experimental::filesystem;


//===========================================================================
// gat::texture_db
//===========================================================================

gat::texture_db::texture_db(const go::file_path &outDataPath)
    : asset_datapath_db("texturedb", "textures", "texture", outDataPath)
{
}


gat::texture_db::texture_db(const go::file_path &filePath, const go::file_path &outDataPath)
    : texture_db(outDataPath)
{
    load(filePath);
}


void gat::texture_db::import(const asset_meta_file &metaFile, const std::string &name, const go::file_path &group)
{
    add_record(name, group);
    copy_asset(metaFile.fullAssetPath, name);
}