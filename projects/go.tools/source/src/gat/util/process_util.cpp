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

#include "gat/util/process_util.h"


#include "gat/util/exception.h"


//===========================================================================
// gat::process_util
//===========================================================================

int gat::process_util::create_process(const _TCHAR *appFilePath,
    const _TCHAR *cmdLine, bool hide)
{
    tstring cmdLineStr(" ");
    cmdLineStr.append(cmdLine);

    STARTUPINFO sui;
    memset(&sui, 0, sizeof(sui));
    sui.cb = sizeof(sui);

    PROCESS_INFORMATION pi;
    memset(&pi, 0, sizeof(pi));

    auto wasCreated = CreateProcess(appFilePath, (char *)cmdLineStr.c_str(),
        NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi);
    if(!wasCreated)
    {
        throw tchar_exception(_T("Could not create external process.\n"));
    }

    DWORD ec;
    auto waitResult = WaitForSingleObject(pi.hProcess, INFINITE);
    auto exitCodeResult = GetExitCodeProcess(pi.hProcess, &ec);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    if(waitResult != WAIT_OBJECT_0)
    {
        throw tchar_exception(_T("Process timed out.\n"));
    }

    if(!exitCodeResult)
    {
        throw tchar_exception(_T("Could not query process exit code.\n"));
    }

    return ec;
}