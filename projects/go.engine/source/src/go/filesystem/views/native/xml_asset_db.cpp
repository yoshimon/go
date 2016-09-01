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

#include "go/stdafx.h"

#include "go/filesystem/views/native/xml_asset_db.h"

#include "go/core/exception.h"
#include "go/filesystem/file.h"
#include "go/util/rapidxml_helper.h"


using namespace DirectX;

//===========================================================================
// go::asset_name
//===========================================================================

go::asset_name::asset_name(const char *value)
    : asset_name(std::string(value))
{
}


go::asset_name::asset_name(std::string value)
{
    *this = std::move(value);
}


go::asset_name::operator const std::string &() const noexcept
{
    return m_value;
}


auto go::asset_name::operator=(std::string value) noexcept -> asset_name &
{
    m_value = std::move(value);
    return *this;
}


const std::string &go::asset_name::str() const noexcept
{
    return m_value;
}


const char *go::asset_name::c_str() const noexcept
{
    return m_value.c_str();
}


//===========================================================================
// go::xml_asset_db
//===========================================================================

go::xml_asset_db::xml_asset_db(std::string name, std::string rootName, std::string recordName)
    :
    m_name(std::move(name)),
    m_rootName(std::move(rootName)),
    m_recordName(std::move(recordName)),
    m_root(nullptr),
    m_capacity(0)
{
}


go::xml_asset_db::xml_asset_db(std::string name, std::string rootName, std::string recordName,
    const file_path &fileName, const file_path &dbPath)
    : xml_asset_db(std::move(name), std::move(rootName), std::move(recordName))
{
    load(fileName, dbPath);
}


void go::xml_asset_db::load(const file_path &fileName, const file_path &dbPath)
{
    m_xmlFilePath = fileName;
    m_xmlSource = go::text_file::read(fileName);
    try
    {
        m_doc.parse<0>((char *)m_xmlSource.c_str());
        m_path = dbPath;
    }
    catch(...)
    {
        GO_THROW(go::data_error, "Could not parse database.");
    }

    m_root = m_doc.first_node(m_name.c_str());
    if(!m_root)
    {
        GO_THROW(go::data_error, "Invalid database.");
    }

    auto aCapacity = m_root->first_attribute("capacity");
    if(!aCapacity)
    {
        GO_THROW(go::data_error, "Database is missing capacity attribute.");
    }

    m_capacity = std::stol(aCapacity->value());

    m_root = m_root->first_node(m_rootName.c_str());
    if(!m_root)
    {
        GO_THROW(go::data_error, "Invalid database.");
    }

    cache_record_ids();
}


void go::xml_asset_db::save(const file_path *filePath)
{
    if(filePath)
    {
        m_xmlFilePath = *filePath;
    }

    xml_save_document(m_doc, m_xmlFilePath.c_str());
}


rapidxml::xml_node<> *go::xml_asset_db::find(const asset_name &name) const
{
    for(auto &nRecord : *this)
    {
        auto aName = nRecord->first_attribute("name");
        if(!aName)
        {
            // Invalid model declaration
            GO_THROW(go::data_error, "Corrupted database.");
        }

        if(strcmp(aName->value(), name.c_str()) == 0)
        {
            return nRecord;
        }
    }

    return nullptr;
}


rapidxml::xml_node<> *go::xml_asset_db::find(rid id) const
{
    auto root = m_root;
    auto &ids = m_ids;
    
    auto it = std::find(ids.begin(), ids.end(), id);
    if(it != ids.end())
    {
        auto idx = it - ids.begin();
        return xml_child_at(root, idx, m_recordName.c_str());
    }

    return nullptr;
}


std::vector<rapidxml::xml_node<> *> go::xml_asset_db::find_matches(const asset_name &path)
{
    std::vector<rapidxml::xml_node<> *> result;
    result.reserve(64);

    for(auto &nRecord : *this)
    {
        auto aName = nRecord->first_attribute("name");
        if(!aName)
        {
            // Invalid model declaration
            GO_THROW(go::data_error, "Corrupted database.");
        }

        if(strstr(aName->value(), path.c_str()) == aName->value())
        {
            result.push_back(nRecord);
        }
    }

    return result;
}


void go::xml_asset_db::cache_record_ids()
{
    // auto root = m_root;

    // Reserve ID cache space
    auto &idCache = m_ids;
    idCache.clear();
    idCache.reserve(1024 * 64);

    for(auto &nRecord : *this)
    {
        auto aID = nRecord->first_attribute("id");
        if(aID)
        {
            idCache.push_back(std::stoi(aID->value()));
        }
    }

    // Verify that the sequence is sorted
    if(!std::is_sorted(idCache.begin(), idCache.end()))
    {
        GO_THROW(go::data_error, "The database ID sequence has to be sorted.");
    }

    // Verify that there are no duplicates
    auto lastUnique = std::unique(idCache.begin(), idCache.end());
    if(lastUnique != idCache.end())
    {
        GO_THROW(go::data_error, "Duplicate IDs are not allowed.");
    }
}


auto go::xml_asset_db::generate_id() -> rid
{
    auto &ids = m_ids;

    // Scan ID vector and find the first empty slot
    size_t result = ids.size();
    for(size_t i = 0, count = ids.size(); i < count; ++i)
    {
        if(i != ids[i])
        {
            result = i;
            break;
        }
    }

    return (rid)result;
}


void go::xml_asset_db::remove(const asset_name &path)
{
    auto matches = find_matches(path);
    for(auto &match : matches)
    {
        match->parent()->remove_node(match);
    }
}


void go::xml_asset_db::change_capacity(uint32_t newCapacity)
{
    m_capacity = newCapacity;
    
    // Update DB attribute
    auto dbNode = m_doc.first_node(m_name.c_str());
    auto valueStr = std::to_string(newCapacity);
    xml_require_attribute(dbNode, "capacity", valueStr.c_str());
}
