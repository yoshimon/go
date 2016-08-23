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


#include "go/filesystem/async.h"


#include "tbb/tbb.h"
#include "go/core/exception.h"
#include "go/core/pool_allocator.h"
#include "go/core/locks.h"
#include "go/filesystem/system.h"
#include "go/platform/windows.h"


go::async_io *go::the_io_manager = nullptr;


// ================================================================================ //
// Static variables
// ================================================================================ //

// Thread management
static volatile HANDLE s_readerThreadSem;
static volatile HANDLE s_callbackThreadSem;
static volatile bool s_shutdownThreads = false;
static volatile bool s_isRunning = false;


// Request queues
typedef tbb::concurrent_queue<go::async_io_request *> async_io_request_queue;
async_io_request_queue s_readerThreadRequests;
async_io_request_queue s_callbackThreadRequests;


// File cache
std::unordered_map<go::file_path, uint64_t> s_fileCache; // <FileName, Size>


// Request pool
async_io_request_queue s_requestPool;


// Request buffers
async_io_request_queue s_bufferedRequests;


// ================================================================================ //
// Streaming storage
// ================================================================================ //

// Chunked stream memory (100MB)
#define GO_DEFINE_STREAM_MEMORY_POOL_MB(Size) \
    typedef uint8_t _##Size##MB[Size * 1024 * 1024]; \
    go::pool_allocator<_##Size##MB> s_##Size##MBPool; \
    go::spin_lock s_##Size##MBPoolLock; 

#define GO_DEFINE_STREAM_MEMORY_POOL_KB(Size) \
    typedef uint8_t _##Size##KB[Size * 1024]; \
    go::pool_allocator<_##Size##KB> s_##Size##KBPool; \
    go::spin_lock s_##Size##KBPoolLock; 

// Stream pools
GO_DEFINE_STREAM_MEMORY_POOL_MB(32)
GO_DEFINE_STREAM_MEMORY_POOL_MB(16)
GO_DEFINE_STREAM_MEMORY_POOL_MB(8)
GO_DEFINE_STREAM_MEMORY_POOL_MB(4)
GO_DEFINE_STREAM_MEMORY_POOL_MB(2)
GO_DEFINE_STREAM_MEMORY_POOL_MB(1)

GO_DEFINE_STREAM_MEMORY_POOL_KB(512)
GO_DEFINE_STREAM_MEMORY_POOL_KB(256)
GO_DEFINE_STREAM_MEMORY_POOL_KB(128)
GO_DEFINE_STREAM_MEMORY_POOL_KB(64)

#define GO_STREAM_MEMORY_RESET_MB(PoolSize, Count) s_##PoolSize##MBPool.reset(Count)
#define GO_STREAM_MEMORY_RESET_KB(PoolSize, Count) s_##PoolSize##KBPool.reset(Count)

