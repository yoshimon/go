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

#include "go/core/path_utility.h"

#include "go/core/string_utility.h"
#include "go/core/exception.h"

#include <Shlwapi.h>
#include <Shlobj.h>
#include <KnownFolders.h>

#define PATHCCH_NO_DEPRECATE
#include <PathCch.h>


// Go Engine reserved folders
static go::file_path s_localPath;
static go::file_path s_roamingPath;
static go::file_path s_applicationPath;
static go::file_path s_dataPath;


// Must match the path ID enum
static go::file_path *s_pathMap[] =
{
    &s_localPath,
    &s_roamingPath,
    &s_applicationPath,
    &s_dataPath
};


// Static cache of path symbol mappings
// Symbol -> Value
static std::unordered_map
<
    std::string,
    std::string
> s_pathSymbols;


// ================================================================================ //
// Helper functions
// ================================================================================ //

static void expand_path_symbols(char *outputBuf, const go::fschar_type *path)
{
    // Copy the path string to a temporary string buffer
    std::string tmpBuf(path);

    // Convert to lower-case
    std::transform(tmpBuf.begin(), tmpBuf.end(), tmpBuf.begin(), ::tolower);

    // Find $()
    size_t posSubStart = 0;
    while(posSubStart != std::string::npos)
    {
        posSubStart = tmpBuf.find("$(", posSubStart);
        if(posSubStart != std::string::npos)
        {
            auto posSubEnd = tmpBuf.find(')', posSubStart);
            auto posSymbolStart = posSubStart + 2;
            auto posSymbolEnd = posSubEnd - posSymbolStart;
            auto symbol = tmpBuf.substr(posSymbolStart, posSymbolEnd);
            tmpBuf.replace(posSubStart, posSubEnd + 1, s_pathSymbols[symbol]);
        }
    }

    auto outputLen = std::min((size_t)MAX_PATH, tmpBuf.length());
    memcpy(outputBuf, tmpBuf.c_str(), sizeof(char) * outputLen);
    outputBuf[outputLen] = '\0';
}


static void update_path_symbol_table()
{
    go::define_path_symbol(GO_PATH_SYMBOL_NAME_APP, s_applicationPath.c_str());
    go::define_path_symbol(GO_PATH_SYMBOL_NAME_LOCAL, s_localPath.c_str());
    go::define_path_symbol(GO_PATH_SYMBOL_NAME_ROAMING, s_roamingPath.c_str());
    go::define_path_symbol(GO_PATH_SYMBOL_NAME_DATA, s_dataPath.c_str());
}


// ================================================================================ //
// go::file_path member functions
// ================================================================================ //

go::file_path::file_path(const char_type *filePath) noexcept
{
    from_string(filePath);
}


go::file_path::file_path(const file_path &other) noexcept
{
    *this = other;
}


go::file_path::file_path(const std::string &other) noexcept
{
    *this = other;
}


go::file_path::file_path(const char_type *basePath, const file_path &fileName) noexcept
    : file_path(file_path(basePath), fileName)
{
}


go::file_path::file_path(const file_path &basePath, const file_path &fileName) noexcept
{
    PathCombine(m_string, basePath.c_str(), fileName.c_str());
    normalize();
}


go::file_path::file_path(directory_id basePathId, const char_type *fileName) noexcept
{
    from_string(basePathId, fileName);
}


go::file_path::file_path(directory_id basePathId, const std::string &fileName) noexcept
{
    from_string(basePathId, fileName.c_str());
}


go::file_path &go::file_path::operator+=(const file_path &other) noexcept
{
    *this += other.c_str();
    return *this;
}


go::file_path &go::file_path::operator+=(const char_type *other) noexcept
{
    decltype(m_string) tmpBuf;
    expand_path_symbols(tmpBuf, other);
    PathCombine(m_string, m_string, tmpBuf);
    normalize();
    return *this;
}


go::file_path &go::file_path::operator=(const file_path &rhs) noexcept
{
    // Plain copy
    m_hash = rhs.m_hash;
    m_length = rhs.m_length;
    memcpy(m_string, rhs.m_string, sizeof(m_string));
    return *this;
}


go::file_path &go::file_path::operator=(const char_type *rhs) noexcept
{
    from_string(rhs);
    return *this;
}


go::file_path &go::file_path::operator=(const std::string &rhs) noexcept
{
    from_string(rhs.c_str());
    return *this;
}


void go::file_path::from_string(const char_type *filePath)
{
    expand_path_symbols(m_string, filePath);
    normalize();
}


void go::file_path::from_string(directory_id basePathId, const char_type *fileName)
{
    // Resolve path ID to actual filepath
    auto &rootPath = *s_pathMap[(int32_t)basePathId];
    *this = rootPath + fileName;
}


