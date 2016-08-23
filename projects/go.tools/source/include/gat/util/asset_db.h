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
#include "gat/files/asset_meta_file.h"
#include "go/golib.h"
#include "rapidxml/rapidxml.hpp"


GAT_BEGIN_NAMESPACE

/*!
    A class, which represents an iterator for an asset database.
 */
class asset_db_iterator
    : public std::iterator<std::input_iterator_tag,
    rapidxml::xml_node<> *, ptrdiff_t, const rapidxml::xml_node<> *, const rapidxml::xml_node<> * &>
{
public:
    /*!
        Constructor.

        \param value The value behind the iterator.
     */
    asset_db_iterator(rapidxml::xml_node<> *const value) : m_value(value) {}
public:
    /*!
        Dereference-operator.

        \return The value behind the iterator.
     */
    rapidxml::xml_node<> *const &operator*() const noexcept { return m_value; }
    /*!
        Value-access-operator.

        \return The value behind the iterator.
     */
    rapidxml::xml_node<> *const *operator->() const noexcept { return &m_value; }
    /*!
        Advances the iterator.

        \return The new iterator.
     */
    asset_db_iterator &operator++() { m_value = m_value->next_sibling(m_value->name()); return *this; }
    /*!
        Advances the iterator.

        \return The new iterator.
     */
    asset_db_iterator operator++(int i) { while(i > 0) { ++(*this); --i; } return *this; }
    /*!
        Tests the iterator for equality.

        \param other The other iterator.

        \return True if the iterators are equal. Otherwise false.
     */
    bool equal(const asset_db_iterator &other) const { return m_value == *other; }
private:
    /*!
        The iterator value.
     */
    rapidxml::xml_node<> *m_value;
};


/*!
    Tests the iterator for equality.

    \param lhs The left-hand side.
    \param rhs The left-hand side.

    \return True if the iterators are equal. Otherwise false.
 */
inline bool operator==(const asset_db_iterator &lhs, const asset_db_iterator &rhs)
{
    return lhs.equal(rhs);
}


/*!
    Tests the iterator for inequality.

    \param lhs The left-hand side.
    \param rhs The left-hand side.

    \return True if the iterators are equal. Otherwise false.
 */
inline bool operator!=(const asset_db_iterator &lhs, const asset_db_iterator &rhs)
{
    return !lhs.equal(rhs);
}


/*!
    A class, that represents an asset database.
 */
