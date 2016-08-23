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
    \file core/allocator.h
    \brief Contains a minimalistic base class for custom allocators.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

/*!
    An abstract class for custom allocators.

    \tparam Element The element type.
 */
template<typename Element>
class allocator_base
{
public:
    //! The type of the deleter function.
    typedef std::function<void (void *)> deleter_type;
    //! The element type.
    typedef Element value_type;
    //! The pointer-to-element type.
    typedef Element *pointer;
    //! The pointer-to-const-element type.
    typedef const Element *const_pointer;
    //! The reference-to-element type.
    typedef Element &reference;
    //! The const-reference-to-element type.
    typedef const Element &const_reference;
public:
    //! Destructor.
    virtual ~allocator_base() {}
public:
    /*!
        Allocates a chunk of memory.

        \return A pointer to the new element or nullptr if the allocation failed.
     */
    virtual Element *allocate() = 0;
    /*!
        Frees a previously allocated chunk of memory.

        \param data Pointer to the element to free.
     */
    virtual void release(Element *data) = 0;
    /*!
        Allocates a chunk of reference-counted memory.

        \return If the memory was successfully allocated, the return value is a pointer to the
        memory block. Otherwise, the return value is nullptr.
     */
    std::unique_ptr<Element, deleter_type> allocate_unique();
};


//! Specialization of allocator_base.
template<>
class allocator_base<void>
{
public:
    //! The type of the deleter function.
    typedef std::function<void (void *)> deleter_type;
    //! The element type.
    typedef void value_type;
    //! The pointer-to-element type.
    typedef void *pointer;
    //! The pointer-to-const-element type.
    typedef const void *const_pointer;
public:
    //! Destructor.
    virtual ~allocator_base() {}
public:
    /*!
        Allocates a chunk of memory.

        \param size The number of bytes to allocate.
     */
    virtual void *allocate(size_t size) = 0;
    /*!
        Frees a previously allocated chunk of memory.

        \param data Pointer to the element to free.
     */
    virtual void release(void *data) = 0;
    /*!
        Allocates a chunk of reference-counted memory.

        \param size The number of bytes to allocate.

        \return If the memory was successfully allocated, the return value is a pointer to the
        memory block. Otherwise, the return value is nullptr.
     */
    std::unique_ptr<void, deleter_type> allocate_unique(size_t size);
};


/*!
    An abstract class, which supports aligned allocation.

    \tparam Element The element type.
 */
template<typename Element>
class aligned_allocator_base : public allocator_base<Element>
{
public:
    //! Destructor.
    virtual ~aligned_allocator_base() {}
public:
    /*!
        Allocates a chunk of aligned memory.

        \param size The number of bytes to allocate.
        \param alignment The alignment for the data chunk.

        \return If the memory was successfully allocated, the return value is a pointer to the
        memory block. Otherwise, the return value is nullptr.
     */
    virtual Element *allocate_aligned(size_t size, size_t alignment) = 0;
    /*!
        Frees a chunk of aligned memory.

        \param data The data pointer.
     */
    virtual void free_aligned(void *data) = 0;
    /*!
        Allocates a chunk of aligned, reference-counted memory.

        \param size The number of bytes to allocate.
        \param alignment The alignment for the data chunk.

        \return If the memory was successfully allocated, the return value is a pointer to the
        memory block. Otherwise, the return value is nullptr.
     */
    std::unique_ptr<Element, typename allocator_base<Element>::deleter_type>
    allocate_aligned_unique(size_t size, size_t alignment);
};


#include "allocator_impl.h"

GO_END_NAMESPACE
