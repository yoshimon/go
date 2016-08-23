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


#include "go/core/log.h"


#include "go/platform/windows.h"
#include "go/core/path_utility.h"
#include <ctime>
#include <cstdarg>
#include <iomanip>


static std::string s_defaultPrefix("");
static std::string s_infoPrefix("[I] ");
static std::string s_errorPrefix("[E] ");
static std::string s_warningPrefix("[W] ");


go::log *go::the_logger = nullptr;


// ================================================================================ //
// go::log member functions
// ================================================================================ //

go::log::log()
    : singleton<log>(the_logger)
{
    m_stream.open(file_path(GO_LOG_FILE).c_str());

#if defined(DEBUG) || defined(_DEBUG)
    if(!m_stream.is_open())
    {
        OutputDebugStringA("Go - [WARNING] Could not open standard log file.\n");
    }
#endif
}


void go::log::report(const char *text, uint32_t flags)
{
    if((flags & log::file) && (m_stream.is_open()))
    {
        auto &prefix = s_defaultPrefix;
        if(flags & log::info) prefix = s_infoPrefix;
        else if(flags & log::error) prefix = s_errorPrefix;
        else if(flags & log::warning) prefix = s_warningPrefix;

        std::lock_guard<std::mutex> lock(m_streamLock);
        if(flags & log::no_date)
        {
            m_stream << prefix << text;
        }
        else
        {
            tm ti;
            __time32_t t;
            _time32(&t);
            _localtime32_s(&ti, &t);

            m_stream << "[" << std::setw(2) << std::setfill('0') << ti.tm_hour
                << ":" << std::setw(2) << std::setfill('0') << ti.tm_min
                << ":" << std::setw(2) << std::setfill('0') << ti.tm_sec
                << "] " << prefix << text;
        }
        m_stream.flush();
    }

    post_report_event(text, flags);
}


void go::log::reportf(const char *fmtText, uint32_t flags, ...)
{
    char buf[GO_MAX_LOGGER_FORMAT_STRING_LENGTH + 1];

    va_list argList;
    va_start(argList, flags);

    auto written = vsnprintf_s(buf, GO_MAX_LOGGER_FORMAT_STRING_LENGTH, fmtText, argList);

    va_end(argList);

    if(written > 0)
    {
        buf[written] = '\0';
        report(buf, flags);
    }
}


void go::log::post_report_event(const char *text, uint32_t flags)
{
    log_event_data e = { text, flags };
#if defined(DEBUG) || defined(_DEBUG)
    OutputDebugStringA(text);
#endif
    post_event(e);
}
