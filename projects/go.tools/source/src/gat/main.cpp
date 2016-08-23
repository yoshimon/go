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

#include "gat/util/exception.h"
#include "gat/tools/tool_base.h"
#include "gat/register_tools.h"
#include "gat/files/config_file.h"


/*!
    The entry-point.

    \param argc The argument count.
    \param argv The argument vector.

    \return The program exit code.
 */
int _tmain(int argc, const _TCHAR *argv[])
{    
    _tprintf(_T("/*\n * Go Asset Tools\n * Build ") _T(__DATE__) _T(" @ ") _T(__TIME__) _T("\n * Version 1.0\n */\n\n"));

    int result = 0;
    try
    {
        if(argc <= 2)
        {
            throw gat::tchar_exception(_T("Not enough parameters."));
        }

        // Cache application paths
        go::file_path appFilePath(argv[0]);
        auto appDirPath = appFilePath.directory_path();

        // Register all asset tools
#include "gat/register_tools.inl"

        // Identify asset converter
        auto toolName = argv[1];
        auto converter = gat::tool_base::from_name(toolName);
        if(!converter)
        {
            throw gat::tchar_exception(_T("No matching asset converter found."));
        }

        // Remove AppName and converter string
        argc -= 2;
        argv += 2;

        // Load the configuration file
        gat::config_file cfg(appDirPath + "go.tools.xml");
        cfg.applicationDirPath = appDirPath;
        cfg.applicationFilePath = appFilePath;

        converter->run(argc, argv, cfg);

        _tprintf(_T("Done.\n"));
    }
    catch(gat::tchar_file_not_found &e)
    {
        _tprintf(_T("[ERROR] File not found: %s.\n"), e.what());
        result = -1;
    }
    catch(gat::tchar_exception &e)
    {
        _tprintf(_T("[ERROR] %s\n"), e.what());
        result = -1;
    }
    catch(...)
    {
        _tprintf(_T("[ERROR] An error occurred."));
        result = -1;
    }

    getchar();

    return result;
}