// Alloc
#define GO_STREAM_MEMORY_TRY_ALLOC_MB(PoolSize) \
    if(size <= sizeof(_##PoolSize##MB)) { s_##PoolSize##MBPoolLock.lock(); result = s_##PoolSize##MBPool.allocate(); s_##PoolSize##MBPoolLock.unlock(); }

#define GO_STREAM_MEMORY_TRY_ALLOC_KB(PoolSize) \
    if(size <= sizeof(_##PoolSize##KB)) { s_##PoolSize##KBPoolLock.lock(); result = s_##PoolSize##KBPool.allocate(); s_##PoolSize##KBPoolLock.unlock(); }

// Release
#define GO_STREAM_MEMORY_TRY_RELEASE_MB(PoolSize) \
    if(size <= sizeof(_##PoolSize##MB)) { s_##PoolSize##MBPoolLock.lock(); s_##PoolSize##MBPool.release((_##PoolSize##MB *)buffer); s_##PoolSize##MBPoolLock.unlock(); }

#define GO_STREAM_MEMORY_TRY_RELEASE_KB(PoolSize) \
    if(size <= sizeof(_##PoolSize##KB)) { s_##PoolSize##KBPoolLock.lock(); s_##PoolSize##KBPool.release((_##PoolSize##KB *)buffer); s_##PoolSize##KBPoolLock.unlock(); }


// ================================================================================ //
// Storage utility functions
// ================================================================================ //

static void allocate_streaming_storage()
{
    GO_STREAM_MEMORY_RESET_MB(32, 1);
    GO_STREAM_MEMORY_RESET_MB(16, 2);
    GO_STREAM_MEMORY_RESET_MB(8, 4);
    GO_STREAM_MEMORY_RESET_MB(4, 4);
    GO_STREAM_MEMORY_RESET_MB(2, 4);
    GO_STREAM_MEMORY_RESET_MB(1, 4);

    GO_STREAM_MEMORY_RESET_KB(512, 8);
    GO_STREAM_MEMORY_RESET_KB(256, 8);
    GO_STREAM_MEMORY_RESET_KB(128, 8);
    GO_STREAM_MEMORY_RESET_KB(64, 16);

    // Create request pool
    for(auto i = 0; i < GO_MAX_FILE_IO_REQUESTS; ++i)
    {
        s_requestPool.push(new go::async_io_request());
    }
}


static void free_streaming_memory()
{
    // Destroy request pool
    go::async_io_request *request;
    while(!s_requestPool.empty())
    {
        while(s_requestPool.try_pop(request))
        {
            delete request;
        }
    }
}


static void *allocate_from_streaming_memory(size_t size)
{
    void *result;

    GO_STREAM_MEMORY_TRY_ALLOC_KB(64)
    else GO_STREAM_MEMORY_TRY_ALLOC_KB(128)
    else GO_STREAM_MEMORY_TRY_ALLOC_KB(256)
    else GO_STREAM_MEMORY_TRY_ALLOC_KB(512)
    else GO_STREAM_MEMORY_TRY_ALLOC_MB(1)
    else GO_STREAM_MEMORY_TRY_ALLOC_MB(2)
    else GO_STREAM_MEMORY_TRY_ALLOC_MB(4)
    else GO_STREAM_MEMORY_TRY_ALLOC_MB(8)
    else GO_STREAM_MEMORY_TRY_ALLOC_MB(16)
    else GO_STREAM_MEMORY_TRY_ALLOC_MB(32)
    else GO_THROW(go::exception, "Async I/O request exceeds streaming storage.");

    return result;
}


static void free_from_streaming_memory(void *buffer, size_t size)
{
    GO_STREAM_MEMORY_TRY_RELEASE_KB(64)
    else GO_STREAM_MEMORY_TRY_RELEASE_KB(128)
    else GO_STREAM_MEMORY_TRY_RELEASE_KB(256)
    else GO_STREAM_MEMORY_TRY_RELEASE_KB(512)
    else GO_STREAM_MEMORY_TRY_RELEASE_MB(1)
    else GO_STREAM_MEMORY_TRY_RELEASE_MB(2)
    else GO_STREAM_MEMORY_TRY_RELEASE_MB(4)
    else GO_STREAM_MEMORY_TRY_RELEASE_MB(8)
    else GO_STREAM_MEMORY_TRY_RELEASE_MB(16)
    else GO_STREAM_MEMORY_TRY_RELEASE_MB(32)
    else GO_THROW(go::exception, "Unexpected async I/O chunk size.");
}


static void post_read_request(go::async_io_request *request)
{
    s_readerThreadRequests.push(request);
    ReleaseSemaphore(s_readerThreadSem, 1, NULL);
}


static void buffer_read_request(go::async_io_request *request)
{
    s_bufferedRequests.push(request);
}


static void try_pop_buffered_request()
{
    go::async_io_request *request;
    if(s_bufferedRequests.try_pop(request))
    {
        request->dataBuffer = allocate_from_streaming_memory(request->dataSize);
        if(!request->dataBuffer)
        {
            // Still no memory avilable...
            // So buffer this up again
            s_bufferedRequests.push(request);
        }
        else
        {
            post_read_request(request);
        }
    }
}


// ================================================================================ //
// Reader Thread
// ================================================================================ //

void read_request_data(go::async_io_request *request)
{
    if(request->state == go::async_io_request::state_t::canceled)
    {
        return;
    }

    // Open file 
    go::windows_file_handle file(CreateFileA(request->fileName,
        GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));
    if(file == INVALID_HANDLE_VALUE)
    {
        request->state = go::async_io_request::state_t::failed;
        return;
    }

    // Adjust file pointer
    auto offsetLo = static_cast<LONG>(request->dataOffset);
    auto offsetHi = static_cast<LONG>(request->dataOffset >> (sizeof(LONG) * 8));
    if(SetFilePointer(file, offsetLo, &offsetHi, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        request->state = go::async_io_request::state_t::failed;
        return;
    }

    // Read file
    DWORD bytesRead;
    if(!ReadFile(file, request->dataBuffer, (DWORD)request->dataSize, &bytesRead, NULL))
    {
        request->state = go::async_io_request::state_t::failed;
        return;
    }

    // No partial reads or request data size mismatch (EOF) allowed
    if(bytesRead != request->dataSize)
    {
        request->state = go::async_io_request::state_t::failed;
        return;
    }

    request->state = go::async_io_request::state_t::success;
}


static void reader_thread_proc()
{
    GO_LOGFILE_INFO("Running the AFIO reader thread...\n");

    while(true)
    {
        if(WaitForSingleObject(s_readerThreadSem, INFINITE) != WAIT_OBJECT_0)
        {
            break;
        }

        if(s_shutdownThreads)
        {
            // Make sure that there are not pending requests
            if(s_readerThreadRequests.empty())
            {
                break;
            }
        }

        go::async_io_request *request;
        while(!s_readerThreadRequests.try_pop(request))
        {
            // We HAVE to pop a request here since we decremented the semaphore
        }

        // Read data
        read_request_data(request);

        // Notify callback thread
        s_callbackThreadRequests.push(request);
        ReleaseSemaphore(s_callbackThreadSem, 1, NULL);
    }

    GO_LOGFILE_INFO("Exiting AFIO reader thread.\n");
}


// ================================================================================ //
// Callback Thread
// ================================================================================ //

static void callback_thread_proc()
{
    GO_LOGFILE_INFO("Running the AFIO callback thread...\n");

    while(true)
    {
        // Wait for a request to be completed by the Reader
        if(WaitForSingleObject(s_callbackThreadSem, INFINITE) != WAIT_OBJECT_0)
        {
            break;
        }

        if(s_shutdownThreads)
        {
            // Finish pending requests
            if(s_callbackThreadRequests.empty())
            {
                break;
            }
        }

        // Pop request
        go::async_io_request *request;
        while(!s_callbackThreadRequests.try_pop(request))
        {
            // We HAVE to pop a request here since we decremented the semaphore
        }

        // Notify callback
        switch(request->state)
        {
        case go::async_io_request::state_t::success: request->callback->handle_request_completed(request); break;
        case go::async_io_request::state_t::failed: request->callback->handle_request_failed(request); break;
        case go::async_io_request::state_t::canceled: request->callback->handle_request_canceled(request); break;
        }

        // Close request
        request->callback->handle_request_closed(request);

        // Return buffer to memory pool
        free_from_streaming_memory(request->dataBuffer, request->dataSize);

        // Try pushing a buffered request since we just released a memory block
        try_pop_buffered_request();
    }

    GO_LOGFILE_INFO("Exiting the AFIO callback thread.\n");
}

// Read / Dispatcher thread
std::unique_ptr<tbb::tbb_thread> s_readerThread;
std::unique_ptr<tbb::tbb_thread> s_callbackThread;


// ================================================================================ //
// Thread management
// ================================================================================ //

static void initialize_threads()
{
    // Reader Thread
    s_readerThreadSem = CreateSemaphoreA(NULL, 0, 0x7FFFFFFF, NULL);
    if(s_readerThreadSem == NULL)
    {
        GO_THROW(go::os_error, "Could not initialize reader thread semaphore.");
    }

    // Callback Thread
    s_callbackThreadSem = CreateSemaphoreA(NULL, 0, 0x7FFFFFFF, NULL);
    if(s_callbackThreadSem == NULL)
    {
        GO_THROW(go::os_error, "Could not initialize callback thread semaphore.");
    }

    s_shutdownThreads = false;

    s_readerThread = std::make_unique<tbb::tbb_thread>(reader_thread_proc);
    s_callbackThread = std::make_unique<tbb::tbb_thread>(callback_thread_proc);

    // Run threads
    s_isRunning = true;
}


static void shutdown_threads()
{
    if(s_isRunning)
    {
        // Signal shutdown
        s_isRunning = false;
        s_shutdownThreads = true;

        // by waking up the threads
        if(s_readerThreadSem != NULL)
        {
            ReleaseSemaphore(s_readerThreadSem, 1, NULL);
        }

        if(s_callbackThreadSem != NULL)
        {
            ReleaseSemaphore(s_callbackThreadSem, 1, NULL);
        }

        // Wait for the threads to join
        s_readerThread->join();
        s_readerThread.reset();

        s_callbackThread->join();
        s_callbackThread.reset();

        // Close semaphores
        if(s_readerThreadSem != NULL)
        {
            CloseHandle(s_readerThreadSem);
            s_readerThreadSem = NULL;
        }

        if(s_callbackThreadSem != NULL)
        {
            CloseHandle(s_callbackThreadSem);
            s_callbackThreadSem = NULL;
        }

        s_readerThreadRequests.clear();
        s_callbackThreadRequests.clear();
        s_bufferedRequests.clear();
        
        s_shutdownThreads = false;
    }
}


// ================================================================================ //
// go::async_io
// ================================================================================ //

go::async_io::async_io()
    : singleton<async_io>(the_io_manager)
{
    GO_LOGFILE_INFO("Initializing the AFIO manager...\n");

    allocate_streaming_storage();
    initialize_threads();
    rebuild_file_cache();

    GO_LOGFILE_INFO("The AFIO manager was successfully initialized.\n");
}


go::async_io::~async_io()
{
    shutdown();
}


void go::async_io::shutdown()
{
    if(s_isRunning)
    {
        GO_LOGFILE_INFO("Shutting down the AFIO manager...\n");

        shutdown_threads();
        free_streaming_memory();

        GO_LOGFILE_INFO("The AFIO manager was successfully shut down.\n");
    }
}


void go::async_io::rebuild_file_cache()
{
    go::file_system::enumerate_files(go::file_path(GO_PATH_SYMBOL_DATA, "world\\"), std::regex(""),
        file_system::enum_predicate::no_directories,
        [=](const char *fileName, bool, uint64_t fileSize)
    {
        s_fileCache[fileName] = fileSize;
        return true;
    });
}


bool go::async_io::read(async_io_callback *callback, const file_path &fileName,
    size_t numBytes, uint64_t offset, void *sender, void *userData)
{
    GO_ASSERT(!fileName.is_empty());
    GO_ASSERT(callback);

    // Lookup file size?
    if(numBytes == 0)
    {
        auto it = s_fileCache.find(fileName.c_str());
        if(it != s_fileCache.end())
        {
            numBytes = (size_t)it->second;
        }
        else
        {
            return false;
        }
    }

    GO_ASSERT(numBytes > 0);

    // See if we have a request in the pool to use
    go::async_io_request *request;
    if(!s_requestPool.try_pop(request))
    {
        // No requests in the pool..busy system
        // Allocate new requests (this will get added to the request pool after completion)
        GO_LOGFILE_WARNING("Exceeding expected number of pending I/O requests.\n");
        request = new go::async_io_request();
    }
    
    // Fill out request
    request->dataOffset = offset;
    request->dataSize = numBytes;
    request->dataBuffer = allocate_from_streaming_memory(numBytes);
    request->callback = callback;
    request->sender = sender;
    request->userData = userData;
    memcpy(request->fileName, fileName.c_str(), (fileName.length() + 1) * sizeof(char));

    // Make sure the databuffer was allocated
    if(request->dataBuffer)
    {
        // Forward to reader thread
        post_read_request(request);
    }
    else
    {
        // Out of streaming memory..busy system
        // Buffer read request for later
        buffer_read_request(request);
    }

    return true;
}
