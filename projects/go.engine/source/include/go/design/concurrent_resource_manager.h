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
    \file design/concurrent_resource_manager.h
    \brief Contains a class for managing resources, that should only be loaded once.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/locks.h"
#include "go/core/performance_counter.h"
#include "go/core/exception.h"


GO_BEGIN_NAMESPACE

/*!
    A class, which represents a resource handle.

    \tparam Resource The resource class.
 */
template<typename ResourceManager>
class resource_handle
{
public:
    typedef resource_handle<ResourceManager> this_t;
    typedef ResourceManager resource_manager_t;
    typedef ResourceManager *resource_manager_ptr_t;
    typedef typename resource_manager_t::resource_t resource_t;
    typedef resource_t *resource_ptr_t;
public:
    /*!
        Validity-operator.

        \return True, if the handle is invalid. Otherwise false.
     */
    bool operator!() const noexcept;
    /*!
        Equality-comparison operator.

        \return True, if the two handles point to the same resource. Otherwise false.
     */
    bool operator==(this_t &other) const noexcept;
    /*!
        Inequality-comparison operator.

        \return True, if the two handles do not point to the same resource. Otherwise false.
     */
    bool operator!=(this_t &other) const noexcept;
    /*!
        Query-resource operator.

        \remarks This pointer should not be cached across multiple frames. It is merely intended
        to avoid querying the handle multiple times when performing a series of operations.

        \return Returns the resource-pointer behind the handle.
     */
    resource_ptr_t operator*() const;
    /*!
        Dereference-operator.

        \return The resource pointer.
     */
    resource_ptr_t operator->();
    //! \overload
    const resource_ptr_t operator->() const;
    /*!
        Implicit-type conversion to handle value.

        \return The handle value.
     */
    operator handle_t() const noexcept;
    /*!
        Validity-operator.

        \return True, if the handle is valid. Otherwise false.
     */
    operator bool() const noexcept;
    //! The handle value.
    handle_t value = invalid_handle_value;
    //! The resource ID.
    rid id = -1;
    //! The resource manager.
    resource_manager_ptr_t owner = nullptr;
};


//! Enumeration of possible resource states.
enum class resource_state
{
    //! The resource is ready.
    ready,
    //! The resource is being loaded.
    loading,
    //! The resource was unloaded.
    unloaded
};


//! Enumeration of possible cache-evicition policies for a resource.
enum class resource_eviction_policy
{
    //! The resource can be unloaded if the budget exceeds the limit.
    default,
    //! The resource will not be unloaded.
    permanent
};


/*!
    A structure, which represents a single resource-cache-entry.
 
    \tparam Resource The resource type.
 */
template<typename Resource>
struct resource_desc
{
    GO_DECLARE_NON_COPYABLE(resource_desc)
    typedef Resource resource_t;
    //! Constructor.
    resource_desc() = default;
    //! The resource.
    resource_t resource;
    //! The size of the resource in bytes.
    size_t size;
    //! The resource ID.
    rid id;
    //! The resource state.
    resource_state state;
    //! Specifies whether the resource can be paged.
    resource_eviction_policy evictionPolicy;
    //! The last game-timestamp when the resource was used.
    uint64_t lastTimeUsed;
    //! The handle to the fallback resource.
    handle_t fallbackResourceHandle;
};


/*!
    A base class for resource loaders.

    \tparam Resource The resource type.
    \tparam Manager The resource manager type.
 */
template<typename Resource, typename Manager>
struct concurrent_resource_loader
{
    typedef Resource resource_t;
    typedef resource_t *resource_ptr_t;
    typedef Manager manager_t;
    typedef manager_t *manager_ptr_t;
    typedef resource_desc<resource_t> resource_desc_t;
    typedef resource_desc_t *resource_desc_ptr_t;
    /*!
        Loads a resource from file.

        \param mgr The manager class.
        \param desc The resource descriptor.
     */
    virtual bool load(manager_ptr_t mgr, resource_desc_ptr_t desc) = 0;
    /*!
        Loads a resource from file.

        \param mgr The manager class.
        \param desc The resource descriptor.
     */
    virtual bool load_async(manager_ptr_t mgr, resource_desc_ptr_t desc) = 0;
    /*!
        Loads a resource from file.

        \param mgr The manager class.
        \param desc The resource descriptor.
     */
    virtual bool reload_async(manager_ptr_t mgr, resource_desc_ptr_t desc) = 0;
    /*!
        Unloads an existing resource.

        \param resource The resource to unload.
     */
    virtual void unload(resource_ptr_t resource) = 0;
};


/*!
    A base class for implementing a resource manager.

    \remarks A synchronous-only resource manager does not need to support asynchronous loading or provide a fallback resource.
    However, it has to throw an exception when a synchronous load fails.

    \tparam Loader The resource loader type.
    \tparam Resource The resource type.
 */
