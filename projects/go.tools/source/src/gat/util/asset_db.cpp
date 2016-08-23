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

#include "gat/util/asset_db.h"


#include "gat/util/exception.h"
#include "gat/util/file_util.h"
#include "gat/util/rapidxml_helper.h"
#include <filesystem>


using namespace DirectX;
namespace fs = std::experimental::filesystem;


//===========================================================================
// gat::asset_db
//===========================================================================

gat::asset_db::asset_db(std::string name, std::string rootName, std::string recordName)
    : m_name(std::move(name)), m_rootName(std::move(rootName)), m_recordName(std::move(recordName))
{
}


gat::asset_db::asset_db(std::string name, std::string rootName, std::string recordName,
    const go::file_path &filePath)
    : asset_db(std::move(name), std::move(rootName), std::move(recordName))
{
    load(filePath);
}


void gat::asset_db::load(const go::file_path &filePath)
{
    m_xmlSource = file_util::read_text_file(filePath);
    try
    {
        m_doc.parse<0>((char *)m_xmlSource.c_str());

        m_root = m_doc.first_node(m_name.c_str());
        if(!m_root)
        {
            throw tchar_exception(_T("Invalid database."));
        }

        m_root = xml_require_child(m_root, m_rootName.c_str());

        gather_ids();
    }
    catch(...)
    {
        throw tchar_exception(_T("Could not parse database."));
    }
}


void gat::asset_db::save(const go::file_path &filePath)
{
    // Update the capacity attribute
    go::xml_require_attribute(m_doc.first_node(m_name.c_str()), "capacity", std::to_string(size()).c_str());

    go::xml_save_document(m_doc, filePath);
}


rapidxml::xml_node<> *gat::asset_db::find(const asset_db_name &name)
{
    for(auto &nRecord : *this)
    {
        auto aName = xml_require_attribute(nRecord, "name");
        if(strcmp(aName->value(), name.c_str()) == 0)
        {
            return nRecord;
        }
    }

    return nullptr;
}


rapidxml::xml_node<> *gat::asset_db::find(asset_db_id id)
{
    auto root = m_root;
    auto &ids = m_ids;

    auto it = std::find(ids.begin(), ids.end(), id);
    if(it != ids.end())
    {
        auto idx = it - ids.begin();
        return go::xml_child_at(root, idx, m_recordName.c_str());
    }

    return nullptr;
}


std::vector<rapidxml::xml_node<> *> gat::asset_db::find_partial(const asset_db_name &path)
{
    std::vector<rapidxml::xml_node<> *> result;
    result.reserve(64);

    for(auto &nRecord : *this)
    {
        auto aName = xml_require_attribute(nRecord, "name");
        if(strstr(aName->value(), path.c_str()) == aName->value())
        {
            result.push_back(nRecord);
        }
    }

    return result;
}


std::vector<rapidxml::xml_node<> *> gat::asset_db::find_group(const go::file_path &group)
{
    std::vector<rapidxml::xml_node<> *> result;
    result.reserve(1024);

    for(auto &nRecord : *this)
    {
        auto aAttr = nRecord->first_attribute("group");
        if(aAttr)
        {
            if(!tstrcmp(aAttr->value(), group.c_str()))
            {
                result.push_back(nRecord);
            }
        }
    }

    return result;
}


void gat::asset_db::gather_ids()
{
    // Reserve ID cache space
    auto &idCache = m_ids;
    idCache.clear();
    idCache.reserve(1024 * 64);

    for(auto &nRecord : *this)
    {
        if(strcmp(nRecord->name(), m_recordName.c_str()))
        {
            throw tchar_exception(_T("Invalid record node detected."));
        }

        idCache.push_back(std::stol(xml_require_attribute(nRecord, "id")->value()));
    }

    // Verify that the sequence is sorted
    if(!std::is_sorted(idCache.begin(), idCache.end()))
    {
        throw tchar_exception(_T("The database ID sequence has to be sorted."));
    }

    // Verify that there are no duplicates
    auto lastUnique = std::unique(idCache.begin(), idCache.end());
    if(lastUnique != idCache.end())
    {
        throw tchar_exception(_T("Duplicate IDs are not allowed."));
    }
}


