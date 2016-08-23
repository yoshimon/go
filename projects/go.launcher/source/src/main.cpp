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

#include "go/golib.h"
#include "tbb/tbbmalloc_proxy.h"
#include <tchar.h>


// DLL export function stub
typedef go::application *(*create_go_application_stub)();


void app_post_exit()
{
    go::the_application->post_exit();
}


void app_exit()
{
    app_post_exit();
    go::the_application->run();
}


int _tmain(int argc, _TCHAR *argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    _tprintf(_T("\nGoLauncher (Build ") _T(__TIMESTAMP__) _T(")\n\n"));

    if(argc < 2)
    {
        _tprintf(_T("! ERROR: invalid number of arguments.\n"));
        return -1;
    }

    // All arguments with default values
    auto quietMode = false;
    auto hideConsole = false;
    _TCHAR *dllArg = nullptr;

    // Parse arguments
    for(auto i = 0; i < argc; ++i)
    {
        auto arg = argv[i];
        auto argLen = _tcslen(arg);

        if(argLen == 2)
        {
            if(arg[0] == '-')
            {
                switch(arg[1])
                {
                case 'q': quietMode = true; break;
                case 'h': hideConsole = true; break;
                }
            }
        }
    }

    if(argc > 0)
    {
        dllArg = argv[argc-1];
    }

    // Initialize COM
    _tprintf("> Initializing COM...\n");
    if(FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
    {
        _tprintf("! ERROR: Could not initialize COM.\n");
        return -5;
    }

    if(FAILED(CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0)))
    {
        _tprintf("! ERROR: Could not initialize COM security descriptors.\n");
        return -6;
    }

    // Load user DLL
    _tprintf(_T("> Loading user module '%s'...\n"), dllArg);
    auto hUserDll = LoadLibrary(dllArg);
    if(hUserDll == NULL)
    {
        _tprintf(_T("! ERROR: Could not load user library '%s'.\n"), dllArg);
        return -2;
    }

    // Create new application object
    _tprintf(_T("> Initializing the global application instance...\n"));
    auto createGoApp = (create_go_application_stub)GetProcAddress(hUserDll, "create_go_application");
    if(createGoApp == NULL)
    {
        _tprintf(_T("! ERROR: Could not find export-function 'create_go_application' in module '%s'.\n"), dllArg);
        return -3;
    }

    go::the_application = createGoApp();
    if(!go::the_application)
    {
        _tprintf(_T("! ERROR: Could not create user application in module '%s'.\n"), dllArg);
        return -4;
    }
    
    // Initialize Go
    _tprintf(_T("> Initializing Go Library...\n"));
	try
	{
		go::initialize();
		_tprintf(_T("> Running client code...\n"));
		go::the_application->handle_initialize(argc, argv);
    }
    catch(std::exception &e)
    {
        _tprintf(_T("! ERROR: Could not initialize Go Library. See the log file for further information. Exception: %s.\n"), e.what());
        app_post_exit();
    }
	catch(...)
	{
        _tprintf(_T("! ERROR: Could not initialize Go Library. See the log file for further information.\n"));
        app_post_exit();
    }

	if(hideConsole)
	{
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	try
	{
		go::the_application->run();
	}
    catch(std::exception &e)
    {
        _tprintf(_T("! ERROR: An unexpected exception occurred during program execution. See the log file for further information. Exception: %s.\n"), e.what());
        app_exit();
    }
	catch(...)
	{
		_tprintf(_T("! ERROR: An unexpected exception occurred during program execution. See the log file for further information.\n"));
        app_exit();
	}

	if(hideConsole)
	{
		ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL);
	}

    // Shut down Go
	_tprintf(_T("> Shutting down Go Library...\n"));
    try
    {
        go::shutdown();
    }
    catch(...)
    {
        // Nothing we can do
    }

    // Shut down COM
    _tprintf(_T("> Shutting down COM...\n"));
    CoUninitialize();

    if(!quietMode)
    {
        _tprintf(_T("\n> Press any key to exit...\n"));
        std::getchar();
    }

    return 0;
}