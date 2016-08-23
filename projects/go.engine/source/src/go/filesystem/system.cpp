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


#include "go/filesystem/system.h"


#include <filesystem>
#include <regex>


namespace fs = std::tr2::sys;


// ================================================================================ //
// go::file_system member functions
// ================================================================================ //

bool go::file_system::delete_file(const file_path &filePath)
{
    return fs::remove(fs::path(filePath.c_str()));
}


void go::file_system::enumerate_files(const file_path &directoryPath, const std::regex &fileMask,
    enum_predicate predicate, const enum_callback &callback)
{
    auto noDirectories = predicate == enum_predicate::no_directories;
    for(fs::recursive_directory_iterator dir(directoryPath.c_str()), end; dir != end; ++dir)
    {
        auto &p = dir->path();
        auto isDirectory = fs::is_directory(p);
        if(isDirectory && noDirectories)
        {
            continue;
        }

        auto matchesMask = std::regex_search(p.filename().string(), fileMask);
        if(matchesMask && !callback(p.string().c_str(), isDirectory, fs::file_size(p)))
        {
            break;
        }
    }
}
