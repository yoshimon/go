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
#include "gat/util/asset_db_types.h"
#include "gat/files/gmd_file.h"
#include "gat/util/unity_model.h"
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

//! A class, which holds information about additional meta data.
struct model_meta_data_file
{
    //! Constructor.
    model_meta_data_file() = default;
    //! \overload
    model_meta_data_file(const go::file_path &filePath);
    //! \overload
    model_meta_data_file(
		const unity_model &inModel,
		const gmd_file &gmdFile,
        const go::file_path &gmdFilePath);
    //! The material names that are associated with the model.
    std::vector<asset_db_name> materials;
    //! The static bounding box of the model.
    DirectX::XMFLOAT3 staticAABB[2];
    //! The name of the animation collection referenced by the model.
    std::string animationCollection;
    //! The bounding boxes for each animation in the referenced collection.
    std::vector<DirectX::XMFLOAT3[2]> animationAABBs;
    //! The GMD file path.
    std::string gmdFilePath;
    /*!
        Loads the file from disk.

        \param filePath The file path.
     */
    void load(const go::file_path &filePath);
    /*!
        Saves the file to disk.

        \param filePath The file path.
     */
    void save();
private:
    //! \overload
    model_meta_data_file(const gmd_file &gmdFile, const go::file_path &gmdFilePath);
};

GAT_END_NAMESPACE