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

/*!
    \file go/asset_management/asset_database.h
    \brief Contains a class for managing databased assets.

    \author Gokhan Ozdogan
    \version 1.0
*/

#pragma once


#include "go/core/common.h"
#include "go/core/path_utility.h"
#include "go/filesystem/views/native/xml_asset_db.h"


GO_BEGIN_NAMESPACE

//! A base class for asset database records.
struct asset_record
{
    //! The asset identifier.
    rid id = -1;
    //! The asset name.
    std::string name;
};


//! A structure, which holds the parameters to initialize an asset database.
struct asset_db_load_parameters
{
    //! The database file path.
    go::file_path filePath;
    //! The database content path.
    go::file_path contentPath;
};


//! A class, which provides a basic interfaces for asset databases.
template<typename Record>
class asset_database
{
    GO_DECLARE_NON_COPYABLE(asset_database)
public:
    typedef Record record_t;
public:
    /*!
        Constructor.

        \param dbName The name of the database. (e.g. modeldb)
        \param rootName The record root name. (e.g. models)
        \param recordName The name of the record descriptors. (e.g. model)
     */
    asset_database(std::string dbName, std::string rootName, std::string recordName);
    //! Destructor.
    virtual ~asset_database() {}
public:
    /*!
        Find-operator.

        \param id The resource identifier.
     */
    record_t *operator[](rid id);
    //! \overload
    record_t *operator[](const std::string &name);
public:
    /*!
        Loads the database from file.

        \param filePath The database file.
        \param dbPath The database data path.
     */
    void load(const file_path &filePath, const file_path &dbPath);
    //! \overload
    void load(const asset_db_load_parameters &loadParams);
    /*!
        Returns the current data path.

        \return The current data path.
     */
    const file_path &path() const noexcept;
    /*!
        Searches for a resource using its identifier.

        \param id The resource identifier.

        \return If the resource definition was found, the return value is a pointer to that definition.
        Otherwise, the return value is nullptr.
     */
    record_t *find(rid id) noexcept;
    /*!
        Searches for a resource using its name.

        This function is slower than the ID-function.

        \param name The resource name.

        \return If the resource definition was found, the return value is a pointer to that definition.
        Otherwise, the return value is nullptr.
     */
    record_t *find(const std::string &name) noexcept;
    /*!
        Returns whether a record exists.

        \param id The resource ID.

        \return id The record identifier.
     */
    bool exists(rid id) const noexcept;
    /*!
        Returns the database capacity in records.

        \return The database capacity.
     */
    size_t capacity() const noexcept;
protected:
    /*!
        Processes a record in the database.

        \param n The record descriptor.
        \param[in, out] outRecord The output record.
     */
    virtual void process_record(rapidxml::xml_node<> *n, record_t &outRecord) = 0;
private:
    typedef std::vector<Record> record_vector_t;
    typedef std::unordered_map<std::string, Record *> string_map_t;
private:
    //! The name of the database.
    std::string m_dbName;
    //! The record root name.
    std::string m_rootName;
    //! The name of the record descriptors.
    std::string m_recordName;
    //! The database path.
    file_path m_dbPath;
    //! The list of records in the entire database.
    record_vector_t m_records;
    //! Maps record names to IDs.
    string_map_t m_recordStringMap;
};

#include "asset_database_impl.h"

GO_END_NAMESPACE
