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
    \file core/pool_allocator.h
    \brief Contains a class for fast pool-based memory allocation.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go\core\common.h"
#include "go\core\allocator.h"


GO_BEGIN_NAMESPACE

//! A class which provides fast pool-based memory allocation.
template<typename Element>
class pool_allocator : public allocator_base<Element>
{
    static_assert(sizeof(Element) >= sizeof(void *), "allocator element size too small.");
    GO_DECLARE_NON_COPYABLE(pool_allocator);
public:
    /*!
        Constructs the object and initializes it.

        \param maxCount The size of the memory pool in element count.
        \param alignment The alignment of the data.
     */
    pool_allocator(size_t maxCount, size_t alignment = 0);
    /*!
        Move constructor.

        \param other The object to move.
     */
    pool_allocator(pool_allocator<Element> &&other) noexcept;
    //! \overload
    pool_allocator() noexcept;
    //! Destructor.
    ~pool_allocator();
public:
    Element *allocate() override;
    void release(Element *element) override;
public:
    /*!
        Move-assignment operator.

        \param other The object to move.

        \return The object being moved to.
     */
    pool_allocator<Element> &operator=(pool_allocator<Element> &&other) noexcept;
public:
    /*!
        Returns the allocated memory footprint.
        
        \return Memory footprint in bytes.
     */
    size_t size() const noexcept;
    /*!
        Returns the memory footprint reserved by used slots.

        \return Used memory footprint in bytes.
     */
    size_t used_memory_size() const noexcept;
    /*!
        Returns the number of elements in the list.

        \return Number of elements in the list.
     */
    size_t used_element_count() const noexcept;
    /*!
        Returns the capacity of the list.

        \return Maximum number of elements the list can hold.
     */
    size_t element_capacity() const noexcept;
    //! Clears the pool.
    void clear() noexcept;
    /*!
        Tests whether the pool is exhausted.

        \return If the pool is exhausted, the return value is true. Otherwise, the return value is
        false.
     */
    bool is_exhausted() const noexcept;
    /*!
        Resets the pool allocator.

        \param maxCount The size of the memory pool in element count.
        \param alignment The alignment of the data.
     */
    void reset(size_t maxCount, size_t alignment = 0);
private:
    //! Frees the memory occupied by the allocator.
    void free_memory();
private:
    //! The internal data pointer.
    Element *m_data;
    //! The size of the data pointed to by the internal data pointer.
    size_t m_sizeBytes;
    //! The index at which the next allocation will take place.
    ptrdiff_t m_nextFreeIndex;
    //! The maximum number of elements the pool can hold.
    size_t m_maxElementCount;
    //! The number of used elements.
    size_t m_usedElementCount;
    //! State flag, which indicates whether the internal data pointer points to aligned memory.
    bool m_isDataAligned;
};


#include "pool_allocator_impl.h"

GO_END_NAMESPACE
