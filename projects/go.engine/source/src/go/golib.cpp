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

// Go
#include "go/golib.h"

// Misc headers
#include "tbb/tbb.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/tbbmalloc_proxy.h"

#include <dbghelp.h>
#include <strsafe.h>


// MiniDumpWriteDump stub
typedef BOOL (WINAPI *Stub_MiniDumpWriteDump)
(
    HANDLE hProcess,
    DWORD ProcessId,
    HANDLE hFile,
    MINIDUMP_TYPE DumpType,
    PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    PMINIDUMP_CALLBACK_INFORMATION CallbackParam
);


// dbghelp.dll handle
static HMODULE g_hDbgHelp = NULL;
// MiniDumpWriteDump address
static Stub_MiniDumpWriteDump g_pMiniDumpWriteDump = NULL;
// Exception handler handle
static void *g_hOnHandleException = NULL;

// Runtime environment status flag
static bool g_runtimeEnvironmentInitialized = false; 

// Global TBB task scheduler
tbb::task_scheduler_init *the_task_scheduler = nullptr;


// ============================================================================== //
// Exception handler functions
// ============================================================================== //

static LONG WINAPI OnHandleException(_EXCEPTION_POINTERS *pExceptionInfo)
{
    if(pExceptionInfo->ExceptionRecord->ExceptionCode < 0x80000000)
    {
        // Just a warning, no real exception
        /*
        GO_LOGFILE_ERROR
        (
            "Non-critical exception thrown (code: %#x).\n",
            pExceptionInfo->ExceptionRecord->ExceptionCode
        );
         */
        return EXCEPTION_CONTINUE_SEARCH;
    }

    // Create crash dump folder
    GO file_path path(GO directory_id::application, "crash");
    CreateDirectoryA(path.c_str(), nullptr);

    // Generate crash-dump timestamp
    SYSTEMTIME time;
    GetLocalTime(&time);

    // Build file path
    char fileName[MAX_PATH];
    StringCchPrintfA
    (
        fileName,
        MAX_PATH,
        "%s\\%d-%d-%d_%dh%dm%ds.dmp",
        path.c_str(), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond
    );

    // Create dump file
    HANDLE hFile = CreateFileA(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
    if(hFile != INVALID_HANDLE_VALUE)
    {
        {
            // Create MiniDump
            MINIDUMP_EXCEPTION_INFORMATION mdei;
            mdei.ThreadId = GetCurrentThreadId(); 
            mdei.ExceptionPointers = pExceptionInfo;
            mdei.ClientPointers = TRUE;

            g_pMiniDumpWriteDump
            (
                GetCurrentProcess(), GetCurrentProcessId(),
                hFile, MiniDumpWithThreadInfo, &mdei, NULL, NULL
            );
        }

        CloseHandle(hFile);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}


static void RegisterUnhandledExceptionFilter()
{
    if(g_hDbgHelp == NULL)
    {
        g_hDbgHelp = LoadLibraryA("dbghelp.dll");
        if(g_hDbgHelp != NULL)
        {
            g_pMiniDumpWriteDump = (Stub_MiniDumpWriteDump)GetProcAddress(g_hDbgHelp, "MiniDumpWriteDump");
            g_hOnHandleException = AddVectoredExceptionHandler(0, OnHandleException);
        }
    }
}


static void UnregisterUnhandledExceptionFilter()
{
    if(g_hDbgHelp)
    {
        RemoveVectoredExceptionHandler(g_hOnHandleException);
        g_pMiniDumpWriteDump = NULL;
        g_hDbgHelp = NULL;
    }
}


// ============================================================================== //
// Initialization functions
// ============================================================================== //

static void log_header()
{
    GO_LOGFILE_PLAIN
    (
        "****************************************************************\n"
        "                  Go API Log File [%s]\n"
        "****************************************************************\n\n",
        __DATE__
    );
}

static void log_machine_info()
{
    GO the_platform_info = new GO platform_info();

    GO_LOGFILE_PLAIN
    (
        "/--------------------------------------------------------------\\\n"
        "                      Platform Information\n"
        " --------------------------------------------------------------\n"
    );

    // CPU
    {
        auto &info = GO the_platform_info->cpu();
        GO_LOGFILE_PLAIN
        (
            "+ CPU\n"
            "  - Architecture: %d-Bit\n"
            "  - Caption: %s\n"
            "  - Current clock speed: %u MHz\n"
            "  - L2 cache size: %u KB\n"
            "  - L2 cache speed: %u MHz\n"
            "  - L3 cache size: %u KB\n"
            "  - L3 cache speed: %u MHz\n"
            "  - Maximum clock speed: %u MHz\n"
            "  - Number of cores: %u\n"
            "  - Number of logical processors: %u\n",
            info.dataWidth,
            info.caption.c_str(),
            info.currentClockSpeed,
            info.l2CacheSize,
            info.l2CacheSpeed,
            info.l3CacheSize,
            info.l3CacheSpeed,
            info.maxClockSpeed,
            info.numberOfCores,
            info.numberOfLogicalProcessors
        );
    }

    // Memory
    {
        auto &info = GO the_platform_info->memory();
        GO_LOGFILE_PLAIN
        (
            "+ Memory\n"
            "  - Capacity: %llu MB\n"
            "  - Number of modules: %u\n",
            info.capacity / (1024 * 1024),
            info.numberOfModules
        );
    }

    // OS
    {
        auto &info = GO the_platform_info->os();
        GO_LOGFILE_PLAIN
        (
            "+ OS\n"
            "  - Architecture: %s\n"
            "  - Caption: %s\n"
            "  - Version: %s\n",
            info.architecture.c_str(),
            info.caption.c_str(),
            info.version.c_str()
        );
    }

    // Video Controller
    {
        uint32_t i = 0;
        for(auto &info : GO the_platform_info->video())
        {
            GO_LOGFILE_PLAIN
            (
                "+ Video Controller[%u]\n"
                "  - Caption: %s\n"
                "  - Current BPP: %u\n"
                "  - Current Refresh Rate (Max): %u Hz (%u Hz)\n"
                "  - Current Resolution: %ux%upx\n"
                "  - Name: %s\n",
                i,
                info.caption.c_str(),
                info.currentBitsPerPixel,
                info.currentRefreshRate, info.maxRefreshRate,
                info.currentHorizontalResolution, info.currentVerticalResolution,
                info.name.c_str()
            );
            ++i;
        }
    }

    GO_LOGFILE_PLAIN
    (
        "\\--------------------------------------------------------------/\n\n"
    );
}


static void initialize_runtime_environment()
{
    if(!g_runtimeEnvironmentInitialized)
    {
        // Initialize random number generator
        std::srand((unsigned int)time(nullptr));

        g_runtimeEnvironmentInitialized = true;

        RegisterUnhandledExceptionFilter();

        GO_INTERNAL refresh_path_cache();

        // Create global Logger
        new GO log();

        log_header();
        log_machine_info();

        GO_LOGFILE_INFO("Initializing Go...\n");

        // Initialize the task scheduler
        the_task_scheduler = new tbb::task_scheduler_init();

        // Create global environment-manager
        new GO environment_manager();

        // Register ALL environment variables
        GO_INTERNAL initialize_all_environment_variables();

        // and load the default configuration file
        GO the_environment->read_file();

        // Initialize global (hardware) timer
        GO timer::query_hardware_timer_frequency();

        // Create global file-manager
        new GO async_io();
    }
}


static void shutdown_runtime_environment()
{
    if(g_runtimeEnvironmentInitialized)
    {
        GO the_environment->write_file();

        delete GO the_environment;
        delete GO the_io_manager;

        GO_LOGFILE_INFO("Go was successfully shut down.\n");

        delete GO the_logger;
        delete GO the_platform_info;

        UnregisterUnhandledExceptionFilter();

        g_runtimeEnvironmentInitialized = false;
    }
}


static void initialize_gpu_display(go::gfx_display_configuration *driverConfig)
{
    if(driverConfig)
    {
        // Create global display driver
        new GO gfx_display(*driverConfig);
    }
}


static void initialize_input()
{
    // System Input
    new GO system_input_driver();

    // Input Dispatcher
    // NOTE: must be initialized after the input drivers
    new GO input_dispatcher();
}


// ============================================================================== //
// Exports
// ============================================================================== //

void go::initialize(go::runtime_configuration *runtimeConfig)
{
    GO_ASSERT(runtimeConfig);

    initialize_runtime_environment();
    initialize_input();
    initialize_gpu_display(runtimeConfig->display);

    GO_LOGFILE_INFO("Go was successfully initialized.\n");
}


void go::initialize()
{
    initialize_runtime_environment();
    
    // Load environment values
    GO gfx_display_configuration_ex envDDC(GO environment_value);
    GO runtime_configuration rtc;
    rtc.display = &envDDC;

    GO initialize(&rtc);
}


void go::shutdown()
{
    GO_LOGFILE_INFO("Shutting down Go...\n");

    // Wait for all pending I/O requests
    GO the_io_manager->shutdown();

    delete GO the_display;
    delete GO the_system_input;
    delete GO the_input_dispatcher;
    delete the_task_scheduler;

    shutdown_runtime_environment();
}
