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
    \file filesystem/async.h
    \brief Contains classes for performing asynchronous file I/O.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/design/singleton.h"
#include "go/filesystem/file.h"
#include "go/core/path_utility.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

struct async_io_callback;

//! A structure, which represents an asynchronous I/O request.
struct async_io_request
{
    //! Enumeration of possible request states.
    enum class state_t
    {
        //! The request was successfully completed.
        success = 0,
        //! There was an error.
        failed,
        //! The request was canceled.
        canceled
    };
    //! The offset to read at.
    uint64_t dataOffset;
    //! Number of bytes to read.
    size_t dataSize;
    //! A pointer to the request callback.
    async_io_callback *callback;
    /*!
        The target buffer.

        This will be initialized by the streaming system.
     */
    void *dataBuffer;
    //! The request initiator.
    void *sender;
    //! The user data.
    void *userData;
    //! The request state.
    state_t state;
    //! The filename.
    char fileName[MAX_PATH];
};


//! A structure, which represents a callback handler for asynchronous I/O operations.
struct async_io_callback
{
    /*!
        Callback function, which is invoked when a request was completed.

        \param request A pointer to the request descriptor.

        \return If the function returns true the request will be closed normally.
        Otherwise, all child requests are canceled before the request is closed.
        */
    virtual bool handle_request_completed(const async_io_request *request) = 0;
    /*!
        Callback function, which is invoked when a request was canceled.

        \param request A pointer to the request descriptor.
        */
    virtual void handle_request_canceled(const async_io_request *request) = 0;
    /*!
        Callback function, which is invoked when a request has failed.

        \param request A pointer to the request descriptor.
        */
    virtual void handle_request_failed(const async_io_request *request) = 0;
    /*!
        Callback function, which is invoked when a request was closed.

        \param request A pointer to the request descriptor.
        */
    virtual void handle_request_closed(const async_io_request *request) = 0;
};


//! A class, which provides asynchronous file I/O operations.
class GO_API async_io : public singleton<async_io>
{
public:
    //! Constructor.
    async_io();
    //! Destructor.
    ~async_io();
public:
    //! Shuts the asynchronous manager down.
    void shutdown();
    /*!
        Reads data from a file.

        \param callback The callback function to call when the data is available or an error occurred.
        \param fileName The filename to read from.
        \param numBytes The number of bytes to read.
        \param offset The offset in bytes to read at.
        \param userData Optional user-data to forward to the completion routine.

        \return Returns true, if the asynchronous read-operation was issued. Otherwise false.
     */
    bool read(async_io_callback *callback,
              const file_path &fileName,
              size_t numBytes,
              uint64_t offset,
              void *sender = nullptr,
              void *userData = nullptr);
    //! Rebuilds the file cache.
    void rebuild_file_cache();
};

//! Global asynchronous I/O manager.
extern async_io *the_io_manager;

GO_END_NAMESPACE
