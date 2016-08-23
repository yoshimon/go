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
    \file core/short_string.h
    \brief Contains a class to store short strings.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

/*!
    A class, which stores short strings.

    \tparam Length The maximum length of the string.
 */
template<typename Char, size_t Length, typename Traits = std::char_traits<Char>>
class short_string
{
    static_assert(std::is_same<Char, char>::value, "short_string only supports char at the moment");
public:
    typedef short_string<Char, Length, Traits> this_type;
    typedef Char char_type;
    typedef Traits traits_type;
    typedef size_t size_type;
    typedef void allocator_type;
    typedef Char& reference;
    typedef const Char& const_reference;
public:
    //! Constructor.
    short_string() noexcept;
    //! \overload
    short_string(const short_string &rhs) noexcept;
    //! \overload
    short_string(const char_type *str) noexcept;
    //! \overload
    short_string(const std::string &str) noexcept;
public:
    /*!
        Returns the capacity of the string.

        \return The capacity of the string.
     */
    size_t capacity() const noexcept;
    /*!
        Returns the length of the string.

        \return The length of the string.
     */
    size_t length() const noexcept;
    //! Returns whether the string is empty.
    bool is_empty() const noexcept;
    //! Clears the string.
    void clear() noexcept;
    /*!
        Returns the null-terminated C-string.

        \return The null-terminated C-string.
     */
    const char_type *c_str() const noexcept;
    /*!
        Computes a hash value for the string.

        \return The string hash value.
     */
    size_t hash() noexcept;
    /*!
        Returns a reference to the last character in the string.

        \return The last character in the string.
     */
    const char_type &back() const;
    //! \overload
    char_type &back();
    /*!
        Returns the first character in the string.

        \return The first character in the string.
     */
    const char_type &front() const;
    //! \overload
    char_type &front();
    /*!
        Concat operator.

        \param rhs The right-hand side.

        \return The object being assigned to.
     */
    short_string &operator+=(const short_string &rhs) noexcept;
    //! \overload
    short_string &operator+=(const char_type *rhs) noexcept;
    /*!
        Assignment operator.

        \param rhs The right-hand side.

        \return The object being assigned to.
     */
    short_string &operator=(const short_string &rhs) noexcept;
    //! \overload
    short_string &operator=(const char_type *rhs) noexcept;
    //! \overload
    short_string &operator=(const std::string &rhs) noexcept;
    /*!
        Less-than operator.

        \param rhs The right-hand side.

        \return True, if the RHS is greater. Otherwise false.
     */
    bool operator<(const short_string &rhs) noexcept;
    /*!
        Greater-than operator.

        \param rhs The right-hand side.

        \return True, if the RHS is lesser. Otherwise false.
     */
    bool operator>(const short_string &rhs) noexcept;
    /*!
        Less-than-or-equal operator.

        \param rhs The right-hand side.

        \return True, if the RHS is less-or-equal.
     */
    bool operator<=(const short_string &rhs) noexcept;
    /*!
        Greater-than-or-equal operator.

        \param rhs The right-hand side.

        \return True, if the RHS is greater-or-equal.
     */
    bool operator>=(const short_string &rhs) noexcept;
    /*!
        Implicit cast operator.

        \return The C++-style string.
     */
    operator std::string() const noexcept;
    /*!
        Element-access operator.

        \param index The index to access.

        \return The character at the specified index.
     */
    char_type operator[](size_t index) const;
    /*!
        Searches the string for the first character that matches any of the
        characters specified in its arguments.

        \param c Individual character to be searched for.
        \param pos Position of the first character in the string to be considered in the search.

        \return The clamped substring.
     */
    size_t find_first_of(char_type c, size_t pos = 0) const noexcept;
    /*!
        Compares two strings.

        \param other The string to compare with.

        \return The comparison result (see type-trait).
     */
    int compare(const char_type *other) const noexcept;
    //! \overload
    int compare(const short_string &other) const noexcept;
    //! Updates the length and resets the string hash value.
    void internal_update_length_and_hash();
protected:
    //! The hash value of the string.
    size_t m_hash = 0;
    //! The length of the string.
    size_t m_length;
    //! The string.
    char_type m_string[Length+1];
};

