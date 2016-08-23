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
#include "gat/files/material_file.h"


GAT_BEGIN_NAMESPACE

//! A class, which provides an interface for a Unity material set.
class unity_material_set
{
public:
    //! An entry in the material set.
    struct entry_t
    {
        //! The material.
        material_file material;
    };
    typedef std::vector<entry_t> entry_vector_t;
public:
    //! Constructor.
    unity_material_set() = default;
    //! \overload
    unity_material_set(const go::file_path &filePath);
public:
    /*!
        Resets the material set.

        \param filePath The file path.
     */
    void reset(const go::file_path &filePath);
    /*!
        Returns the material vector.

        \return The material vector.
     */
    entry_vector_t &entries() noexcept { return m_entries; }
    //! \overload
    const entry_vector_t &entries() const noexcept { return m_entries; }
private:
    //! The list of materials.
    entry_vector_t m_entries;
};

GAT_END_NAMESPACE