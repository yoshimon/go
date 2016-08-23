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
// optional<typename> member functions
// ================================================================================ //

template<typename Type>
optional<Type>::optional() noexcept
    : m_isInitialized(false)
{
}


template<typename Type>
optional<Type>::optional(Type &other)
    : m_instance(other), m_isInitialized(true)
{
}


template<typename Type>
optional<Type>::optional(const Type &other)
    : m_instance(other), m_isInitialized(true)
{
}


template<typename Type>
optional<Type>::optional(Type &&other)
    : m_instance(std::move(other)), m_isInitialized(true)
{
}


template<typename Type>
Type &optional<Type>::operator=(Type &other)
{
    m_isInitialized = true;
    return m_instance = other;
}


template<typename Type>
Type &optional<Type>::operator=(const Type &other)
{
    m_isInitialized = true;
    return m_instance = other;
}


template<typename Type>
Type &optional<Type>::operator=(Type &&other)
{
    m_isInitialized = true;
    return m_instance = std::move(other);
}


template<typename Type>
bool optional<Type>::operator==(Type &other) const
{
    return m_instance == other;
}


template<typename Type>
bool optional<Type>::operator>(const Type &other) const
{
    return m_instance > other;
}


template<typename Type>
bool optional<Type>::operator<(const Type &other) const
{
    return m_instance < other;
}


template<typename Type>
bool optional<Type>::operator>=(const Type &other) const
{
    return m_instance >= other;
}


template<typename Type>
bool optional<Type>::operator<=(const Type &other) const
{
    return m_instance <= other;
}


template<typename Type>
optional<Type>::operator const Type &() const noexcept
{
    return m_instance;
}


template<typename Type>
optional<Type>::operator Type &() const noexcept
{
    return m_instance;
}


template<typename Type> 
Type *optional<Type>::operator &() noexcept
{
    return &m_instance;
}


template<typename Type>
optional<Type>::operator bool() const noexcept
{
    return m_isInitialized;
}


template<typename Type>
bool optional<Type>::is_initialized() const noexcept
{
    return m_isInitialized;
}


template<typename Type>
void optional<Type>::reset() noexcept
{
    m_isInitialized = false;
}
