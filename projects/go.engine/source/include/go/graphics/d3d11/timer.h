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
    \file graphics/d3d11/timer.h
    \brief Contains a class for timing GPU events.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once

#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! A class, which holds the frequency of the internal GPU timer.
class GO_API gfx_timer_frequency
{
public:
    //! Constructor.
    gfx_timer_frequency();
public:
    /*!
        Changes the timer state.

        \param state True, if the timer should be enabled. Otherwise false.
     */
    void enable(bool state);
    /*!
        Returns the state of the timers.

        \return True, if the timers are enabled. Otherwise false.
     */
    bool is_enabled() const;
    //! Begins a new timer frame.
    void begin();
    //! Returns the frequency of the internal clock in cycles.
    void end();
    /*!
        Waits for the query to complete.

        \param outFrequency The frequency.

        \return True, if the query was successful. Otherwise false.
     */
    bool wait_for(uint64_t *outFrequency);
    /*!
        Returns the internal D3D object.

        \return The internal D3D object.
     */
    ID3D11Query *d3d_query() { return m_query.Get(); }
private:
    //! The frequency query.
    Microsoft::WRL::ComPtr<ID3D11Query> m_query;
    //! State flag.
    bool m_isEnabled = true;
};


//! A class, which represents a GPU timestamp.
class GO_API gfx_timestamp
{
public:
    //! Constructor.
    gfx_timestamp();
public:
    //! Queries the current timestamp.
    void query_now();
    /*!
        Waits for the query to complete.

        \param outCycles The cycles.

        \return True, if the query was successful. Otherwise false.
     */
    bool wait_for(uint64_t *outCycles);
    /*!
        Returns the internal D3D object.

        \return The internal D3D object.
     */
    ID3D11Query *d3d_query() { return m_query.Get(); }
private:
    //! The time query.
    Microsoft::WRL::ComPtr<ID3D11Query> m_query;
};

typedef std::vector<gfx_timestamp> gfx_timestamps;
typedef std::vector<float> gfx_resolved_timestamps;

//! A class, which represents a collection of GPU timers.
class GO_API gfx_timers
{
public:
    /*!
        Changes the timer state.

        \param state True, if the timer should be enabled. Otherwise false.
     */
    void enable(bool state);
    /*!
        Returns the state of the timers.

        \return True, if the timers are enabled. Otherwise false.
     */
    bool is_enabled() const;
    /*!
        Reserves a number of timers.

        \param count The number of timers to reserve.
     */
    void reserve(size_t count);
    //! Starts a new event.
    void add_event();
    /*!
        Returns a list of collected timer events.

        \return The list of timer events.
     */
    const gfx_timestamps &timestamps();
    /*!
        Resolves all events.

        \param frequency The frequency of the timer.
        \param outTimestamps The resolved times in milliseconds.

        \return True, if all queries were successful. Otherwise false.
     */
    bool resolve_milliseconds(uint64_t frequency);
    /*!
        Returns the vector of the currently resolved timestamps.

        \return The vector of the currently resolved timestamps.
     */
    const std::vector<float> &resolved_timestamps() const;
    //! Clears all events.
    void clear();
    //! Shrinks the timer list to the current active size.
    void shrink_to_fit();
private:
    //! The timestamps.
    gfx_timestamps m_timestamps;
    //! The number of active timers.
    size_t m_activeTimers = 0;
    //! The resolved timestamps.
    std::vector<float> m_resolvedTimestamps;
    //! State flag.
    bool m_isEnabled = true;
};

//! A class, which represents a double-buffered element.
template<typename T>
class GO_API double_buffered
{
public:
    //! Constructor.
    double_buffered() : m_A(), m_B(), m_active(&m_A), m_swapCount(0) {}
public:
    /*!
        Returns the active element.
        
        \return The active element.
     */
    const T &active() const { return *m_active; }
    /*!
        Returns the active element.

        \return The active element.
     */
    T &active() { return *m_active; }
    /*!
        Returns the active element.

        \return The active element.
    */
    const T &inactive() const { return m_active == &m_A ? m_B : m_A; }
    /*!
        Returns the inactive element.

        \return The inactive element.
    */
    T &inactive() { return m_active == &m_A ? m_B : m_A; }
    /*!
        Swaps the active element.

        \return The old active element.
     */
    T &swap()
    {
        auto &&result = *m_active;
        m_active = m_active == &m_A ? &m_B : &m_A;
        ++m_swapCount;
        return result;
    }
    /*!
        Swaps the active element.
     
        \return The old active element.
     */
    const T &swap() const
    {
        auto &&result = *m_active;
        m_active = m_active == &m_A ? &m_B : &m_A;
        ++m_swapCount;
        return result;
    }
    /*!
        Returns the number of times that the buffer has been swapped.

        \return The number of times that the buffer has been swapped.
     */
    uint64_t swap_count() const { return m_swapCount; }
private:
    //! The first timer set.
    T m_A;
    //! The second timer set.
    T m_B;
    //! The active element.
    T *m_active;
    //! The number of times that the buffer has been swapped.
    uint64_t m_swapCount;
};

GO_END_NAMESPACE