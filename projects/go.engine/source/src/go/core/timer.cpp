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


#include "go/core/timer.h"


#include "go/platform/windows.h"
#include "go/core/log.h"


// ================================================================================ //
// go::timer static members
// ================================================================================ //

float go::timer::s_frequencyReciprocal = 0.0f;


// ================================================================================ //
// go::timer member functions
// ================================================================================ //

go::timer::timer(float startTimeSeconds)
    : timeScale(1.0f), isPaused(false), m_timeCycles(seconds_to_cycles(startTimeSeconds)), m_timeLifeBeginCycles(0)
{
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&m_timeLifeBeginCycles));
}


float go::timer::cycles_to_seconds(uint64_t timeCycles)
{
    return static_cast<float>(timeCycles) * s_frequencyReciprocal;
}


float go::timer::query_delta_time_seconds(uint64_t startTimeCycles)
{
    auto &&end = timer::query_current_time_cycles();
    return timer::cycles_to_seconds(end - startTimeCycles);
}


float go::timer::query_delta_time_milliseconds(uint64_t startTimeCycles)
{
    return query_delta_time_seconds(startTimeCycles) * 1000.0f;
}


uint64_t go::timer::seconds_to_cycles(float timeSeconds)
{
    return static_cast<uint64_t>(timeSeconds / s_frequencyReciprocal);
}


uint64_t go::timer::query_current_time_cycles()
{
    uint64_t cycles = 0;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&cycles));
    return cycles;
}


void go::timer::query_hardware_timer_frequency()
{
    LARGE_INTEGER freq;

    // Query high-res timer frequency
    if(!QueryPerformanceFrequency(&freq))
    {
        GO_THROW(os_error, "Could not query high-resolution timer frequency.");
    }

    // Cache reciprocal of frequency
    s_frequencyReciprocal = 1.0f / static_cast<float>(freq.QuadPart);
}


float go::timer::calculate_delta_seconds(const timer &other)
{
    return cycles_to_seconds(m_timeCycles - other.time_cycles());
}


uint64_t go::timer::life_time_cycles() const
{
    return m_timeLifeBeginCycles;
}


uint64_t go::timer::time_cycles() const
{
    return m_timeCycles;
}


float go::timer::integrate(float elapsedTime)
{
    if(!isPaused)
    {
        auto e = elapsedTime * timeScale;
        m_timeCycles += seconds_to_cycles(e);
        return e;
    }
    
    return 0.0f;
}