gat::asset_db_id gat::asset_db::generate_id()
{
    // Scan ID vector and find the first empty slot
    size_t result = m_ids.size();
    for(size_t i = 0, count = m_ids.size(); i < count; ++i)
    {
        if(i != m_ids[i])
        {
            result = i;
            m_ids.insert(m_ids.begin() + i, i);
            break;
        }
    }

    if(result == m_ids.size())
    {
        m_ids.push_back(result);
    }

    return result;
}


void gat::asset_db::remove_partial(const asset_db_name &path)
{
    remove(find_partial(path));
}


void gat::asset_db::remove(const asset_db_name &name)
{
    remove(find(name));
}


void gat::asset_db::remove(rapidxml::xml_node<> *node)
{
    if(node)
    {
        auto id = std::stol(xml_require_attribute(node, "id")->value());
        node->parent()->remove_node(node);
        return_id(id);
    }
}


void gat::asset_db::remove(const std::vector<rapidxml::xml_node<> *> &nodes)
{
    for(auto &node : nodes)
    {
        remove(node);
    }
}


void gat::asset_db::remove_group(const go::file_path &group)
{
    std::vector<rapidxml::xml_node<> *> removed;

    for(auto &nRecord : *this)
    {
        auto aGrp = nRecord->first_attribute("group");
        if(aGrp)
        {
            if(!strcmp(aGrp->value(), group.c_str()))
            {
                removed.push_back(nRecord);
            }
        }
    }

    for(auto &node : removed)
    {
        remove(node);
    }
}


void gat::asset_db::return_id(asset_db_id id)
{
    for(size_t i = 0, count = m_ids.size(); i < count; ++i)
    {
        if(m_ids[i] == id)
        {
            m_ids.erase(m_ids.begin() + i);
            return;
        }
    }

    throw tchar_exception(_T("Invalid record ID."));
}


uint32_t gat::asset_db::size() const noexcept
{
    return m_ids.size();
}


rapidxml::xml_node<> *gat::asset_db::add_record(const asset_db_name &name, const go::file_path &group,
    const char *value)
{
    if(find(name))
    {
        _tprintf(_T("A record with the name '%s' already exists.\n"), name.c_str());
        throw tchar_exception(_T("Duplicate record detected."));
    }

    auto id = generate_id();
    try
    {
        auto record = go::xml_insert_child(m_root, go::xml_child_at(m_root, id, m_recordName.c_str()), m_recordName.c_str(), value);
        go::xml_require_attribute(record, "id", std::to_string(id).c_str());
        go::xml_require_attribute(record, "name", name.c_str());

        if(!group.is_empty())
        {
            go::xml_require_attribute(record, "group", group.c_str());
        }

        return record;
    }
    catch(...)
    {
        return_id(id);
        throw tchar_exception(_T("Could not add record."));
    }
}

//===========================================================================
// gat::asset_datapath_db
//===========================================================================

gat::asset_datapath_db::asset_datapath_db(std::string name, std::string rootName,
    std::string recordName, const go::file_path &dataPath)
    : asset_db(std::move(name), std::move(rootName), std::move(recordName)), m_dataPath(dataPath)
{
}


gat::asset_datapath_db::asset_datapath_db(std::string name, std::string rootName,
    std::string recordName, const go::file_path &dataPath, const go::file_path &filePath)
    : asset_db(std::move(name), std::move(rootName), std::move(recordName), filePath), m_dataPath(dataPath)
{
}


void gat::asset_datapath_db::copy_asset(const go::file_path &srcFilePath, const go::file_path &dstFilePath) const
{
    auto outAssetPath = m_dataPath + dstFilePath;
    fs::create_directories(outAssetPath.directory_path().c_str());
    fs::copy_file(srcFilePath.c_str(), outAssetPath.c_str(), fs::copy_options::overwrite_existing);
}