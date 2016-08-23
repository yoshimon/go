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


#include "go/core/exception.h"


// ================================================================================ //
// go::exception member functions
// ================================================================================ //

go::exception::exception(const std::string &whatArg)
	: m_whatArg(whatArg)
{
}


go::exception::exception(const char *whatArg)
	: m_whatArg(whatArg)
{
}


go::exception::~exception()
{
}


const std::string &go::exception::what() const noexcept
{
	return m_whatArg;
}


// ================================================================================ //
// go::os_error member functions
// ================================================================================ //

go::os_error::os_error(const std::string &whatArg, int32_t errorCode)
    : base_class_type(whatArg), m_errorCode(errorCode)
{
}

go::os_error::os_error(const char *whatArg, int32_t errorCode)
    : base_class_type(whatArg), m_errorCode(errorCode)
{
}


int32_t go::os_error::error_code() const noexcept
{
    return m_errorCode;
}


// ================================================================================ //
// go::data_error member functions
// ================================================================================ //

go::data_error::data_error(const std::string &whatArg)
    : base_class_type(whatArg)
{
}

go::data_error::data_error(const char *whatArg)
    : base_class_type(whatArg)
{
}


// ================================================================================ //
// go::identifier_not_found member functions
// ================================================================================ //

go::identifier_not_found::identifier_not_found(const std::string &whatArg)
    : base_class_type(whatArg)
{
}

go::identifier_not_found::identifier_not_found(const char *whatArg)
    : base_class_type(whatArg)
{
}
