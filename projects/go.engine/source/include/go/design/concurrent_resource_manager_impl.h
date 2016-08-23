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
// resource_handle member functions
// ================================================================================ //

template<typename ResourceManager>
auto resource_handle<ResourceManager>::operator->() -> resource_ptr_t
{
    if(!*this)
    {
        GO_THROW(data_error, "Dereferenced invalid resource handle.");
    }

    return owner->query_resource(value);
}


template<typename ResourceManager>
auto resource_handle<ResourceManager>::operator->() const -> const resource_ptr_t
{
    if(!*this)
    {
        GO_THROW(data_error, "Dereferenced invalid resource handle.");
    }

    return owner->query_resource(value);
}


template<typename ResourceManager>
resource_handle<ResourceManager>::operator handle_t() const noexcept
{
    return value;
}


template<typename ResourceManager>
bool resource_handle<ResourceManager>::operator!() const noexcept
{
    return (owner != nullptr) && (!owner->is_valid_handle(value, id));
}


template<typename ResourceManager>
resource_handle<ResourceManager>::operator bool() const noexcept
{
    return (owner != nullptr) && (owner->is_valid_handle(value, id));
}


template<typename ResourceManager>
bool resource_handle<ResourceManager>::operator==(this_t &other) const noexcept
{
    return (value == other.value) && (id == other.id) && (owner == other.owner);
}


template<typename ResourceManager>
bool resource_handle<ResourceManager>::operator!=(this_t &other) const noexcept
{
    return !((*this) == other);
}


template<typename ResourceManager>
auto resource_handle<ResourceManager>::operator*() const -> resource_ptr_t
{
    if(!*this)
    {
        GO_THROW(data_error, "Dereferenced invalid resource handle.");
    }

    return owner->query_resource(value);
}


// ================================================================================ //
// concurrent_resource_manager member functions
// ================================================================================ //

template<typename Loader, typename Resource>
concurrent_resource_manager<Loader, Resource>::concurrent_resource_manager(size_t maxCount,
    size_t fallbackResourceCount, size_t budgetSize)
    : m_fallbackResourceCount(0), // This counter is increased by the fallback loader functions
    m_budget(budgetSize),
    m_usedBudget(0)
{
    // NOTE: the first fallbackResourceCount slots are reserved.
    // This allows us to point to the fallback resources using a threshold handle value (i.e. below fallbackResourceCount).
    auto maxNumSlots = maxCount + fallbackResourceCount;
    m_handleMap.resize(maxNumSlots); // The capacity / size has to match
    m_cache = new resource_desc_t[maxNumSlots]; // for every ID-indexed
    m_handleIndirections.resize(maxNumSlots); // container

    // Initialize owner pointers
    for(auto &handle : m_handleMap)
    {
        handle.owner = this;
    }

    m_completedRequests.reserve(64); // Estimated maximum number of completed requests per update()
}


template<typename Loader, typename Resource>
concurrent_resource_manager<Loader, Resource>::~concurrent_resource_manager()
{
    // TODO: cancel pending requests and wait

    // Unload all resources
    for(size_t i = 0, end = m_handleMap.size(); i < end; ++i)
    {
        auto &r = m_cache[i];
        if(r.id != -1)
        {
            m_loader.unload(&r.resource);
        }
    }

    delete[] m_cache;
}


template<typename Loader, typename Resource>
auto concurrent_resource_manager<Loader, Resource>::operator[](rid id) -> resource_handle_t
{
    return get(id);
}


template<typename Loader, typename Resource>
auto concurrent_resource_manager<Loader, Resource>::id_to_handle(rid id) const -> rid 
{
    auto result = id + m_fallbackResourceCount;
    GO_ASSERT(result >= 0 && result < (rid)m_handleMap.size());
    return result;
}


