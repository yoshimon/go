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


#include "go/graphics/d3d11/timer.h"


#include "go/graphics/d3d11/device.h"


// ================================================================================ //
// go::gfx_timer_frequency member functions
// ================================================================================ //

go::gfx_timer_frequency::gfx_timer_frequency()
{
    CD3D11_QUERY_DESC qd(D3D11_QUERY_TIMESTAMP_DISJOINT);
    GO_THROW_HRESULT
    (
        GO_INTERNAL g_d3d11Device->CreateQuery(&qd, &m_query),
        "Could not create D3D11 disjoint timestamp query."
    );
}


void go::gfx_timer_frequency::begin()
{
    if(m_isEnabled)
    {
        GO_INTERNAL g_d3d11ImmediateContext->Begin(m_query.Get());
    }
}


void go::gfx_timer_frequency::end()
{
    if(m_isEnabled)
    {
        GO_INTERNAL g_d3d11ImmediateContext->End(m_query.Get());
    }
}


bool go::gfx_timer_frequency::wait_for(uint64_t *outFrequency)
{
    if(!m_isEnabled)
    {
        if(outFrequency)
        {
            *outFrequency = 1;
        }

        return true;
    }

    while(GO_INTERNAL g_d3d11ImmediateContext->GetData(m_query.Get(), NULL, 0, 0) == S_FALSE)
    {
        Sleep(1);
    }

    // See if the query is valid
    D3D11_QUERY_DATA_TIMESTAMP_DISJOINT qdtd;
    GO_INTERNAL g_d3d11ImmediateContext->GetData(m_query.Get(), &qdtd, sizeof(qdtd), 0);

    if(outFrequency)
    {
        *outFrequency = qdtd.Frequency;
    }

    return qdtd.Disjoint == FALSE;
}


void go::gfx_timer_frequency::enable(bool state)
{
    m_isEnabled = state;
}


bool go::gfx_timer_frequency::is_enabled() const
{
    return m_isEnabled;
}


// ================================================================================ //
// go::gfx_timestamp member functions
// ================================================================================ //

go::gfx_timestamp::gfx_timestamp()
{
    CD3D11_QUERY_DESC qd(D3D11_QUERY_TIMESTAMP);
    GO_THROW_HRESULT
    (
        GO_INTERNAL g_d3d11Device->CreateQuery(&qd, &m_query),
        "Could not create D3D11 timestamp query."
    );
}


void go::gfx_timestamp::query_now()
{
    GO_INTERNAL g_d3d11ImmediateContext->End(m_query.Get());
}


bool go::gfx_timestamp::wait_for(uint64_t *outCycles)
{
    // See if the query is valid
    while(GO_INTERNAL g_d3d11ImmediateContext->GetData(m_query.Get(), NULL, 0, 0) == S_FALSE)
    {
        Sleep(1);
    }

    return GO_INTERNAL g_d3d11ImmediateContext->GetData(m_query.Get(), outCycles, sizeof(uint64_t), 0) == S_OK;
}


// ================================================================================ //
// go::gfx_timers member functions
// ================================================================================ //

void go::gfx_timers::enable(bool state)
{
    m_isEnabled = state;
}


bool go::gfx_timers::is_enabled() const
{
    return m_isEnabled;
}


void go::gfx_timers::reserve(size_t count)
{
    // Create all queries upfront
    m_timestamps.resize(count);
}


void go::gfx_timers::shrink_to_fit()
{
    // Reset the active counter
    clear();

    // Delete all reserved timers
    m_timestamps.clear();
}


void go::gfx_timers::add_event()
{
    // Create a new timer if necessary
    if(m_activeTimers == m_timestamps.size())
    {
        m_timestamps.emplace_back();
    }

    // Grab the next timer
    if(m_isEnabled)
    {
        auto &&timer = m_timestamps[m_activeTimers];
    
        // Issue the query
        timer.query_now();
    }

    // Increase active timer count
    ++m_activeTimers;
}


const go::gfx_timestamps &go::gfx_timers::timestamps()
{
    return m_timestamps;
}


void go::gfx_timers::clear()
{
    m_activeTimers = 0;
}


const std::vector<float> &go::gfx_timers::resolved_timestamps() const
{
    return m_resolvedTimestamps;
}


bool go::gfx_timers::resolve_milliseconds(uint64_t frequency)
{
    // Clear output
    m_resolvedTimestamps.clear();
    m_resolvedTimestamps.resize(m_activeTimers - 1);

    std::fill(m_resolvedTimestamps.begin(), m_resolvedTimestamps.end(), 0.0f);

    // Needs to have more than 2 events for delta
    if(!m_isEnabled || m_activeTimers < 2)
    {
        return false;
    }

    // Grab the first entry
    uint64_t lastCycles;
    if(!m_timestamps[0].wait_for(&lastCycles))
    {
        return false;
    }

    // RCP of timer frequency
    auto &&cyclesToMS = 1000.0f / float(frequency);

    // Skip the first entry
    for(size_t i = 1, end = m_activeTimers; i < end; ++i)
    {
        auto &&current = m_timestamps[i];

        uint64_t currentCycles;
        if(!current.wait_for(&currentCycles))
        {
            return false;
        }

        // Calculate difference to last element
        auto &&diffMS = float(currentCycles - lastCycles) * cyclesToMS;

        // Save the timestamp out
        m_resolvedTimestamps[i - 1] = diffMS;

        lastCycles = currentCycles;
    }

    return true;
}