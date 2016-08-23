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

#pragma once

#include "gat/defines.h"
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

/*!
    A class, which provides utility functions for working with files.
 */
struct file_util
{
    /*!
        Reads a binary file into memory.

        \param filePath The file path.

        \return The contents of the file.
     */
    static std::vector<uint8_t> read_binary_file(const go::file_path &filePath);
    /*!
        Reads a text file into memory.

        \param filePath The file path.

        \return The contents of the file.
     */
    static std::string read_text_file(const go::file_path &filePath);
    /*!
        Reads a text file into memory.

        \param filePath The file path.

        \return The contents of the file.
     */
    static void read_text_file(const go::file_path &filePath, std::vector<std::string> &lines);
    /*!
        Queries the size of a file.

        \return The size of the specified file.
     */
    static uint64_t query_size(const go::file_path &filePath);
    /*!
        Computes the SHA256 hash value of the specified file.

        \param filePath The file path.

        \return The SHA256 hash.
     */
    static std::string compute_sha256_hash(const void *data, size_t numBytes);
};

GAT_END_NAMESPACE