template<typename Loader, typename Resource>
auto concurrent_resource_manager<Loader, Resource>::load(rid id,
    handle_t fallbackResourceHandle,
    resource_eviction_policy evictionPolicy) -> resource_handle_t
{
    auto handle = id_to_handle(id);
    auto &newHandle = m_handleMap[handle];
    if(newHandle.id != -1) 
    {
        // Already exists
        use_resource(newHandle);
        return newHandle;
    }

    // Bind the ID
    newHandle.id = id;

    // Fill out the cache entry
    auto &desc = m_cache[handle];
    desc.id = id;
    desc.evictionPolicy = evictionPolicy;
    desc.lastTimeUsed = go::performance.engineFrameTimeMS;
    desc.size = 0;
    desc.state = resource_state::ready;

    // Bind fallback-resource handle
    desc.fallbackResourceHandle = fallbackResourceHandle;

    // Try loading the resource
    if(m_loader.load((typename loader_t::manager_t *)this, &desc))
    {
        // Subtract from budget
        m_usedBudget += desc.size;

        // Identity mapping
        newHandle.value = handle;
    }
    else
    {
        GO_LOGFILE_WARNING("Failed to synchronously load resource '%d'.\n", desc.id);

        // Fallback resource mapping
        newHandle.value = desc.fallbackResourceHandle;
    }

    // Apply active indirection
    m_handleIndirections[handle] = newHandle.value;

    return newHandle;
}


template<typename Loader, typename Resource>
auto concurrent_resource_manager<Loader, Resource>::load_fallback_resource(rid id) -> resource_handle_t
{
    auto handle = load(id, (handle_t)id, resource_eviction_policy::permanent);
    if(!handle)
    {
        GO_THROW(go::exception, "Failed to load fallback resource.");
    }

    ++m_fallbackResourceCount;

    return handle;
}


template<typename Loader, typename Resource>
auto concurrent_resource_manager<Loader, Resource>::allocate_fallback_resource(resource_ptr_t *outResource) -> resource_handle_t
{
    auto id = m_fallbackResourceCount;
    auto &newHandle = m_handleMap[id];
    if(newHandle.id != -1)
    {
        GO_THROW(go::exception, "Duplicate fallback resource binding detected.");
    }

    // Bind the ID
    newHandle.id = id;
    newHandle.value = id;

    // Allocate desc
    auto &desc = m_cache[id];
    desc.id = id;
    desc.evictionPolicy = resource_eviction_policy::permanent;
    desc.lastTimeUsed = 0;
    desc.state = resource_state::ready;
    desc.size = 0;
    desc.fallbackResourceHandle = id;
    
    // Identity indirection
    m_handleIndirections[id] = id;

    ++m_fallbackResourceCount;

    *outResource = &desc.resource;

    return newHandle;
}


template<typename Loader, typename Resource>
auto concurrent_resource_manager<Loader, Resource>::load_async(rid id,
    handle_t fallbackResourceHandle,
    resource_eviction_policy evictionPolicy) -> resource_handle_t
{
    auto handle = id_to_handle(id);
    auto &newHandle = m_handleMap[handle];
    if(newHandle.id != -1)
    {
        // Already exists
        use_resource(newHandle);
        return newHandle;
    }
    
    // Bind the ID
    newHandle.id = id;
    newHandle.value = handle;

    // Load resource
    auto &desc = m_cache[handle];
    desc.id = id;
    desc.evictionPolicy = evictionPolicy;
    desc.lastTimeUsed = go::performance.engineFrameTimeMS;
    desc.state = resource_state::loading;
    desc.size = 0;

    // Start out with fallback-resource
    GO_ASSERT(fallbackResourceHandle < (handle_t)m_fallbackResourceCount);
    desc.fallbackResourceHandle = fallbackResourceHandle;

    m_handleIndirections[handle] = fallbackResourceHandle; // Map to fallback resource

    if(!m_loader.load_async((typename loader_t::manager_t *)this, &desc))
    {
        GO_LOGFILE_WARNING("Failed to asynchronously load resource '%d'.\n", desc.id);
    }

    return newHandle;
}


template<typename Loader, typename Resource>
auto concurrent_resource_manager<Loader, Resource>::get(rid id) -> resource_handle_t
{
    return m_handleMap[m_handleIndirections[id_to_handle(id)]];
}


template<typename Loader, typename Resource>
bool concurrent_resource_manager<Loader, Resource>::unload(rid id)
{
    return unload(m_handleMap[id_to_handle(id)]);
}


