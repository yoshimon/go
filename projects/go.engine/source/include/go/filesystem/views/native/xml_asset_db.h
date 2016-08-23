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
    \file filesystem/views/native/xml_asset_db.h
    \brief Contains a class for accessing XML asset databases.

    \author Gokhan Ozdogan
    \version 1.0
*/

#pragma once


#include "go/core/common.h"
#include "go/core/path_utility.h"
#include "rapidxml/rapidxml.hpp"


GO_BEGIN_NAMESPACE

//! A class, which represents the name of an asset.
class GO_API asset_name
{
public:
    /*!
        Constructor.

        \param value The asset name.
     */
    asset_name(const char *value);
    //! \overload
    asset_name(std::string value);
    //! \overload
    asset_name() = default;
public:
    //! Returns the string-representation of the asset name.
    operator const std::string &() const noexcept;
    //! Assignment-operator.
    asset_name &operator=(std::string value) noexcept;
public:
    /*!
        Returns the string-representation of the asset name.

        \return The string-representation.
     */
    const std::string &str() const noexcept;
    /*!
        Returns the string-representation of the asset name.

        \return The string-representation.
     */
    const char *c_str() const noexcept;
private:
    //! The asset name.
    std::string m_value;
};


//! A class, which represents an iterator for asset database records.
class xml_asset_db_iterator
    : public std::iterator<std::input_iterator_tag,
        rapidxml::xml_node<> *, ptrdiff_t, const rapidxml::xml_node<> *, const rapidxml::xml_node<> * &>
{
public:
    /*!
        Constructor.

        \param value The iterator value.
     */
    xml_asset_db_iterator(rapidxml::xml_node<> *const value) : m_value(value) {}
public:
    /*!
        Value-operator.

        \return The XML node.
     */
    rapidxml::xml_node<> *const &operator*() const { return m_value; }
    /*!
        Value-access operator.

        \return The XML node pointer.
     */
    rapidxml::xml_node<> *const *operator->() const { return &m_value; }
    /*!
        Increment-operator.

        \param The incremented iterator.
     */
    xml_asset_db_iterator &operator++() { m_value = m_value->next_sibling(m_value->name()); return *this; }
    //! \overload
    xml_asset_db_iterator operator++(int i) { while(i > 0) { ++(*this); --i; } return *this; }
public:
    /*!
        Tests whether two iterators are equal.

        \param rhs The right-hand side.

        \return True, if both iterators point to the same node. Otherwise false.
     */
    bool equal(const xml_asset_db_iterator &rhs) const { return m_value == *rhs; }
private:
    //! The XML node.
    rapidxml::xml_node<> *m_value;
};

/*!
    Equality-operator.

    \param lhs The left-hand side.
    \param rhs The right-hand side.

    \return True, if both iterators are equal. Otherwise false.
 */
inline bool operator==(const xml_asset_db_iterator &lhs, const xml_asset_db_iterator &rhs)
{
    return lhs.equal(rhs);
}


/*!
    Inequality-operator.

    \param lhs The left-hand side.
    \param rhs The right-hand side.

    \return True, if both iterators are unequal. Otherwise false.
 */
inline bool operator!=(const xml_asset_db_iterator &lhs, const xml_asset_db_iterator &rhs)
{
    return !lhs.equal(rhs);
}


//! A class, which represents an XML asset database.
class GO_API xml_asset_db
{
    GO_DECLARE_NON_COPYABLE(xml_asset_db)
public:
    typedef xml_asset_db_iterator iterator;
    typedef xml_asset_db_iterator const_iterator;
public:
    /*!
        Returns an iterator to the first record.

        \return An iterator to the first record.
     */
    iterator begin() { return iterator(m_root->first_node(m_recordName.c_str())); }
    /*!
        Returns an iterator to the first record.

        \return An iterator to the first record.
     */
    const_iterator begin() const { return const_iterator(m_root->first_node(m_recordName.c_str())); }
    /*!
        Returns an iterator to an invalid record.

        \return Always returns an iterator to nullptr.
     */
    iterator end() { return iterator(nullptr); }
    /*!
        Returns an iterator to an invalid record.

        \return Always returns an iterator to nullptr.
     */
    const_iterator end() const { return const_iterator(nullptr); }
public:
    /*!
        Constructor.

        \param name The database name.
        \param rootName The record root container node name.
        \param recordName The record name.
        \param filePath The database file to load.
        \param dbPath The database data directory path.
     */
    xml_asset_db(std::string name, std::string rootName, std::string recordName,
        const file_path &filePath, const file_path &dbPath);
    //! \overload
    xml_asset_db(std::string name, std::string rootName, std::string recordName);
public:
    /*!
        Returns the maximum number of elements in the database.

        \return The maximum number of records in the database.
     */
    uint32_t capacity() const noexcept { return m_capacity; }
    /*!
        Changes the maximum number of records in the database.

        \param newCapacity The new capacity.
     */
    void change_capacity(uint32_t newCapacity);
    /*!
        Searches for a record.

        \param name The name of the record to search for.
     
        \return If the record was found, the return value is pointer to the record node.
        Otherwise, the return value is nullptr.
     */
    rapidxml::xml_node<> *find(const asset_name &name) const;
    //! \overload
    rapidxml::xml_node<> *find(rid id) const;
    /*!
        Searches for all nodes that partially (from the beginning) match a path.

        \param path The record name substring to search for.

        \return A list of nodes that match the specified path.
     */
    std::vector<rapidxml::xml_node<> *> find_matches(const asset_name &path);
    /*!
        Removes a set of nodes that match a path from the database.

        \param path The path to search for.
     */
    void remove(const asset_name &path);
    //! Saves the database.
    void save(const file_path *filePath = nullptr);
    /*!
        The database data directory path.

        \return The database data directory path.
     */
    const file_path &path() noexcept { return m_path; }
    /*!
        Returns the root node.

        \return The root node.
     */
    rapidxml::xml_node<> *root_node() noexcept { return m_root; }
    /*!
        Returns the database name.

        \return The database name.
     */
    const std::string &name() const noexcept { return m_name; }
    /*!
        Returns the root name.

        \return The root name.
     */
    const std::string &root_name() const noexcept { return m_rootName; }
    /*!
        Returns the record name.

        \return The record name.
     */
    const std::string &record_name() const noexcept { return m_recordName; }
protected:
    /*!
        Loads the database from file.

        \param filePath The file path.
        \param dbPath The database data directory path.
     */
    void load(const file_path &fileName, const file_path &dbPath);
    /*!
        Generates a new, unique database identifier.

        \return The new database identifier.
     */
    rid generate_id();
    //! Gathers the set of identifiers from all records in the database.
    void cache_record_ids();
private:
    //! The XML document.
    rapidxml::xml_document<> m_doc;
    //! The root node.
    rapidxml::xml_node<> *m_root;
    //! The XML file path.
    file_path m_xmlFilePath;
    //! The XML source.
    std::string m_xmlSource;
    //! The database data directory path.
    file_path m_path;
    //! The record ID cache.
    rid_vector m_ids;
    //! The database name.
    std::string m_name;
    //! The root name.
    std::string m_rootName;
    //! The record name.
    std::string m_recordName;
    //! The database capacity.
    uint32_t m_capacity;
};

GO_END_NAMESPACE
