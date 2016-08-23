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
    \file core/timer.h
    \brief Contains a class for accessing the high resolution timer of the computer.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

//! A class, which provides simplified access to the high resolution timer of the computer.
class GO_API timer
{
public:
    /*!
        Constructor.

        \param startTimeSeconds Optional start time in seconds.
     */
    timer(float startTimeSeconds = 0.0f);
public:
    /*!
        Converts time (measured in cycles) to seconds.
        
        \param timeCycles Time in cycles.

        \return The time in seconds.
     */
    static float cycles_to_seconds(uint64_t timeCycles);
    /*!
        Converts time (measured in cycles) to seconds.

        \param startTimeCycles Time in cycles.

        \return The time in seconds.
     */
    static float query_delta_time_seconds(uint64_t startTimeCycles);
    /*!
        Converts time (measured in cycles) to milliseconds.

        \param startTimeCycles Time in cycles.

        \return The time in milliseconds.
     */
    static float query_delta_time_milliseconds(uint64_t startTimeCycles);
    /*!
        Converts time (measured in seconds) to cycles.
        
        \param timeSeconds Time in seconds.

        \return The time in cycles.
     */
    static uint64_t seconds_to_cycles(float timeSeconds);
    /*!
        Queries the hardware timer frequency.
        
        Call this once before using this class.
     */
    static void query_hardware_timer_frequency();
    /*!
        Queries the current time in cycles.
        
        \return Current time in cycles.
     */
    static uint64_t query_current_time_cycles();
public:
    //! Time scale factor.
    float timeScale;
    //! Indicates whether the timer is paused.
    bool isPaused;
public:
    /*!
        Calculates the time difference between two timers.
        
        \return Time difference in seconds.
     */
    float calculate_delta_seconds(const timer &other);
    /*!
        Returns the elapsed time in cycles since the timer was started.
        
        \return Elapsed time in cycles since the timer was started.
     */
    uint64_t life_time_cycles() const;
    /*!
        Returns the current time in cycles.
        
        \return Time in cycles.
     */
    uint64_t time_cycles() const;
    /*!
        Steps the timer ahead.

        \param elapsedTime Unscaled frame delta time (step size) in seconds.
                
        \return Scaled elapsed time. If the timer is disabled the return value is zero.
     */
    float integrate(float elapsedTime);
private:
    //! Reciprocal of hi-res timer frequency.
    static float s_frequencyReciprocal;
private:
    //! Total clock time in cycles.
    uint64_t m_timeCycles;
    //! Clock initialization time-stamp
    uint64_t m_timeLifeBeginCycles;
};

GO_END_NAMESPACE