void go::file_path::from_string(const char_type *basePath, const char_type *fileName)
{
    from_string(basePath);
    *this += fileName;
}


bool go::file_path::is_directory() const noexcept
{
    // Check if the last character is a path delimiter
    return m_length > 0 ? m_string[m_length - 1] == '\\' : true;
}


auto go::file_path::file_name() const noexcept -> const char_type *
{
    return PathFindFileNameA(m_string);
}


auto go::file_path::file_extension() const noexcept -> const char_type *
{
    return PathFindExtensionA(m_string);
}


void go::file_path::replace_extension(const char_type *extension) noexcept
{
    if(PathRenameExtension(m_string, extension))
    {
        internal_update_length_and_hash();
    }
}


void go::file_path::normalize()
{
    // To lower case
    cstr_to_lower(m_string);

    // Forward to backward slash and update length
    for(auto i = 0; i < _countof(m_string); ++i)
    {
        if(m_string[i] == '\0')
        {
            m_length = i;
            break;
        }
        
        if(m_string[i] == '/')
        {
            m_string[i] = '\\';
        }
    }

    // URL to file path
    if(strstr(m_string, "file:\\\\") == m_string)
    {
        auto newLen = m_length - 7;
        memmove(m_string, &m_string[7], sizeof(fschar_type) * newLen);
        m_length = newLen;
        m_string[newLen] = '\0';
    }

    // Canonicalize the path
    if(m_length > 0)
    {
        decltype(m_string) tmpBuf;
        if(PathCanonicalize(tmpBuf, m_string))
        {
            memcpy(m_string, tmpBuf, sizeof(tmpBuf));
        }
    }

    internal_update_length_and_hash();
}


go::file_path go::file_path::directory_path() const
{
    file_path result(*this);
    
    if(PathRemoveFileSpecA((char *)result.c_str()))
    {
        result.internal_update_length_and_hash();
    }
    
    return result;
}


// ================================================================================ //
// Extern functions
// ================================================================================ //

const go::file_path &go::local_path() noexcept
{
    return s_localPath;
}


const go::file_path &go::roaming_path() noexcept
{
    return s_roamingPath;
}


const go::file_path &go::application_path() noexcept
{
    return s_applicationPath;
}


const go::file_path &go::data_path() noexcept
{
    return s_dataPath;
}


void go::define_path_symbol(const std::string &symbol, std::string val)
{
    // Path symbol restricted symbols: $,(,)
    for(auto c : symbol)
    {
        if(c == '$' || c == '(' || c == ')')
        {
            GO_THROW(std::invalid_argument, "Path symbol contains reserved symbols.");
        }
    }

    // Replace existing symbol
    std::string lowerCaseSymbol = symbol;
    std::string lowerCaseVal = val;
    std::transform(lowerCaseSymbol.begin(), lowerCaseSymbol.end(), lowerCaseSymbol.begin(), ::tolower);
    std::transform(lowerCaseVal.begin(), lowerCaseVal.end(), lowerCaseVal.begin(), ::tolower);
    s_pathSymbols[std::move(lowerCaseSymbol)] = std::move(lowerCaseVal);
}


// ================================================================================ //
// Internal functions
// ================================================================================ //

void go::internal::refresh_path_cache()
{
    // Reserve some space for the symbols
    s_pathSymbols.reserve(64);

    wchar_t *path = nullptr;

    // Local
    if(SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path)))
    {
        auto s = unsafe_wcstombs(path);
        CoTaskMemFree(path);
        s.append("\\" GO_APPLICATION_DIRECTORY_NAME "\\");
        CreateDirectoryA(s.c_str(), nullptr);

        s_localPath.from_string(s.c_str());
    }
    else
    {
        s_localPath.clear();
    }

    // Roaming
    if(SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path)))
    {
        auto s = unsafe_wcstombs(path);
        CoTaskMemFree(path);
        s.append("\\" GO_APPLICATION_DIRECTORY_NAME "\\");
        CreateDirectoryA(s.c_str(), nullptr);
        
        s_roamingPath.from_string(s.c_str());
    }
    else
    {
        s_roamingPath.clear();
    }

    // Application
    char pathBuffer[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, pathBuffer, MAX_PATH);
    if(len > 0)
    {
        for(int32_t i = len - 2; i >= 0; --i)
        {
            if(pathBuffer[i] == '\\')
            {
                pathBuffer[i+1] = '\0';
                len = i + 1;
                break;
            }
        }

        s_applicationPath.from_string(pathBuffer);
    }
    else
    {
        s_applicationPath.clear();
    }

    // Media/Data
    s_dataPath = s_applicationPath + GO_DATA_DIRECTORY_PATH;

    update_path_symbol_table();
}
