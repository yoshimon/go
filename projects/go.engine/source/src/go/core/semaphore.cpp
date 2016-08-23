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


#include "go/core/semaphore.h"


// ================================================================================ //
// go::semaphore member functions
// ================================================================================ //

go::semaphore::semaphore(int32_t value)
    : m_value(value), m_numWakeups(0)
{
}


void go::semaphore::signal()
{
    std::lock_guard<std::mutex> lock(m_lock);
    ++m_value;
    if(m_value <= 0)
    {
        ++m_numWakeups;
        m_condVar.notify_one();
    }
}


void go::semaphore::wait()
{
    std::unique_lock<std::mutex> lock(m_lock);
    --m_value;
    if(m_value < 0)
    {
        m_condVar.wait(lock, [&] { return m_numWakeups > 0; });
        --m_numWakeups;
    }
}