/*!
    Equality-operator.

    \tparam Char The character type.
    \tparam Length The maximum length of the string.
    \param Traits The character traits.

    \param lhs The left-hand side.
    \param rhs The right-hand side.

    \return True, if both strings are identical.
 */
template<typename Char, size_t Length, typename Traits>
inline bool operator==(const go::short_string<Char, Length, Traits> &lhs, const Char *rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline bool operator==(const Char *lhs, const go::short_string<Char, Length, Traits> &rhs) noexcept
{
    return rhs.compare(lhs) == 0;
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline bool operator==(
    const go::short_string<Char, Length, Traits> &lhs,
    const go::short_string<Char, Length, Traits> &rhs) noexcept
{
    return lhs.compare(rhs.c_str()) == 0;
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline bool operator==(
    const std::string &lhs,
    const go::short_string<Char, Length, Traits> &rhs) noexcept
{
    return rhs.compare(lhs.c_str()) == 0;
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline bool operator==(
    const go::short_string<Char, Length, Traits> &lhs,
    const std::string &rhs) noexcept
{
    return lhs.compare(rhs.c_str()) == 0;
}


/*!
    Inequality-operator.

    \tparam Char The character type.
    \tparam Length The maximum length of the string.
    \param Traits The character traits.

    \param lhs The left-hand side.
    \param rhs The right-hand side.

    \return True, if both strings are identical.
 */
 template<typename Char, size_t Length, typename Traits>
inline bool operator!=(
    const go::short_string<Char, Length, Traits> &lhs,
    const Char *rhs) noexcept
{
    return !(lhs == rhs);
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline bool operator!=(
    const Char *lhs,
    const go::short_string<Char, Length, Traits> &rhs) noexcept
{
    return !(lhs == rhs);
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline bool operator!=(
    const go::short_string<Char, Length, Traits> &lhs,
    const go::short_string<Char, Length, Traits> &rhs) noexcept
{
    return !(lhs == rhs);
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline bool operator!=(
    const std::string &lhs,
    const go::short_string<Char, Length, Traits> &rhs) noexcept
{
    return !(lhs == rhs);
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline bool operator!=(
    const go::short_string<Char, Length, Traits> &lhs,
    const std::string &rhs) noexcept
{
    return !(lhs == rhs);
}


/*!
    Concat-operator.

    \tparam Char The character type.
    \tparam Length The maximum length of the string.
    \param Traits The character traits.

    \param lhs The left-hand side.
    \param rhs The right-hand side.

    \return The concatenated string.
 */
template<typename Char, size_t Length, typename Traits>
inline
go::short_string<Char, Length, Traits>
operator+(
    const go::short_string<Char, Length, Traits> &lhs,
    const go::short_string<Char, Length, Traits> &rhs) noexcept
{
    go::short_string<Char, Length, Traits> result(lhs);
    result += rhs;
    return result;
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline
go::short_string<Char, Length, Traits>
operator+(
    const go::short_string<Char, Length, Traits> &lhs,
    const Char *rhs) noexcept
{
    go::short_string<Char, Length, Traits> result(lhs);
    result += rhs;
    return result;
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline
go::short_string<Char, Length, Traits>
operator+(
    const Char *lhs,
    const go::short_string<Char, Length, Traits> &rhs) noexcept
{
    go::short_string<Char, Length, Traits> result(lhs);
    result += rhs;
    return result;
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline
go::short_string<Char, Length, Traits>
operator+(
    const std::string &lhs,
    const go::short_string<Char, Length, Traits> &rhs) noexcept
{
    go::short_string<Char, Length, Traits> result(lhs);
    result += rhs;
    return result;
}


//! \overload
template<typename Char, size_t Length, typename Traits>
inline
go::short_string<Char, Length, Traits>
operator+(
    const go::short_string<Char, Length, Traits> &lhs,
    const std::string &rhs) noexcept
{
    go::short_string<Char, Length, Traits> result(lhs);
    result += rhs;
    return result;
}

typedef short_string<char, 16> name_string;


#include "short_string_impl.h"

GO_END_NAMESPACE

namespace std
{
    template<typename Char, size_t Length, typename Traits>
    struct hash<go::short_string<Char, Length, Traits>>
    {
        typedef go::short_string<Char, Length, Traits> _Kty;
        size_t operator()(const _Kty &k) const
        {
            return go::hash_c_str(k.c_str());
        }
    };
}
