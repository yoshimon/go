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
#include "gat/util/asset_type.h"
#include "gat/util/saved_data.h"
#include "gat/util/asset_db_types.h"
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

//! A class, which provides an interface for asset meta files.
struct asset_meta_file
{
    //! The type of the asset.
    asset_type assetType;
    //! The file name.
    go::file_path fileName;
    //! The size of the asset file.
    uint64_t fileSize;
    //! The file SHA256 hash value.
    std::string fileSHA256;
    //! Contains the full asset path.
    go::file_path fullAssetPath;
    //! Constructor.
    asset_meta_file() = default;
    //! \overload
    asset_meta_file(const go::file_path &filePath) { load(filePath); }
    /*!
        Loads the file from disk.

        \param filePath The file path.
     */
    void load(const go::file_path &filePath);
    /*!
        Saves the file to disk.

        \param filePath The file path.
     */
    void save(const go::file_path &filePath);
};


typedef std::function<void(gat::asset_meta_file &, const std::vector<uint8_t> &)> asset_meta_generator_proc;


//! A structure, which represents a set of meta generators.
class asset_meta_generators
{
public:
    /*!
        Creates a new meta generator.

        \param savedData The saved data entries to add.
     */
    void push_back(const gat::saved_data &savedData);
    //! Generates the meta data and saves it to disk.
    void generate_and_save();
private:
    //! The meta generators.
    std::vector<std::pair<gat::saved_data_entry, asset_meta_generator_proc>> m_generators;
};

GAT_END_NAMESPACE