class asset_db
{
public:
    typedef asset_db_iterator iterator;
    typedef asset_db_iterator const_iterator;
public:
    /*!
        Returns an iterator to the first database entry.

        \return An iterator to the first database entry.
     */
    iterator begin() { return iterator(m_root->first_node(m_recordName.c_str())); }
    /*!
        Returns an iterator to the first database entry.

        \return An iterator to the first database entry.
     */
    const_iterator begin() const { return const_iterator(m_root->first_node(m_recordName.c_str())); }
    /*!
        Returns an iterator to the one-past-end database entry.

        \return An iterator to the one-past-end database entry.
     */
    iterator end() { return iterator(nullptr); }
    /*!
        Returns an iterator to the one-past-end database entry.

        \return An iterator to the one-past-end database entry.
     */
    const_iterator end() const { return const_iterator(nullptr); }
public:
    /*!
        Constructor.

        \param name The name of the database.
        \param rootName The name of the root node in the database.
        \param recordName The name of a record node in the database.
     */
    asset_db(std::string name, std::string rootName, std::string recordName);
    /*!
        \overload
     */
    asset_db(std::string name, std::string rootName, std::string recordName, const go::file_path &filePath);
public:
    /*!
        Imports a material into the database.

        \param metaFile The meta file.
        \param name The database name.
        \param group The group to import the resource into.
     */
    virtual void import(
        const asset_meta_file &metaFile,
        const std::string &name,
        const go::file_path &group) = 0;
public:
    /*!
        Returns the capacity of the database.

        \return The database capacity.
     */
    uint32_t size() const noexcept;
    /*!
        Finds a record in the database.

        \param name The name to match.

        \return The database record or nullptr if no matching record was found.
     */
    rapidxml::xml_node<> *find(const asset_db_name &name);
    /*!
        \overload
     */
    rapidxml::xml_node<> *find(asset_db_id id);
    /*!
        Finds a partial match in the database.

        \param path The database path to match.

        \return The database record or nullptr if no matching record was found.
     */
    std::vector<rapidxml::xml_node<> *> find_partial(const asset_db_name &path);
    /*!
        Finds all assets that belong to the same group.
        
        \param group The name of the group to search for.

        \return The asset group.
     */
    std::vector<rapidxml::xml_node<> *> find_group(const go::file_path &group);
    /*!
        Removes all entries that match a path from the database.

        \param path The path to remove.
     */
    void remove_partial(const asset_db_name &path);
    /*!
        Removes an entry that matches a name from the database.

        \param name The entry to remove.
     */
    void remove(const asset_db_name &name);
    /*!
        Removes a group from the database.

        \param group The group to remove.
     */
    void remove_group(const go::file_path &group);
    /*!
        \overload
     */
    void remove(rapidxml::xml_node<> *node);
    /*!
        \overload
     */
    void remove(const std::vector<rapidxml::xml_node<> *> &nodes);
    /*!
        Saves the database.

        \param filePath The file path to save the file at.
     */
    void save(const go::file_path &filePath);
    /*!
        Returns the root node of the database.

        \return The root node of the database.
     */
    rapidxml::xml_node<> *root_node() noexcept { return m_root; }
    /*!
        Returns the name of the database.

        \return The name of the database.
     */
    const std::string &name() const noexcept { return m_name; }
    /*!
        Returns the root name of the database.

        \return The root name of the database.
     */
    const std::string &root_name() const noexcept  { return m_rootName; }
    /*!
        Returns the record name of the database.

        \return The record name of the database.
     */
    const std::string &record_name() const noexcept  { return m_recordName; }
protected:
    /*!
        Loads the database from file.

        \param filePath The file path.
     */
    void load(const go::file_path &filePath);
    /*!
        Generates a new, unique asset ID.

        \return The asset ID.
     */
    asset_db_id generate_id();
    /*!
        Caches all asset IDs for quick lookup.
     */
    void gather_ids();
    /*!
        Adds a new record to the database.

        \param name The record name.
        \param group The name of the group to add the record to.
        \param value The record value.

        \return The new XML record.
     */
    rapidxml::xml_node<> *add_record(const asset_db_name &name, const go::file_path &group, const char *value = nullptr);
private:
    /*!
        Returns an ID to the ID pool.
     */
    void return_id(asset_db_id id);
private:
    /*!
        The XML database.
     */
    rapidxml::xml_document<> m_doc;
    /*!
        The XML root node.
     */
    rapidxml::xml_node<> *m_root;
    /*!
        The XML source data.
     */
    std::string m_xmlSource;
    /*!
        The asset IDs.
     */
    asset_db_ids m_ids;
    /*!
        The database name.
     */
    std::string m_name;
    /*!
        The root name.
     */
    std::string m_rootName;
    /*!
        The record name.
     */
    std::string m_recordName;
};


/*!
    A class, which represents an asset database with an additional data path.
 */
class asset_datapath_db : public asset_db
{
public:
    /*!
        Constructor.

        \param name The name of the database.
        \param rootName The name of the root node in the database.
        \param dataPath The data path.
        \param recordName The name of a record node in the database.
     */
    asset_datapath_db(std::string name, std::string rootName, std::string recordName, const go::file_path &dataPath);
    /*!
        \overload
    */
    asset_datapath_db(std::string name, std::string rootName, std::string recordName, const go::file_path &dataPath, const go::file_path &filePath);
public:
    /*!
        Returns the data path.

        \return The data path.
     */
    go::file_path data_path() const noexcept { return m_dataPath; }
    /*!
        Copies an asset into the data directory.

        \param srcFilePath The source file path.
        \param dstFilePath The destination file path.
     */
    void copy_asset(const go::file_path &srcFilePath, const go::file_path &dstFilePath) const;
private:
    /*!
        The data path.
     */
    go::file_path m_dataPath;
};

GAT_END_NAMESPACE