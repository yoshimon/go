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
    \file core/string_utility.h
    \brief Contains helper functions for common string operations.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include <DirectXMath.h>


GO_BEGIN_NAMESPACE

//! Splits a string at a specified symbol until it goes out of scope.
struct temporary_string_splitter
{
public:
    /*!
        Constructor.

        \param str The string to split.
        \param splitIndex The index to split the string at.
        \param newChar The new character.
     */
    temporary_string_splitter(char *str, size_t index, char newChar = '\0');
    //! Destructor.
    ~temporary_string_splitter();
public:
    //! Restores the replaced character.
    void restore();
private:
    //! The string pointer.
    char *m_str;
    //! The index.
    size_t m_index;
    //! The backup-character.
    char m_oldChar;
};

/*!
	Safely advances a null-terminated string pointer.

	\param p The pointer to a advance.
 */
GO_API char *advance_str_ptr(char *p);
/*!
    Converts a C-string to lower-case.

    \param buf The C-string buffer.
 */
GO_API void cstr_to_lower(char *buf);
/*!
    Converts a wchar-string to a char-string.

    \param s The input string.

    \return The converted string.
 */
GO_API std::string unsafe_wcstombs(const std::wstring &s);
/*!
    Converts a char-string to a wchar-string.

    \param s The input string.

    \return The converted string.
 */
GO_API std::wstring unsafe_mbstowcs(const std::string &s);
/*!
    Trims a string to remove spaces (left and right).

    \param[in,out] inOut String to trim.
 */
GO_API void string_trim(std::string &inOut);
/*!
    Trims a string to remove spaces (left).

    \param[in,out] inOut String to trim.
 */
GO_API void string_trim_left(std::string &inOut);
/*!
    Trims a string to remove spaces (right).

    \param[in,out] inOut String to trim.
 */
GO_API void string_trim_right(std::string &inOut);
/*!
    Returns the next token in a string.

    \param[out] out Next token in the string.
    \param[in,out] line Input line.
    \param allowQuote Set this to true, to allow quotes (") to be used to group multiple words, separated by white spaces.
 */
GO_API void string_next_token(std::string &out, std::string &line, bool allowQuote);
/*!
    Replaces all characters in a string with a new string.

    \param inOut The string to modify.
    \param searchChar The character to replace.
    \param replaceStr The string to replace with.
 */
GO_API void string_replace_char(std::string &inOut, char searchChar, const std::string &replaceStr);
/*!
    Returns a list of standard token separators.

    \return A list of standard token separators.
 */
GO_API std::string standard_separators();
/*!
    Converts a vector2-type to a string.

    \tparam Vector The vector type.

    \param v The vector to convert.

    \return The string representation.
 */
template<typename Vector>
inline std::string vector2_to_string(const Vector &v)
{
    return std::to_string(v.x) + " " + std::to_string(v.y);
}
/*!
    Converts a vector3-type to a string.

    \tparam Vector The vector type.

    \param v The vector to convert.

    \return The string representation.
 */
template<typename Vector>
inline std::string vector3_to_string(const Vector &v)
{
    return std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z);
}
/*!
    Converts a vector3-type to a string.

    \tparam Vector The vector type.

    \param v The vector to convert.

    \return The string representation.
 */
template<typename Vector>
inline std::string vector4_to_string(const Vector &v)
{
    return std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z) + " " + std::to_string(v.w);
}
/*!
    Converts a string to a vector.

    \tparam Vector The vector type.

    \param in The input string.

    \return The new type object.
 */
template<typename Vector>
inline Vector string_to_vector2l(const char *value)
{
    Vector result;
    char *end;
    result.x = std::strtol(value, &end, 10);
    result.y = std::strtol(advance_str_ptr(end), &end, 10);
    return result;
}
/*!
    Converts a string to a vector.

    \tparam Vector The vector type.

    \param in The input string.

    \return The new type object.
 */
template<typename Vector>
inline Vector string_to_vector2f(const char *value)
{
    Vector result;
    char *end;
    result.x = std::strtof(value, &end);
    result.y = std::strtof(advance_str_ptr(end), &end);
    return result;
}
/*!
    Converts a string to a vector.

    \tparam Vector The vector type.

    \param in The input string.

    \return The new type object.
 */
