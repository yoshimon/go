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


#include "go/platform/windows.h"


// ================================================================================ //
// Global functions
// ================================================================================ //

void go::throw_if_hresult_failed(const char *fileName, int32_t line, HRESULT hr, std::string logMsg)
{
    if(FAILED(hr))
    {
        if(the_logger)
        {
            the_logger->reportf("(%s @ line %d) Exception message: %s (HRESULT=0x%X).\n", go::log::file | go::log::error, fileName, line, logMsg.c_str(), hr);
        }
        throw hresult_error(std::move(logMsg), hr);
    }
}


// ================================================================================ //
// go::hresult_error member functions
// ================================================================================ //

go::hresult_error::hresult_error(const std::string &whatArg, HRESULT errorCode)
    : base_class_type(whatArg), m_errorCode(errorCode)
{
}


go::hresult_error::hresult_error(const char *whatArg, HRESULT errorCode)
    : base_class_type(whatArg), m_errorCode(errorCode)
{
}


HRESULT go::hresult_error::error_code() const noexcept
{
    return m_errorCode;
}
