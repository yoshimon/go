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
    Generic exception class.
 */
class tchar_exception : public std::exception
{
public:
    /*!
        Constructor.
     */
    tchar_exception() = default;
    /*!
        \overload
     */
    tchar_exception(const _TCHAR *msg) : std::exception(), m_what(msg) {}
public:
    /*!
        Returns the exception message.

        \return The exception message.
     */
    const _TCHAR *what() { return m_what.c_str(); }
private:
    /*!
        The exception message.
     */
    tstring m_what;
};

/*!
    File-not-found exception.
 */
class tchar_file_not_found : public tchar_exception
{
public:
    /*!
        Constructor.
     */
    tchar_file_not_found() = default;
    /*!
        \overload
     */
    tchar_file_not_found(const _TCHAR *msg) : tchar_exception(msg) {}
};


/*!
    File-not-found exception.
 */
class com_exception : public tchar_exception
{
public:
    /*!
        Constructor.
     */
    com_exception() = default;
    /*!
        \overload
     */
    com_exception(HRESULT hr) : tchar_exception(_com_error(hr).ErrorMessage()) {}
};


typedef std::vector<std::string> string_vector;


/*!
    Throws an exception if a predicate indicates failure.

    \tparam T The exception class.
    \tparam P The predicate type.

    \param pred The predicate to check.
    \param msg The failure message.
 */
template<class T, typename P>
inline void throw_if_not(P pred, const _TCHAR *msg)
{
    if(!pred)
    {
        throw T(msg);
    }
}

GAT_END_NAMESPACE