template<typename Vector, typename Func>
inline Vector string_to_vector2ll(const char *value)
{
    Vector result;
    char *end;
    result.x = std::strtoll(value, &end, 10);
    result.y = std::strtoll(advance_str_ptr(end), &end, 10);
    return result;
}
/*!
    Converts a string to a vector.

    \tparam Vector The vector type.

    \param in The input string.

    \return The new type object.
 */
template<typename Vector>
inline Vector string_to_vector3f(const char *value)
{
    Vector result;
    char *end;
    result.x = std::strtof(value, &end);
    result.y = std::strtof(advance_str_ptr(end), &end);
    result.z = std::strtof(advance_str_ptr(end), &end);
    return result;
}
/*!
    Converts a string to a vector.

    \tparam Vector The vector type.

    \param in The input string.

    \return The new type object.
 */
template<typename Vector>
inline Vector string_to_vector3l(const char *value)
{
    Vector result;
    char *end;
    result.x = std::strtol(value, &end, 10);
    result.y = std::strtol(advance_str_ptr(end), &end, 10);
    result.z = std::strtol(advance_str_ptr(end), &end, 10);
    return result;
}
/*!
    Converts a string to a vector.

    \tparam Vector The vector type.

    \param in The input string.

    \return The new type object.
 */
template<typename Vector>
inline Vector string_to_vector3ll(const char *value)
{
    Vector result;
    char *end;
    result.x = std::strtoll(value, &end, 10);
    result.y = std::strtoll(advance_str_ptr(end), &end, 10);
    result.z = std::strtoll(advance_str_ptr(end), &end, 10);
    return result;
}
/*!
    Converts a string to a vector.

    \tparam Vector The vector type.

    \param in The input string.

    \return The new type object.
 */
template<typename Vector>
inline Vector string_to_vector4f(const char *value)
{
    Vector result;
    char *end;
    result.x = std::strtof(value, &end);
    result.y = std::strtof(advance_str_ptr(end), &end);
    result.z = std::strtof(advance_str_ptr(end), &end);
    result.w = std::strtof(advance_str_ptr(end), &end);
    return result;
}
/*!
    Converts a string to a vector.

    \tparam Vector The vector type.

    \param in The input string.

    \return The new type object.
 */
template<typename Vector>
inline Vector string_to_vector4l(const char *value)
{
    Vector result;
    char *end;
    result.x = std::strtof(value, &end, 10);
    result.y = std::strtof(advance_str_ptr(end), &end, 10);
    result.z = std::strtof(advance_str_ptr(end), &end, 10);
    result.w = std::strtof(advance_str_ptr(end), &end, 10);
    return result;
}
/*!
    Converts a string to a vector.

    \tparam Vector The vector type.

    \param in The input string.

    \return The new type object.
 */
template<typename Vector>
inline Vector string_to_vector4ll(const char *value)
{
    Vector result;
    char *end;
    result.x = std::strtoll(value, &end, 10);
    result.y = std::strtoll(advance_str_ptr(end), &end, 10);
    result.z = std::strtoll(advance_str_ptr(end), &end, 10);
    result.w = std::strtoll(advance_str_ptr(end), &end, 10);
    return result;
}

/*!
    Converts a C-string to a counted enum.

    \tparam T The enum type.
    \param str The C-string to convert.
    \param table The translation table.
    \param outValue The output value.

    \return True, if the conversion was successful. Otherwise false.
 */
template<typename T>
inline bool cstr_to_enum(const char *str, const char *const table, T &outValue)
{
    for(auto i = (T)0; i < T::count; ++i)
    {
        if(!strcmp(str, table[i]))
        {
            outValue = (T)i;
            return true;
        }
    }

    return false;
}


//! \overload
template<typename Enum, typename Table, typename Mapper>
inline bool cstr_to_enum(const char *str, const Table &table, Enum &outValue, const Mapper &translationFunc)
{
    for(size_t i = 0; i < (size_t)Enum::count; ++i)
    {
        if(!strcmp(str, translationFunc(table[i])))
        {
            outValue = (Enum)i;
            return true;
        }
    }

    return false;
}

GO_END_NAMESPACE