template<typename Loader, typename Resource = typename Loader::resource_t>
class concurrent_resource_manager
{
    GO_DECLARE_NON_COPYABLE(concurrent_resource_manager)
public:
    typedef Loader loader_t;
    typedef Resource resource_t;
    typedef resource_t *resource_ptr_t;
    typedef resource_handle<concurrent_resource_manager<Loader, Resource>> resource_handle_t;
    typedef resource_desc<resource_t> resource_desc_t;
public:
    /*!
        Constructor.

        \param maxCount The maximum number of resources to manage.
        \param fallbackResourceCount The number of fallback resources.
        \param budgetSize The resource budget in bytes. If set to zero, there is no allocation budget.
     */
    concurrent_resource_manager(size_t maxCount, size_t fallbackResourceCount, size_t budgetSize = 0);
    //! Destructor.
    virtual ~concurrent_resource_manager();
public:
    /*!
        Resource-lookup operator.

        \param id The resource ID.

        \return If the resource was found, the return value is a pointer to that resource.
        Otherwise, the return value is nullptr.
     */
    resource_handle_t operator[](rid id);
public:
    /*!
        Loads a resource synchronously.

        \tparam Args The argument types to forward to the loader.

        \param id The resource ID.
        \param fallbackResourceHandle The fallback resource to use.
        \param evictionPolicy The eviction policy for the resource.

        \return If the resource was successfully loaded, the return value is a handle to that resource.
        Otherwise, the return value is an invalid handle value.
     */
    resource_handle_t load(
        rid id,
        handle_t fallbackResourceHandle = 0,
        resource_eviction_policy evictionPolicy = resource_eviction_policy::default);
    /*!
        Loads a resource asynchronously.

        \remarks Not every resource manager supports asynchronous resources.

        \tparam Args The argument types to forward to the loader.
        
        \param id The resource ID.
        \param fallbackResourceHandle The fallback resource to use.
        \param evicitionPolicy The evicition policy for the resource.

        \return If the resource was successfully loaded, the return value is a handle to that resource.
        Otherwise, the return value is an invalid handle value.
     */
    resource_handle_t load_async(
        rid id,
        handle_t fallbackResourceHandle = 0,
        resource_eviction_policy evictionPolicy = resource_eviction_policy::default);
    /*!
        Returns a resource using its load key.
        
        \param id The resource ID.

        \return If the resource was found, the return value is a pointer to that resource.
        Otherwise, the return value is nullptr.
     */
    resource_handle_t get(rid id);
    /*!
        Unloads a resource.

        \remarks This will not invalidate any handles, but replace the resource with a default resource.
        
        \param id The resource ID.

        \return True, if the resource was unloaded. Otherwise false.
     */
    bool unload(rid id);
    //! \overload
    bool unload(const resource_handle_t &handle);
    /*!
        Returns the number of allocated bytes.

        \return The number of allocated bytes.
     */
    size_t used_budget_size() const noexcept;
    /*!
        Returns the maximum spendable budget.

        \return The maximum spendable budget.
     */
    size_t maximum_budget() const noexcept;
    /*!
        Returns a resource pointer by dereferencing a handle.

        \param handle The resource handle.
        
        \return The resource pointer.
     */
    resource_ptr_t query_resource(handle_t handle);
    /*!
        Returns whether a given handle is valid.

        \param handle The handle value.
        \param id The resource ID.
     */
    bool is_valid_handle(handle_t handle, rid id);
    /*!
        Updates the resource manager.

        This will apply all pending asynchronous load operations as well as balance the budget by potentially unloading resources.
     */
    virtual void update();
    /*!
        Updates a pending asynchronous resource.

        Internal use only.

        \param loadDesc The load descriptor.
     */
    void loader_update_resource_success(resource_desc_t *loadDesc);
    /*!
        Updates a pending asynchronous resource.

        Internal use only.

        \param loadDesc The load descriptor.
     */
    void loader_update_resource_failure(resource_desc_t *loadDesc);
protected:
    /*!
        Loads a fallback resource synchronously.

        Fallback resources will not be unloaded by the cache.

        \param id The resource ID.

        \return If the resource was successfully loaded, the return value is a handle to that resource.
        Otherwise, the return value is an invalid handle value.
     */
    resource_handle_t load_fallback_resource(rid id);
    /*!
        Binds a new fallback resource.

        Fallback resources will not be unloaded by the cache.

        \param[out] outResource The output resource pointer.

        \return The resource handle.
     */
    resource_handle_t allocate_fallback_resource(resource_ptr_t *outResource);
private:
    /*!
        Maps a resource ID to an effective ID.

        \param id The resource ID.

        \return The effective ID.
     */
    rid id_to_handle(rid id) const;
private:
    typedef std::vector<resource_handle_t> handle_vector_t;
    typedef resource_desc_t *resource_vector_t;
    typedef std::vector<handle_t> handle_value_vector_t;
    typedef std::vector<rid> rid_vector_t;
private:
    //! Flushes all asynchronous updates.
    void flush_async_updates();
    //! Re-balances the budget.
    void try_balance_budget();
    /*!
        Signals a re-load operation of a resource if it was unloaded.

        \param handle The handle to the resource to reload.
     */
    resource_desc_t *use_resource(handle_t handle);
private:
    //! Maps load-keys to resource handles.
    handle_vector_t m_handleMap;
    //! The resource cache.
    resource_vector_t m_cache;
    //! Maps handles to cache slots.
    handle_value_vector_t m_handleIndirections;
    //! Completed async-requests.
    rid_vector_t m_completedRequests;
    //! Spin-lock for the completed requests buffer.
    spin_lock m_completedRequestsLock;
    //! The resource budget.
    size_t m_budget;
    //! The current resource allocation size.
    size_t m_usedBudget;
    //! Number of fallback resources.
    rid m_fallbackResourceCount;
    //! The resource loader to use.
    loader_t m_loader;
};


#include "concurrent_resource_manager_impl.h"

GO_END_NAMESPACE
