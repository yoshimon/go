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
    \file core/path_utility.h
    \brief Contains helper functions for dealing with file-paths.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/short_string.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! An enumeration of file locations which are frequently accessed by the engine.
enum class directory_id : int32_t
{
    //! The directory for storing per-user local data.
    local,
    //! The directory for storing per-user roaming data.
    roaming,
    //! The application directory.
    application,
    //! The data directory.
    data
};


typedef short_string<fschar_type, MAX_PATH> basic_file_path;


/*!
    A class, which represents a file path.

    The class normalizes the string representation, i.e. strings that point
    to the same file will always have the same string representation.
 */
class GO_API file_path : public basic_file_path
{
public:
    typedef basic_file_path base_t;
public:
    /*!
        Constructor.

        \param basePathId The base path identifier.
        \param fileName The relative filename.
     */
    file_path(directory_id basePathId, const char_type *fileName) noexcept;
    //! Constructor.
    file_path(directory_id basePathId, const std::string &fileName) noexcept;
    //! \overload
    file_path(const char_type *basePath, const file_path &fileName) noexcept;
    //! \overload
    file_path(const file_path &basePath, const file_path &fileName) noexcept;
    //! \overload
    file_path(const char_type *filePath) noexcept;
    //! \overload
    file_path(const file_path &other) noexcept;
    //! \overload
    file_path(const std::string &other) noexcept;
    //! \overload
    file_path() = default;
public:
    /*!
        Path-combine operator.

        \param other The file-path to combine with.

        \return The combined path.
     */
    file_path &operator+=(const file_path &rhs) noexcept;
    //! \overload
    file_path &operator+=(const char_type *rhs) noexcept;
    /*!
        Assignment-operator.

        \param rhs The string to assign.

        \return The object being assigned to.
     */
    file_path &operator=(const file_path &rhs) noexcept;
    //! \overload
    file_path &operator=(const char_type *rhs) noexcept;
    //! \overload
    file_path &operator=(const std::string &rhs) noexcept;
public:
    /*!
        Builds a file-path from a string representation.

        \param filePath The string representation of a file-path. This will be normalized.
     */
    void from_string(const char_type *filePath);
    //! \overload
    void from_string(directory_id basePathId, const char_type *fileName);
    //! \overload
    void from_string(const char_type *basePath, const char_type *fileName);
    /*!
        Returns whether the file-path points to a folder.

        \return If the file-path points to a folder, the return value is true.
        Otherwise, the return value is false.
     */
    bool is_directory() const noexcept;
    /*!
        Returns the filename of the entity pointed to by the file-path.

        This is equal to the name of the directory for folders.

        \return The filename of the entity pointed to by the file-path.
     */
    const char_type *file_name() const noexcept;
    /*!
        Returns the file extension of the file pointed to by the file-path.

        \return The file extension of the file pointed to by the file-path.
     */
    const char_type *file_extension() const noexcept;
    //! Replaces the file extension.
    void replace_extension(const char_type *extension) noexcept;
    /*!
        Normalizes the path.

        This should not have to be called externally as file paths will always be auto-normalized.
     */
    void normalize();
    /*!
        Returns the directory path of the string.

        \return The directory path of the string.
     */
    file_path directory_path() const;
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
inline bool operator==(const file_path &lhs, const fschar_type *rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}


//! \overload
inline bool operator==(const fschar_type *lhs, const file_path &rhs) noexcept
{
    return rhs.compare(lhs) == 0;
}


//! \overload
inline bool operator==(const file_path &lhs, const file_path &rhs) noexcept
{
    return lhs.compare(rhs.c_str()) == 0;
}


//! \overload
inline bool operator==(const std::string &lhs, const file_path &rhs) noexcept
{
    return rhs.compare(lhs.c_str()) == 0;
}


//! \overload
inline bool operator==(const file_path &lhs, const std::string &rhs) noexcept
{
    return lhs.compare(rhs.c_str()) == 0;
}


/*!
    Inequality-operator.

    \param lhs The left-hand side.
    \param rhs The right-hand side.

    \return True, if both paths are identical.
 */
inline bool operator!=(const file_path &lhs, const fschar_type *rhs) noexcept
{
    return !(lhs == rhs);
}


//! \overload
inline bool operator!=(const fschar_type *lhs, const file_path &rhs) noexcept
{
    return !(lhs == rhs);
}


//! \overload
inline bool operator!=(const file_path &lhs, const file_path &rhs) noexcept
{
    return !(lhs == rhs);
}


//! \overload
inline bool operator!=(const std::string &lhs, const file_path &rhs) noexcept
{
    return !(lhs == rhs);
}


//! \overload
inline bool operator!=(const file_path &lhs, const std::string &rhs) noexcept
{
    return !(lhs == rhs);
}


/*!
    Concat-operator.

    \param lhs The left-hand side.
    \param rhs The right-hand side.

    \return The concatenated string.
 */
inline file_path operator+(const file_path &lhs, const file_path &rhs) noexcept
{
    file_path result(lhs);
    result += rhs;
    return result;
}


//! \overload
inline file_path operator+(const file_path &lhs, const fschar_type *rhs) noexcept
{
    file_path result(lhs);
    result += rhs;
    return result;
}


//! \overload
inline file_path operator+(const fschar_type *lhs, const file_path &rhs) noexcept
{
    file_path result(lhs);
    result += rhs;
    return result;
}


//! \overload
inline file_path operator+(const file_path &lhs, const std::string &rhs) noexcept
{
    file_path result(lhs);
    result += rhs;
    return result;
}


//! \overload
inline file_path operator+(const std::string &lhs, const file_path &rhs) noexcept
{
    file_path result(lhs);
    result += rhs;
    return result;
}


/*!
    Returns the per-user (local) working directory path.

    \return The local working directory path.
 */
GO_API const file_path &local_path() noexcept;
/*!
    Returns the per-user (roaming) working directory path.

    \return The roaming working directory path.
 */
GO_API const file_path &roaming_path() noexcept;
/*!
    Returns the application directory path.

    \return The absolute application directory path.
 */
GO_API const file_path &application_path() noexcept;
/*!
    Returns the data directory path.

    \return The absolute data directory path.
 */
GO_API const file_path &data_path() noexcept;
/*!
    Defines a new or overwrites an existing path symbol.

    \param symbol The symbol to declare.
    \param value The symbol value.
 */
GO_API void define_path_symbol(const std::string &symbol, std::string value);

GO_END_NAMESPACE


GO_BEGIN_INTERNAL_NAMESPACE

/*!
    Updates the path cache.

    This is called once by the engine to initialize the path cache at startup.
 */
void refresh_path_cache();

GO_END_INTERNAL_NAMESPACE


namespace std
{
    template<>
    struct hash<go::file_path>
    {
        typedef go::file_path _Kty;
        size_t operator()(const _Kty &k) const
        {
            return go::hash_c_str(k.c_str());
        }
    };
}
