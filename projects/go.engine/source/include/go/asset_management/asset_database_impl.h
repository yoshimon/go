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

// ================================================================================ //
// go::asset_database member functions
// ================================================================================ //

template<typename Record>
asset_database<Record>::asset_database(std::string dbName, std::string rootName, std::string recordName)
    : m_dbName(std::move(dbName)), m_rootName(std::move(rootName)), m_recordName(std::move(recordName))
{
}


template<typename Record>
auto asset_database<Record>::operator[](rid id) -> record_t *
{
    return find(id);
}

    
template<typename Record>
auto asset_database<Record>::operator[](const std::string &name) -> record_t *
{
    return find(name);
}


template<typename Record>
void asset_database<Record>::load(const file_path &filePath, const file_path &dbPath)
{
    go::xml_asset_db db(m_dbName, m_rootName, m_recordName, filePath, dbPath);
    m_dbPath = dbPath;

    // Load all records into memory
    m_records.clear();
    m_recordStringMap.clear();

    // Pre-allocate the entire DB
    m_records.resize(db.capacity());
    m_recordStringMap.reserve(db.capacity());

    for(auto &n : db)
    {
        auto id = std::stol(n->first_attribute("id")->value());
        auto &r = m_records[id];
        if(r.id != -1)
        {
            GO_THROW(go::data_error, "Duplicate database record IDs detected.");
        }

        // Read base record data
        r.id = id;
        
        auto attName = n->first_attribute("name");
        if(!attName)
        {
            GO_THROW(go::data_error, "Missing record name attribute.");
        }

        r.name = attName->value();

        m_recordStringMap[r.name] = &r;

        // Process the rest of the record
        process_record(n, r);
    }
}


template<typename Record>
void asset_database<Record>::load(const asset_db_load_parameters &params)
{
    return load(params.filePath, params.contentPath);
}


template<typename Record>
auto asset_database<Record>::path() const noexcept -> const file_path &
{
    return m_dbPath;
}


template<typename Record>
auto asset_database<Record>::find(rid id) noexcept -> record_t *
{
    return exists(id) ? &m_records[id] : nullptr;
}


template<typename Record>
auto asset_database<Record>::find(const std::string &name) noexcept -> record_t *
{
    auto it = m_recordStringMap.find(name);
    return it != m_recordStringMap.end() ? it->second : nullptr;
}


template<typename Record>
bool asset_database<Record>::exists(rid id) const noexcept
{
    return (id >= 0) && (id < (rid)m_records.size()) && (m_records[id].id != -1);
}


template<typename Record>
size_t asset_database<Record>::capacity() const noexcept
{
    return m_records.size();
}
