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
    \file filesystem/system.h
    \brief Contains a class for file-system operations.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/filesystem/file.h"


GO_BEGIN_NAMESPACE

//! A simple file-system abstraction.
class file_system
{
    GO_DECLARE_NON_COPYABLE(file_system)
public:
    //! Enumeration of possible predicates for file enumeration operations.
    enum class enum_predicate
    {
        //! All files will be accepted.
        accept_all,
        //! Directories will be ignored.
        no_directories,
        //! All non-directory entries will be ignored.
        only_directories
    };
    //! Callback function stub for file enumeration.
    typedef std::function<bool(const char *, bool, uint64_t)> enum_callback;
public:
    //! Constructor.
    file_system() = delete;
public:
    /*!
        Deletes a file.

        \param filePath A pointer to the name of the file to delete. If a path name is
        specified, it is interpreted relative to the application. Directory names preceding a file
        name or another directory name should be followed by a single forward slash character (/).
        
        \return If the file was successfully deleted, the return value is true.
        Otherwise the return value is false.
     */
    static GO_API bool delete_file(const file_path &filePath);
    /*!
        Enumerates files in a directory.

        \param directoryPath The search path.
        \param fileMask The file mask.
        \param predicate The predicate to filter the enumeration list with.
        \param callback Callback function, which is invoked for each file.
     */
    static GO_API void enumerate_files(const file_path &directoryPath, const std::regex &fileMask, 
        enum_predicate predicate, const enum_callback &callback);
    /*!
        Returns the file header of a file.

        \param fileName The name of the file to open.

        \return The file header.
     */
    template<typename Header>
    static Header query_file_header(const file_path &fileName);
};


#include "system_impl.h"

GO_END_NAMESPACE
