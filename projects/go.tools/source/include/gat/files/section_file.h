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
class section_file
{
public:
    //! A structure, which represents a section in the file.
    struct section_t
    {
        //! Identifies the current section as a list section.
        bool isList;
        //! The flattened section index.
        size_t index;
        //! The name of the section.
        std::string name;
        //! The index of the parent section.
        int32_t parentSectionIndex;
        //! The child section indices.
        std::vector<size_t> childIndices;
        /*!
            The property mappings.

            Only valid for regular sections.
         */
        std::map<std::string, std::string> mappings;
        /*!
            The list content.

            Only valid for list sections.
         */
        std::vector<std::string> listContent;
        /*!
            Returns the value of an optional mapping.

            \param key The mapping key.
            \param defaultVal The default value.

            \return The mapped value or the default value.
         */
        const std::string &opt_mapping(const std::string &key, const std::string &defaultVal = "") const
        {
            auto it = mappings.find(key);
            return it == mappings.end() ? defaultVal : it->second;
        }
    };
    typedef std::vector<section_t> sections_t;
public:
    //! Constructor.
    section_file() = default;
    //! \overload
    section_file(const go::file_path &filePath);
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
    const sections_t &sections() const noexcept;
    /*!
        Finds a named section.

        \param name The section name.

        \return The section pointer or nullptr if no matching section was found.
     */
    section_t *find_section(const std::string &name) noexcept;
    /*!
        Returns the next sibling of a section.

        \param[in,out] section The current section and the next sibling.
     */
    void next_sibling(const section_t **section) noexcept;
private:
    /*!
        Loads a v0.9 section file.

        \param text The text file content.
     */
    void load_section_file_v09(std::stringstream &text);
private:
    //! The flattened section array.
    sections_t m_sections;
};

GAT_END_NAMESPACE