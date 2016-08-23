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

#include "gat/files/section_file.h"


#include "gat/util/file_util.h"
#include "gat/util/converters.h"
#include "gat/util/exception.h"


//===========================================================================
// gat::section_file
//===========================================================================

gat::section_file::section_file(const go::file_path &filePath)
{
    load(filePath);
}


void gat::section_file::load(const go::file_path &filePath)
{
    m_sections.clear();

    // Read in the section file
    std::stringstream txt(gat::file_util::read_text_file(filePath));

    // The first line contains the version
    std::string line;
    if(std::getline(txt, line))
    {
        if(line == "!version 0.9")
        {
            load_section_file_v09(txt);
        }
        else
        {
            throw tchar_exception(_T("Invalid section file."));
        }
    }
}


void gat::section_file::load_section_file_v09(std::stringstream &text)
{
    std::string kToken_Section = "#section";
    std::string kToken_List = "#list";
    std::string kToken_Section_End = "#";

    std::string line;
    section_t *currentSection = nullptr;
    int sectionDepth = 0;
    
    m_sections.reserve(1024 * 10);

    while(std::getline(text, line))
    {
        auto isNewSection = line.find(kToken_Section) == 0;
        auto isNewList = line.find(kToken_List) == 0;

        if(isNewSection || isNewList)
        {
            // Section start
            ++sectionDepth;

            int32_t parentSectionIndex = currentSection ? currentSection->index : -1;

            // Allocate new section
            m_sections.emplace_back();
            currentSection = &m_sections.back();

            auto prefixLen = isNewSection ? kToken_Section.length() : kToken_List.length();
            currentSection->name = line.substr(prefixLen + 1, std::string::npos);
            currentSection->parentSectionIndex = parentSectionIndex;
            currentSection->isList = isNewList;
            currentSection->index = m_sections.size() - 1;
        }
        else if(line == kToken_Section_End)
        {
            // Section end
            // Push the current section
            if(sectionDepth > 0)
            {
                section_t *parentSection = nullptr;

                // Append to parent section
                if(currentSection->parentSectionIndex != -1)
                {
                    parentSection = &m_sections[currentSection->parentSectionIndex];
                    parentSection->childIndices.push_back(currentSection->index);
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
        else if(!line.empty() && sectionDepth > 0)
        {
            if(!currentSection->isList)
            {
                // Key, Value-mapping
                auto splitPos = line.find(' ');
                if(splitPos > 0)
                {
                    auto key = line.substr(0, splitPos);
                    auto value = line.substr(splitPos + 1, std::string::npos);
                    currentSection->mappings[key] = std::move(value);
                }
                else
                {
                    // Empty mapping
                    currentSection->mappings[line] = "";
                }
            }
            else
            {
                // Value only
                currentSection->listContent.push_back(line);
            }
        }
    }
}


auto gat::section_file::sections() const noexcept -> const sections_t &
{
    return m_sections;
}


auto gat::section_file::find_section(const std::string &name) noexcept -> section_t *
{
    auto it = std::find_if(m_sections.begin(), m_sections.end(), [name](const section_t &s)
    {
        return s.name == name;
    });

    return it != m_sections.end() ? &*it : nullptr;
}


void gat::section_file::next_sibling(const section_t **section) noexcept
{
    if(section)
    {
        auto s = *section;
        if(s)
        {
            auto searchIndex = s->parentSectionIndex;
            for(auto i = s->index + 1, end = m_sections.size(); i < end; ++i)
            {
                if(m_sections[i].parentSectionIndex == searchIndex)
                {
                    *section = &m_sections[i];
                    return;
                }
            }
        }

        *section = nullptr;
    }
}