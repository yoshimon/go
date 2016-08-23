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

#include "gat/util/file_util.h"


#include "gat/util/exception.h"
#include "gat/hashlib/sha256.h"


//===========================================================================
// gat::file_util
//===========================================================================

std::vector<uint8_t> gat::file_util::read_binary_file(const go::file_path &filePath)
{
    std::ifstream t(filePath, std::ios::binary);
    if(!t.is_open())
    {
        throw tchar_file_not_found(filePath.c_str());
    }

    return std::vector<uint8_t>((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}


std::string gat::file_util::read_text_file(const go::file_path &filePath)
{
    std::ifstream t(filePath);
    if(!t.is_open())
    {
        throw tchar_file_not_found(filePath.c_str());
    }

    return std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}


void gat::file_util::read_text_file(const go::file_path &filePath, std::vector<std::string> &lines)
{
    auto txt = file_util::read_text_file(filePath);
    std::istringstream ss(txt);
    std::string line;
    while(!ss.eof())
    {
        std::getline(ss, line);
        lines.push_back(line);
    }
}


uint64_t gat::file_util::query_size(const go::file_path &filePath)
{
    std::ifstream in(filePath, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}


std::string gat::file_util::compute_sha256_hash(const void *data, size_t numBytes)
{
    SHA256 result;
    return result(data, numBytes);
}