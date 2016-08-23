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
// simple_resource_manager member functions
// ================================================================================ //

template<class Loader, class Resource, class StorageKey>
simple_resource_manager<Loader, Resource, StorageKey>::~simple_resource_manager()
{
    for(auto &it : m_resources)
    {
        delete it.second;
    }
}


template<class Loader, class Resource, class StorageKey>
auto simple_resource_manager<Loader, Resource, StorageKey>::operator[](const storage_key_t &key) -> resource_t *
{
    return get(key);
}


template<class Loader, class Resource, class StorageKey>
template<typename... Args>
auto simple_resource_manager<Loader, Resource, StorageKey>::load(const storage_key_t &key, Args&&... args) -> resource_t *
{
    auto it = m_resources.insert(std::make_pair(key, nullptr));
    if(!it.second)
    {
        // Already exists
        return it.first->second;
    }

    // Load resource
    auto resource = loader_t::load((typename loader_t::manager_t *)this, key, std::forward<Args>(args)...);
    if(!resource)
    {
        // Failed to load
        return nullptr;
    }

    // Bind resource
    it.first->second = std::move(resource);

    return resource;
}


template<class Loader, class Resource, class StorageKey>
auto simple_resource_manager<Loader, Resource, StorageKey>::get(const storage_key_t &key) -> resource_t *
{
    auto it = m_resources.find(key);
    return it != m_resources.end() ? it->second : nullptr;
}


template<class Loader, class Resource, class StorageKey>
void simple_resource_manager<Loader, Resource, StorageKey>::unload(const storage_key_t &key)
{
    m_resources.erase(key);
}
