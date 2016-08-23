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


#include "go/core/timer_pool.h"


// ================================================================================ //
// go::timer_pool member functions
// ================================================================================ //

go::timer_pool::timer_pool(size_t timerCountHint)
    : m_poolSizeHint(timerCountHint), m_firstFreeIndex(-1), m_lastFreeIndex(-2)
{
    m_timers.reserve(timerCountHint);
    m_timerValues.reserve(timerCountHint);
}


go::timer_pool::timer_pool(timer_pool &&other) noexcept
{
    *this = std::move(other);
}


go::timer_pool &go::timer_pool::operator=(timer_pool &&other) noexcept
{
    m_timers = std::move(other.m_timers);
    m_timerValues = std::move(other.m_timerValues);
    return *this;
}


go::handle_t go::timer_pool::add_timer(timer_info info)
{
    if(m_timers.size() == m_timers.capacity())
    {
        m_timers.reserve(m_timers.size() + m_poolSizeHint);
        m_timerValues.reserve(m_timers.size());
    }

    // Search for unused timer slots
    for(auto i = m_firstFreeIndex; i <= m_lastFreeIndex; ++i)
    {
        if(m_timerValues[i] == 0.0f)
        {
            m_timerValues[i] = info.interval;
            m_firstFreeIndex = i;
            m_timers[i] = std::move(info);
            return i;
        }
    }

    // If no unused timer slots available reserve a new one
    auto index = m_timers.size();
    m_timerValues.push_back(info.interval);
    m_timers.push_back(std::move(info));
    
    return index;
}


void go::timer_pool::dispose_timer(handle_t handle) noexcept
{
    GO_ASSERT(handle > 0 && handle < m_timerValues.size());
    
    m_timerValues[handle] = 0.0f;

    if(handle < m_firstFreeIndex)
    {
        m_firstFreeIndex = handle;   
    }
    else if(handle > m_lastFreeIndex)
    {
        m_lastFreeIndex = handle;
    }
}


void go::timer_pool::change_timer(handle_t handle, timer_info info) noexcept
{
    GO_ASSERT(handle > 0 && handle < m_timerValues.size());

    m_timers[handle] = std::move(info);
}


void go::timer_pool::reset_timer(handle_t handle) noexcept
{
    GO_ASSERT(handle > 0 && handle < m_timerValues.size());

    m_timerValues[handle] = m_timers[handle].interval;
}


void go::timer_pool::clear_timers()
{
    m_timers.clear();
    m_timerValues.clear();
}


void go::timer_pool::update(float elapsedTime) noexcept
{
    handle_t i = 0;
    for(auto &t : m_timerValues)
    {
        if(t == 0.0f)
        {
            // Timer disabled
            continue;
        }
        
        t -= elapsedTime;
        if(t <= 0.0f)
        {
            auto &ti = m_timers[i];
            if(ti.currentTriggerCount == std::numeric_limits<uint32_t>::max())
            {
                t = ti.interval; // Loop infinite
            }
            else if(ti.currentTriggerCount >= ti.triggerCount)
            {
                t = ti.interval; // Reset the timer
                ++ti.currentTriggerCount;
            }
            else
            {
                t = 0.0f; // Disable timer
            }

            // Shrink/widen linear search range for next timer
            if(i < m_firstFreeIndex)
            {
                m_firstFreeIndex = i;
            }
            else if(i > m_lastFreeIndex)
            {
                m_lastFreeIndex = i;
            }

            ti.callback(ti);
        }

        ++i;
    }
}
