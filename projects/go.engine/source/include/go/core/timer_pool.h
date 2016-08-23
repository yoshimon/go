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
    \file core/timer_pool.h
    \brief Contains a class that manages a pool of timers.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

//! A class, which manages a pool of timers.
class GO_API timer_pool
{
public:
    //! Forward declaration.
    struct timer_info;
    //! The timer-callback function.
    typedef void (*timer_callback_function)(const timer_info &);
    //! A structure, containing information about a timer.
    struct timer_info
    {
        //! The interval at which the timer is triggered in seconds.
        float interval;
        /*!
            The number of times the timer should be triggered. Set this to 0 to trigger the timer
            indefinitely.
         */
        uint32_t triggerCount;
        /*!
            The current number of times the timer has been trigger.
            If the timer should be triggered #triggerCount times, set this to 0.
         */
        uint32_t currentTriggerCount;
        //! The callback function.
        timer_callback_function callback;
        //! A user-defined parameter to pass to the callback function.
        void *cookie;
    };
public:
    /*!
        Default constructor.

        \param timerCountHint Number of estimated timers the pool should hold.
     */
    timer_pool(size_t timerCountHint = 10);
    /*!
        Move constructor.
        
        \param other The object to move.
     */
    timer_pool(timer_pool &&other) noexcept;
public:
    /*!
        Move-assignment operator.

        \param other The object to move.

        \return The object being move to.
     */
    timer_pool &operator=(timer_pool &&other) noexcept;
public:
    /*!
        Adds a new timer to the pool.

        \param info The timer description.

        \return A handle to the new timer.
     */
    handle_t add_timer(timer_info info);
    /*!
        Disposes an existing timer.

        \param handle The timer handle.
     */
    void dispose_timer(handle_t handle) noexcept;
    /*!
        Resets an existing timer.

        \param handle The timer handle.
        \param info The timer descriptor.
     */
    void change_timer(handle_t handle, timer_info info) noexcept;
    /*!
        Resets a timer to its initial state.

        \param handle The timer handle.
     */
    void reset_timer(handle_t handle) noexcept;
    //! Removes all timers from the pool.
    void clear_timers();
    //! Updates the timer pool and its timers.
    void update(float elapsedTime) noexcept;
private:
    //! The user-supplied pool-size hint.
    size_t m_poolSizeHint;
    //! The first free timer index.
    handle_t m_firstFreeIndex;
    //! The last free timer index.
    handle_t m_lastFreeIndex;
    //! The internal timer pool.
    std::vector<timer_info> m_timers;
    //! The current timer values.
    std::vector<float> m_timerValues;
};

GO_END_NAMESPACE
