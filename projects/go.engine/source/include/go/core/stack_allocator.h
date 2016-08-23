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
    \file core/stack_allocator.h
    \brief Contains a class for fast stack-based memory allocation.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go\core\common.h"
#include "go\core\allocator.h"


GO_BEGIN_NAMESPACE

//! A class, which represents as a single-ended stack allocator.
class GO_API stack_allocator : public aligned_allocator_base<void>
{
    GO_DECLARE_NON_COPYABLE(stack_allocator)
public:
    //! Marks a memory location in the stack allocator.
    typedef size_t marker_t;
public:
    /*!
        Constructs the object and initializes it.

        \param size The size of the stack in bytes.
        \param alignment The alignment for the data block.
     */
    stack_allocator(size_t size, size_t alignment = 0);
    /*!
        Move constructor.

        \param other The object to move.
     */
    stack_allocator(stack_allocator &&other) noexcept;
    //! Destructor.
    ~stack_allocator();
public:
    void *allocate_aligned(size_t size, size_t alignment) override;
    void free_aligned(void *data) override;
    void *allocate(size_t size) override;
    void release(void *data) override;
public:
    /*!
        Move-assignment operator.

        \param other The object to move.

        \return The object being moved to.
     */
    stack_allocator &operator=(stack_allocator &&other) noexcept;
public:
    /*!
        Returns the number of allocated bytes.

        \return The number of allocated bytes.
     */
    size_t size() const noexcept;
    //! \overload
    void release(marker_t marker) noexcept;
    /*!
        Returns the current marker position.
        The stack marker always points to the next free stack slot.
        
        \return The current marker position.
     */
    marker_t marker() const noexcept;
    /*!
        Clears the entire stack.
        Rolls the stack marker back to the stack base.
        This function does not return any allocated memory.
     */
    void clear() noexcept;
private:
    //! The data pointer.
    uint8_t *m_data;
    //! Marks the current allocation position in the stack.
    marker_t m_marker;
    //! The size of the stack in bytes.
    size_t m_stackSize;
    //! A flag, which indicates whether the data pointer is aligned.
    bool m_isDataAligned;
};

GO_END_NAMESPACE
