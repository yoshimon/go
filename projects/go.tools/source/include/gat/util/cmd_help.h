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


GAT_BEGIN_NAMESPACE

/*!
    Returns the index of a command-line parameter.

    \param argc The number of arguments.
    \param argv The argument vector.
    \param parameter The parameter name.

    \return The parameter index or -1.
 */
inline int get_parameter_index(int argc, const _TCHAR *argv[], const _TCHAR *parameter)
{
    for(auto i = 0; i < argc; ++i)
    {
        if(!tstrcmp(argv[i], parameter))
        {
            return i;
        }
    }

    return -1;
}
/*!
    Returns whether a given parameter is set.

    \param argc The number of arguments.
    \param argv The argument vector.
    \param parameter The parameter name.

    \return True, if the parameter is set. Otherwise false.
 */
inline bool is_parameter_set(int argc, const _TCHAR *argv[], const _TCHAR *parameter)
{
    return get_parameter_index(argc, argv, parameter) >= 0;
}

/*!
    Returns the value of a parameter.

    \tparam T The parameter type.

    \param argc The number of arguments.
    \param argv The argument vector.
    \param parameter The parameter name.
    \param defaultVal The default value.

    \return The parameter or default value.
 */
template<typename T>
inline T get_parameter_value(int argc, const _TCHAR *argv[], const _TCHAR *parameter, T defaultVal)
{
    auto i = get_parameter_index(argc, argv, parameter) + 1;
    return (i >= 1 && i < argc) ? argv[i] : defaultVal;
}


/*!
    Returns the value of a parameter.

    \param argc The number of arguments.
    \param argv The argument vector.
    \param parameter The parameter name.
    \param defaultVal The default value.

    \return The parameter or default value.
 */
template<>
inline long get_parameter_value<long>(int argc, const _TCHAR *argv[], const _TCHAR *parameter, long defaultVal)
{
    auto i = get_parameter_index(argc, argv, parameter) + 1;
#ifdef UNICODE
    return (i >= 1 && i < argc) ? _wtol(argv[i]) : defaultVal;
#else
    return (i >= 1 && i < argc) ? std::atol(argv[i]) : defaultVal;
#endif
}


/*!
    Returns the value of a parameter.

    \param argc The number of arguments.
    \param argv The argument vector.
    \param parameter The parameter name.
    \param defaultVal The default value.

    \return The parameter or default value.
 */
template<>
inline int get_parameter_value<int>(int argc, const _TCHAR *argv[], const _TCHAR *parameter, int defaultVal)
{
    auto i = get_parameter_index(argc, argv, parameter) + 1;
#ifdef UNICODE
    return (i >= 1 && i < argc) ? _wtoi(argv[i]) : defaultVal;
#else
    return (i >= 1 && i < argc) ? std::atoi(argv[i]) : defaultVal;
#endif
}


/*!
    Returns the value of a parameter.

    \param argc The number of arguments.
    \param argv The argument vector.
    \param parameter The parameter name.
    \param defaultVal The default value.

    \return The parameter or default value.
 */
template<>
inline float get_parameter_value<float>(int argc, const _TCHAR *argv[], const _TCHAR *parameter, float defaultVal)
{
    auto i = get_parameter_index(argc, argv, parameter) + 1;
#ifdef UNICODE
    return (i >= 1 && i < argc) ? (float)_wtof(argv[i]) : defaultVal;
#else
    return (i >= 1 && i < argc) ? (float)std::atof(argv[i]) : defaultVal;
#endif
}


/*!
    Maps a string to a wide string.

    \param data The string to convert.
    \param len The length of the string.

    \return The wide-string.
 */
inline std::wstring to_wstring(const char *data, size_t len = 0)
{
    len = len == 0 ? strlen(data) + 1 : len;
    std::wstring wc(len, L'#');
    size_t l = 0;
    mbstowcs_s(&l, &wc[0], len, data, _TRUNCATE);
    return wc;
}


/*!
    Maps a string to a wide string.

    \param data The string to convert.
    \param len The length of the string.

    \return The wide-string.
 */
inline std::wstring map_to_wide(const wchar_t *data, size_t len = 0)
{
    return std::wstring(data, len);
}

GAT_END_NAMESPACE