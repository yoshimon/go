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
    \file core/concurrent_append_array.h
    \brief Contains a class which allows concurrent appending to an array.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

/*!
    A class, which represents a concurrent, append-only array.

    \tparam Element The element type.
    \tparam MaxNumElementsHint The maximum number of elements.
 */
template<typename Element, int32_t MaxNumElementsHint>
class concurrent_append_array
{
public:
    typedef Element element_type;
    typedef std::vector<element_type> element_vector_t;
public:
    GO_DECLARE_STL_ITERABLE_CUSTOM_END(typename element_vector_t, m_array, m_nextFreeIndex)
public:
    //! Constructor.
    concurrent_append_array() : m_nextFreeIndex(0) { m_array.resize(MaxNumElementsHint); }
public:
    /*!
        Index-access operator.

        \param index The index to access.

        \return The element at the index position.
     */
    Element &operator[](uint32_t index) { GO_ASSERT(index < m_nextFreeIndex); return m_array[index]; }
public:
    /*!
        Returns a pointer to the internal data array.

        \return The internal data array.
     */
    const Element *data() const noexcept { return m_array.data(); }
    //! \overload
    Element *data() noexcept { return m_array.data(); }
    //! Reserves a new element.
    uint32_t increment() noexcept { return InterlockedIncrement(&m_nextFreeIndex) - 1; }
    /*!
        Returns the total number of allocated elements.

        \return The number of allocated elements.
     */
    uint32_t size() const noexcept { return m_nextFreeIndex; }
    /*!
        Returns the maximum number of allocatable elements.

        \return The capacity of the container.
     */
    uint32_t capacity() const noexcept { return m_array.capacity(); }
    /*!
        Clears the array.

        This function does not deallocate any memory.
     */
    void clear() noexcept { m_nextFreeIndex = 0; }
private:
    //! The next free index.
    volatile uint32_t m_nextFreeIndex;
    //! The size of the
    element_vector_t m_array;
};

GO_END_NAMESPACE
