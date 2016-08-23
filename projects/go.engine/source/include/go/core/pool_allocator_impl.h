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
// go::pool_allocator member functions
// ================================================================================ //

template<typename Element>
pool_allocator<Element>::pool_allocator(size_t maxCount, size_t alignment)
    : m_data(nullptr)
{
    reset(maxCount, alignment);
}


template<typename Element>
pool_allocator<Element>::pool_allocator(pool_allocator<Element> &&other) noexcept
{
    *this = std::move(other);
}


template<typename Element>
pool_allocator<Element>::pool_allocator() noexcept
: m_data(nullptr), m_maxElementCount(0), m_sizeBytes(0), m_usedElementCount(0)
{
}


template<typename Element>
pool_allocator<Element> &pool_allocator<Element>::operator=(pool_allocator<Element> &&other) noexcept
{
    std::swap(m_data, other.m_data);
    std::swap(m_isDataAligned, other.m_isDataAligned);
    m_sizeBytes = other.m_sizeBytes;
    m_nextFreeIndex = other.m_nextFreeIndex;
    m_maxElementCount = other.m_maxElementCount;
    m_usedElementCount = other.m_usedElementCount;
    return *this;
}


template<typename Element>
pool_allocator<Element>::~pool_allocator()
{
    free_memory();
}


template<typename Element>
size_t pool_allocator<Element>::size() const noexcept
{
    return m_sizeBytes;
}


template<typename Element>
size_t pool_allocator<Element>::used_memory_size() const noexcept
{
    return sizeof(Element) * m_usedElementCount;
}


template<typename Element>
size_t pool_allocator<Element>::used_element_count() const noexcept
{
    return m_usedElementCount;
}


template<typename Element>
size_t pool_allocator<Element>::element_capacity() const noexcept
{
    return m_maxElementCount;
}


template<typename Element>
Element *pool_allocator<Element>::allocate()
{
    if(is_exhausted())
    {
        return nullptr;
    }

    ++m_usedElementCount;
    auto element = m_data + m_nextFreeIndex;
    m_nextFreeIndex = *reinterpret_cast<size_t *>(element);

    return element;
}


template<typename Element>
void pool_allocator<Element>::release(Element *element)
{
    if(element)
    {
        *reinterpret_cast<size_t *>(element) = m_nextFreeIndex;
        m_nextFreeIndex = element - m_data; // Index of element
        --m_usedElementCount;
    }
}


template<typename T>
void pool_allocator<T>::clear() noexcept
{
    auto dataPtr = m_data;
    for(size_t i = 0, end = m_maxElementCount; i < end; ++i, ++dataPtr)
    {
        *reinterpret_cast<size_t *>(dataPtr) = i + 1;
    }

    m_nextFreeIndex = 0;
    m_usedElementCount = 0;
}


template<typename Element>
bool pool_allocator<Element>::is_exhausted() const noexcept
{
    return m_usedElementCount == m_maxElementCount;
}


template<typename Element>
void pool_allocator<Element>::reset(size_t maxCount, size_t alignment)
{
    GO_ASSERT(maxCount > 0);

    free_memory();
    
    m_maxElementCount = maxCount;
    m_isDataAligned = alignment > 0;
    m_sizeBytes = maxCount * sizeof(Element);
    if(m_isDataAligned)
    {
        m_data = reinterpret_cast<Element *>(_aligned_malloc(m_sizeBytes, alignment));
    }
    else
    {
        m_data = reinterpret_cast<Element *>(malloc(m_sizeBytes));
    }

    if(!m_data)
    {
        GO_THROW(std::bad_alloc, "Could not allocate enough memory for pool allocator.");
    }

    clear();
}


template<typename Element>
void pool_allocator<Element>::free_memory()
{
    if(m_isDataAligned)
    {
        _aligned_free(m_data);
    }
    else
    {
        free(m_data);
    }
}