template<typename Loader, typename Resource>
bool concurrent_resource_manager<Loader, Resource>::unload(const resource_handle_t &handle)
{
    if(handle.value < (handle_t)m_fallbackResourceCount)
    {
        // Don't unload fallback resources
        return false;
    }

    auto &desc = m_cache[handle.value];
    if(desc.evictionPolicy == resource_eviction_policy::default)
    {
        m_loader.unload(&desc.resource);
        desc.state = resource_state::unloaded;
        m_usedBudget -= desc.size;
        desc.size = 0;
        m_handleIndirections[handle] = desc.fallbackResourceHandle; // Route to fallback resource
        return true;
    }
    else
    {
        // Don't unload permanent resources
        return false;
    }
}


template<typename Loader, typename Resource>
size_t concurrent_resource_manager<Loader, Resource>::used_budget_size() const noexcept
{
    return m_usedBudget;
}


template<typename Loader, typename Resource>
size_t concurrent_resource_manager<Loader, Resource>::maximum_budget() const noexcept
{
    return m_budget;
}


template<typename Loader, typename Resource>
auto concurrent_resource_manager<Loader, Resource>::query_resource(handle_t handle) -> resource_ptr_t
{
    return &use_resource(handle)->resource;
}


template<typename Loader, typename Resource>
bool concurrent_resource_manager<Loader, Resource>::is_valid_handle(handle_t handle, rid id)
{
    return handle != invalid_handle_value && // Valid handle?
           handle < m_handleMap.size() && // Valid handle?
           m_cache[handle].id == id; // IDs have to match
}


template<typename Loader, typename Resource>
auto concurrent_resource_manager<Loader, Resource>::use_resource(handle_t handle) -> resource_desc_t *
{
    // Update timestamp
    auto &desc = m_cache[handle];
    auto engineFrameTimeMS = go::performance.engineFrameTimeMS;

    // Already used?
    if(desc.lastTimeUsed < engineFrameTimeMS)
    {
        desc.lastTimeUsed = engineFrameTimeMS;

        // Issue async reload
        if(desc.state == resource_state::unloaded)
        {
            desc.state = resource_state::loading;
            m_loader.reload_async((typename loader_t::manager_t *)this, &desc);
        }
    }

    return &desc;
}


template<typename Loader, typename Resource>
void concurrent_resource_manager<Loader, Resource>::update()
{
    // Bring in async resources
    flush_async_updates();

    // Balance budget
    try_balance_budget();
}


template<typename Loader, typename Resource>
void concurrent_resource_manager<Loader, Resource>::loader_update_resource_success(
    resource_desc_t *desc)
{
    // Queue up for flush
    lock_guard<spin_lock> guard(m_completedRequestsLock);
    m_completedRequests.push_back(desc->id);
}


template<typename Loader, typename Resource>
void concurrent_resource_manager<Loader, Resource>::loader_update_resource_failure(
    resource_desc_t *desc)
{
    auto fullDesc = desc;
    fullDesc->state = resource_state::ready;
    fullDesc->lastTimeUsed = go::performance.engineFrameTimeMS;
    fullDesc->size = 0;
    // Don't update handle indirection: keep pointing to fallback resource
}


template<typename Loader, typename Resource>
void concurrent_resource_manager<Loader, Resource>::flush_async_updates()
{
    auto timeStamp = go::performance.engineFrameTimeMS;
    {
        lock_guard<spin_lock> guard(m_completedRequestsLock);
        for(auto id : m_completedRequests)
        {
            // Add element to cache
            auto handle = id_to_handle(id);
            auto &desc = m_cache[handle]; // NOTE: this is the true handle, indirection points to fallback
            desc.state = resource_state::ready;
            desc.lastTimeUsed = timeStamp;
            m_usedBudget += desc.size;
            m_handleIndirections[handle] = handle; // Identity
        }

        m_completedRequests.clear();
    }
}


template<typename Loader, typename Resource>
void concurrent_resource_manager<Loader, Resource>::try_balance_budget()
{
    if(m_budget > 0 && m_usedBudget > m_budget)
    {
        // TODO: LRU unload
        // TODO: only if resource was not flagged as "permanent"
        
        // LRU-Strategy
        // (1) Unload-candidates must be at least X seconds unused
        // (2) User-specified unload criteria: (a) largest first (b) random
    }
}
