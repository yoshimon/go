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

#include "gat/files/unity_meta_file.h"


#include "gat/util/file_util.h"
#include "gat/util/converters.h"
#include "gat/util/exception.h"


//===========================================================================
// gat::unity_meta_file
//===========================================================================

gat::unity_meta_file::unity_meta_file(const go::file_path &filePath)
{
    load(filePath);
}


void gat::unity_meta_file::load(const go::file_path &filePath)
{
    m_entries.clear();
    
    // Build asset path by dropping the extension
    m_assetPath = filePath;
    m_assetPath.replace_extension("");

    // Read in the section file
    std::stringstream txt(gat::file_util::read_text_file(filePath));

    // The first line contains the version
    std::string line;
    if(std::getline(txt, line))
    {
        if(line == "fileFormatVersion: 2")
        {
            load_meta_file_v2(txt);
        }
        else
        {
            throw tchar_exception(_T("Unsupported Unity file format version (expected v2)."));
        }
    }
}


void gat::unity_meta_file::load_meta_file_v2(std::stringstream &text)
{
    // State-tracking variables for current indentation block
    const int kEntryIndent = 2; // Two spaces = next sub-entry
    int currentIndent = 0; // No spaces = no indentation

    std::string line;
    int32_t previousEntryIndex = -1;
    entry_t *currentEntry = nullptr;
    int sectionDepth = 0;

    m_entries.reserve(1024 * 10);

    // Helper function for counting leading spaces
    auto countSpaces = [](const std::string &s)
    {
        auto i = s.find_first_not_of(' ');
        return i == s.npos ? s.length() : i;
    };

    while(std::getline(text, line))
    {
        auto indent = countSpaces(line);

        // Indentation has to be aligned
        if(indent % kEntryIndent != 0)
        {
            throw tchar_exception(_T("Invalid indentation."));
        }

        // How has the indentation changed?
        auto deltaIndent = indent - currentIndent;
        /*
        if(deltaIndent == kEntryIndent)
        {
            // New sub-entry
            ++sectionDepth;

            // Allocate new section
            m_entries.emplace_back();
            currentSection = &m_entries.back();

            currentSection->name = line.substr(kToken_Section.length() + 1, std::string::npos);
            currentSection->parentSectionIndex = previousSectionIndex;
        }
        else if(deltaIndent < kEntryIndent)
        {
            // Pop sub entries
            // Push the current section
            if(sectionDepth > 0)
            {
                section_t *parentSection = nullptr;

                // Append to parent section
                if(currentSection->parentSectionIndex != -1)
                {
                    parentSection = &m_sections[currentSection->parentSectionIndex];
                    auto currentSectionIndex = m_sections.size();
                    parentSection->childIndices.push_back(currentSectionIndex);
                }

                // Go up the hierarchy
                --sectionDepth;
                currentSection = parentSection;
            }
            else
            {
                throw tchar_exception(_T("Incomplete section file."));
            }
        }
        else
        {
            // Key, Value-mapping
            auto splitPos = line.find(' ');
            if(splitPos > 0)
            {
                auto key = line.substr(0, splitPos);
                auto value = line.substr(splitPos + 1, std::string::npos);
                currentSection->mappings[key] = value;
            }
            else
            {
                // Empty mapping
                currentEntry->mappings[line] = "";
            }
        }*/
    }
}


auto gat::unity_meta_file::entries() const noexcept -> const entries_t &
{
    return m_entries;
}


go::file_path gat::unity_meta_file::asset_path() const noexcept
{
    return m_assetPath;
}