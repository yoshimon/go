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
    \file core/log.h
    \brief Contains a class to log events.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/defines.h"
#include "go/stdafx.h"
#include "go/design/observer.h"
#include "go/design/singleton.h"


GO_BEGIN_NAMESPACE

//! A structure, which encodes an event within a log instance.
struct GO_API log_event_data
{
    //! The reported text message.
    const char *text;
    //! Flags pertaining to the message.
    uint32_t flags;
};


//! A class, which provides simple text-logging functionality.
class GO_API log : public singleton<log>, public observable<log, log_event_data &>
{
    GO_DECLARE_NON_COPYABLE(log)
public:
    //! The text is written to the standard log file.
    static const uint32_t file = (1 << 0);
    //! The text should be considered as an error message.
    static const uint32_t error = (1 << 1);
    //! The text should be considered as a simple information message.
    static const uint32_t info = (1 << 2);
    //! The text should be considered as a warning message.
    static const uint32_t warning = (1 << 3);
    //! Skips the date formatter stage.
    static const uint32_t no_date = (1 << 4);
public:
    //! Constructor.
    log();
public:
    /*!
        Reports a message to the log.

        \param text The text message to report.
        \param flags Flags pertaining to the message.
     */
    void report(const char *text, uint32_t flags = 0);
    /*!
        Formats and reports a message to the log.

        \param fmtText The format text message.
        \param flags Flags pertaining to the message.
     */
    void reportf(const char *fmtText, uint32_t flags, ...);
private:
    /*!
        Posts an event to all observers, which notifies them of a new incoming report.

        \param text The message being reported.
        \param flags Flags pertaining to the message.
     */
    void post_report_event(const char *text, uint32_t flags);
private:
    //! The output stream to write the log messages to.
    std::ofstream m_stream;
    //! A mutex for synchronizing writes to the output stream.
    std::mutex m_streamLock;
};


//! Global log singleton.
extern GO_API log *the_logger;

GO_END_NAMESPACE
