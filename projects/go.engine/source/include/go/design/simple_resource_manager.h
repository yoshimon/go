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
    \file design/simple_resource_manager.h
    \brief Contains a class for managing resources, that should only be loaded once.
    Does not maintain an internal handle system.

    \author Gokhan Ozdogan
    \version 1.0
*/

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

/*!
    A base class for resource loaders.

    \tparam Resource The resource type.
    \tparam StorageKey The storage key type.
 */
template<typename Resource, typename StorageKey>
struct simple_resource_loader
{
    typedef Resource resource_t;
    typedef StorageKey storage_key_t;
};


//! A class, which implements a basic resource manager interface.
template<class Loader, class Resource = typename Loader::resource_t, class StorageKey = typename Loader::storage_key_t>
class simple_resource_manager
{
    GO_DECLARE_NON_COPYABLE(simple_resource_manager)
public:
    typedef Loader loader_t;
    typedef Resource resource_t;
    typedef StorageKey storage_key_t;
public:
    //! Constructor.
    simple_resource_manager() = default;
    //! Destructor.
    virtual ~simple_resource_manager();
public:
    /*!
        Resource-lookup operator.

        \param key The storage key.

        \return If the resource was found, the return value is a pointer to that resource.
        Otherwise, the return value is nullptr.
     */
    resource_t *operator[](const storage_key_t &key);
public:
    /*!
        Loads a resource synchronously.

        \tparam Args The argument types to forward to the loader.

        \param key The load key.
        \param args The arguments to forward to the loader.

        \return If the resource was successfully loaded, the return value is a pointer to that resource.
        Otherwise, the return value is nullptr.
     */
    template<typename... Args>
    resource_t *load(const storage_key_t &storageKey, Args&&... args);
    /*!
        Returns a resource using its storage key.
        
        \param key The storage key.

        \return If the resource was found, the return value is a pointer to that resource.
        Otherwise, the return value is nullptr.
     */
    resource_t *get(const storage_key_t &key);
    /*!
        Unloads a resource.

        \param key The storage key.
     */
    void unload(const storage_key_t &key);
private:
    typedef std::unordered_map<storage_key_t, resource_t *> resource_map_t;
private:
    //! The resource database.
    resource_map_t m_resources;
};


#include "simple_resource_manager_impl.h"

GO_END_NAMESPACE
