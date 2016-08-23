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


#include "go/stdafx.h"


#include "go/core/stack_allocator.h"


#include "go/core/exception.h"


// ================================================================================ //
// go::stack_allocator member functions
// ================================================================================ //

go::stack_allocator::stack_allocator(size_t size, size_t alignment)
    : m_data(nullptr), m_marker(0), m_stackSize(size)
{
    m_isDataAligned = alignment > 0;
    if(m_isDataAligned)
    {
        m_data = static_cast<uint8_t *>(_aligned_malloc(size, alignment));
    }
    else
    {
        m_data = static_cast<uint8_t *>(malloc(size));
    }

    if(!m_data)
    {
        GO_THROW(std::bad_alloc, "Could not allocate enough memory.");
    }
}


go::stack_allocator::stack_allocator(stack_allocator &&other) noexcept
{
    *this = std::move(other);
}


go::stack_allocator &go::stack_allocator::operator=(stack_allocator &&other) noexcept
{
    std::swap(m_data, other.m_data);
    std::swap(m_isDataAligned, other.m_isDataAligned);
    m_marker = other.m_marker;
    m_stackSize = other.m_stackSize;
    return *this;
}


go::stack_allocator::~stack_allocator()
{
    if(m_isDataAligned)
    {
        _aligned_free(m_data);
    }
    else
    {
        release(m_data);
    }
}


size_t go::stack_allocator::size() const noexcept
{
    return m_stackSize;
}


go::stack_allocator::marker_t go::stack_allocator::marker() const noexcept
{
    return m_marker;
}


void go::stack_allocator::release(marker_t marker) noexcept
{
    if(marker <= m_marker)
    {
        m_marker = marker;
    }
}


void go::stack_allocator::release(void *data)
{
    GO_ASSERT(reinterpret_cast<uint8_t *>(data) >= m_data && reinterpret_cast<uint8_t *>(data) <= m_data + m_stackSize);
    
    release(reinterpret_cast<size_t>(data) - reinterpret_cast<size_t>(m_data));
}


void go::stack_allocator::clear() noexcept
{
    m_marker = 0;
}


void *go::stack_allocator::allocate_aligned(size_t size, size_t alignment)
{
    GO_ASSERT(alignment > 0 && alignment < 255 && alignment % 2 == 0);

    // Total allocation size
    auto expandedSizeBytes = size + alignment;

    // Allocate unaligned memory
    auto rawAddress = reinterpret_cast<size_t>(allocate(expandedSizeBytes));

    auto mask = alignment - 1;
    auto misalignment = (rawAddress & mask);
    auto adjustment = alignment - misalignment;
    auto alignedAddress = rawAddress + adjustment;

    // Write the amount of adjustment as a prefix to the memory block
    *(reinterpret_cast<uint8_t *>(alignedAddress) - 1) = (uint8_t)adjustment;

    return (void *)alignedAddress;
}


void go::stack_allocator::free_aligned(void *data)
{
    auto bData = reinterpret_cast<uint8_t *>(data);
    auto adjustment = *(bData - 1);
    release(reinterpret_cast<void *>(bData - adjustment));
}


void *go::stack_allocator::allocate(size_t size)
{
    auto oldMarker = m_marker;
    if(oldMarker + size >= m_stackSize)
    {
        return nullptr;
    }

    m_marker = oldMarker + size;
    return &m_data[oldMarker];
}
