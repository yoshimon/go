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


#include "go/core/string_utility.h"


#include "go/platform/windows.h"
#include <cctype>


// ================================================================================ //
// go::temporary_string_splitter
// ================================================================================ //

go::temporary_string_splitter::temporary_string_splitter(char *str, size_t index, char newChar)
    : m_str(str), m_index(index)
{
    m_oldChar = str[index];
    str[index] = newChar;
}


go::temporary_string_splitter::~temporary_string_splitter()
{
    restore();
}


void go::temporary_string_splitter::restore()
{
    m_str[m_index] = m_oldChar;
}


// ================================================================================ //
// Extern functions
// ================================================================================ //

char *go::advance_str_ptr(char *p)
{
	if(!p || (*p) == '\0')
	{
		return p;
	}

	return p + 1;
}


void go::cstr_to_lower(char *buf)
{
    if(buf)
    {
        auto i = 0;
        while(buf[i] != '\0')
        {
            buf[i] = std::tolower(buf[i]);
            ++i;
        }
    }
}


std::string go::unsafe_wcstombs(const std::wstring &s)
{
    return std::string(s.begin(), s.end());
}


std::wstring go::unsafe_mbstowcs(const std::string &s)
{
    return std::wstring(s.begin(), s.end());
}


void go::string_trim_left(std::string &inOut)
{
    auto str = inOut.c_str();
    auto c = str;
    {
        const char *s = c + inOut.size();
        while(c < s && isspace(static_cast<unsigned char>(*c)))
        {
            ++c;

        }
    }

    inOut.erase(0, c - str);
}


void go::string_trim_right(std::string &inOut)
{
    auto c = inOut.c_str();
    auto s = c + inOut.size() - 1;
    while (c < s && isspace(static_cast<unsigned char>(*s)))
    {
        --s;
    }

    inOut.erase(s - c + 1);
}


void go::string_trim(std::string &inOut)
{
    auto str = inOut.c_str();
    auto c = str;
    auto s = c + inOut.size() - 1;
    while(c < s && isspace(static_cast<unsigned char>(*c)))
    {
        ++c;
    }

    inOut.erase(0, c - str);

    while (c < s && isspace(static_cast<unsigned char>(*s)))
    {
        --s;
    }

    inOut.erase(s - c + 1);
}


void go::string_next_token(std::string &out, std::string &line, bool allowQuote)
{
    string_trim_left(line);
    out.clear();

    if(line.length() == 0)
    {
        return;
    }

    size_t delStart = 0;
    size_t delEnd;
    size_t tokenStart = 0;
    size_t tokenEnd;

    if(line.at(0) == '"')
    {
        // Quote allowed?
        if(!allowQuote)
        {
            return;
        }

        tokenEnd = line.find('"', 1);
        if(tokenEnd == std::string::npos)
        {
            return; // Invalid quote
        }

        delStart = 0;
        delEnd = tokenEnd + 1;
        --tokenEnd;
        tokenStart = 1;

        out = line.substr(tokenStart, tokenEnd);

        string_trim(out);
    }
    else
    {
        tokenEnd = line.find(" ");
        if(tokenEnd == std::string::npos)
        {
            tokenEnd = line.length();
        }

        delEnd = tokenEnd;
        out = line.substr(tokenStart, tokenEnd);
    }

    line.erase(delStart, delEnd);
}


std::string go::standard_separators()
{
    std::string s;
    s.reserve(32);
    for(char c = 33; c < 48; ++c)
    {
        s += c;
    }
    for(char c = 58; c < 65; ++c)
    {
        s += c;
    }
    for(char c = 91; c < 95; ++c)
    {
        s += c;
    }
    s += 96;
    for(char c = 123; c < 127; ++c)
    {
        s += c;
    }
    return s;
}


void go::string_replace_char(std::string &inOut, char searchChar, const std::string &replaceStr)
{
    for(size_t i = 0; i < inOut.size(); ++i)
    {
        if(inOut[i] == searchChar)
        {
            inOut.replace(i, 1, replaceStr);
            i += replaceStr.size() - 1;
        }
    }
}
