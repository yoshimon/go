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
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

//! A class, which represents a section file.
class unity_meta_file
{
public:
    //! A structure, which represents a section in the file.
    struct entry_t
    {
        //! The name of the entry.
        std::string name;
        //! The value of the entry.
        std::string value;
        //! The index of the parent entry.
        int32_t parentEntryIndex;
        //! The child entry indices.
        std::vector<size_t> childIndices;
    };
    typedef std::vector<entry_t> entries_t;
public:
    //! Constructor.
    unity_meta_file() = default;
    //! \overload
    unity_meta_file(const go::file_path &filePath);
public:
    /*!
        Loads the section file from a file on disk.

        \param filePath The file path.
     */
    void load(const go::file_path &filePath);
    /*!
        Returns the flattened sections list.

        \return The flattened sections array.
     */
    const entries_t &entries() const noexcept;
    /*!
        Returns the asset path.

        \return The asset path.
     */
    go::file_path asset_path() const noexcept;
private:
    /*!
        Loads a v2 file.

        \param text The text file content.
     */
    void load_meta_file_v2(std::stringstream &text);
private:
    //! The flattened section array.
    entries_t m_entries;
    //! The asset path.
    go::file_path m_assetPath;
};

GAT_END_NAMESPACE