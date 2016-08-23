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
    \file core/exception.h
    \brief Contains common exception classes.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/defines.h"
#include "go/core/log.h"


GO_BEGIN_NAMESPACE

/*!
    Throws an exception of a given type and writes an entry to the log file.

    \tparam Exception The exception to throw.
    \tparam Args The exception arguments.
    
    \param fileName The current filename.
    \param line The line that raised the exception.
    \param logMsg The message to write to the log file.
    \param args The arguments to forward to the exception constructor.
 */
template<typename Exception, typename... Args>
__declspec(noreturn)
typename std::enable_if<!std::is_default_constructible<Exception>::value, void>::type
throw_exception(const char *fileName, int32_t line, std::string logMsg, Args&&... args)
{
    if(the_logger)
    {
        the_logger->reportf("(%s @ line %d) %s\n", go::log::file | go::log::error, fileName, line, logMsg.c_str());
    }
    throw Exception(std::move(logMsg), std::forward<Args>(args)...);
}


/*!
    Throws an exception of a given type and writes an entry to the log file.

    \tparam Exception The exception to throw.
    
    \param fileName The current filename.
    \param line The line that raised the exception.
    \param logMsg The message to write to the log file.
 */
template<typename Exception>
__declspec(noreturn)
typename std::enable_if<std::is_default_constructible<Exception>::value, void>::type
throw_exception(const char *fileName, int32_t line, const std::string &logMsg)
{
    if(the_logger)
    {
        the_logger->reportf("(%s @ line %d) %s\n", go::log::file | go::log::error, fileName, line, logMsg.c_str());
    }
    throw Exception();
}


//! An exception class, which serves as a base class for custom exceptions thrown by Go.
class GO_API exception
{
public:
    /*!
        Constructor.

        \param whatArg Explanatory string.
     */
    explicit exception(const std::string &whatArg);
    //! \overload
    explicit exception(const char *whatArg);
    //! Destructor.
    virtual ~exception();
public:
    /*!
        Returns the explanatory string, which was provided to the constructor.

        \return The explanatory string, which was provided to the constructor.
     */
    virtual const std::string &what() const noexcept;
private:
    //! The exception message.
    std::string m_whatArg;
};


//! An exception class, which is thrown when the operating system returns an error.
class GO_API os_error : public exception
{
public:
    //! The base class.
    typedef exception base_class_type;
public:
    /*!
        Constructor.

        \param whatArg Explanatory string.
        \param errorCode The error-code. Optional.
     */
    os_error(const std::string &whatArg, int32_t errorCode = 0);
    //! \overload
    os_error(const char *whatArg, int32_t errorCode = 0);
public:
    /*!
        Returns the error-code.

        \return The error-code.
     */
    int32_t error_code() const noexcept;
private:
    //! The error code.
    int32_t m_errorCode;
};


//! An exception class, which is thrown when a piece of data being processed by the engine is ill-formed.
class GO_API data_error : public exception
{
public:
    //! The base class.
    typedef exception base_class_type;
public:
    //! Constructor.
    data_error(const std::string &whatArg);
    //! \overload
    data_error(const char *whatArg);
};


//! An exception class, which is thrown when an identifier is not found.
class GO_API identifier_not_found : public exception
{
public:
    //! The base class.
    typedef exception base_class_type;
public:
    //! Constructor.
    identifier_not_found(const std::string &whatArg);
    //! \overload
    identifier_not_found(const char *whatArg);
};

GO_END_NAMESPACE
