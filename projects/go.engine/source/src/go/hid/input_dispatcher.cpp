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


#include "go/hid/input_dispatcher.h"
#include "go/core/performance_counter.h"


go::input_dispatcher *go::the_input_dispatcher = nullptr;


// ================================================================================ //
// go::input_dispatcher member functions
// ================================================================================ //

go::input_dispatcher::input_dispatcher(size_t maxEventCountHint)
    : singleton(the_input_dispatcher),
    observer<input_dispatcher, system_input_driver>(this, &input_dispatcher::handle_system_input_event)
{
    the_system_input->add_observer(this);
}


bool go::input_dispatcher::handle_system_input_event(system_input_driver *,
    system_input_driver::event_type &in)
{
    input_event e;
    e.timeStamp = go::performance.engineFrameTimeMS;
    switch(in.event)
    {
    case system_input_driver_event_data::event_t::keyboard_input:
    {
        e.device = input_event_device::keyboard;
        e.type = input_event_type::digital;
        e.digitalData.isDown = in.wasInputPressed;
        e.digitalData.keyID = in.inputMask;
        m_inputEvents.push(e);
        break;
    }
    case system_input_driver_event_data::event_t::mouse_input:
    {
        e.device = input_event_device::mouse;

        // Movement
        if(in.motionDelta.x != 0 || in.motionDelta.y != 0)
        {
            e.type = input_event_type::analog;
            e.dualAnalogData.motionDelta[0] = in.motionDelta.x;
            e.dualAnalogData.motionDelta[1] = in.motionDelta.y;
            m_inputEvents.push(e);
        }

        // Clicks
        if(in.inputMask != 0)
        {
            e.type = input_event_type::digital;
            e.digitalData.keyID = in.inputMask;
            e.digitalData.isDown = in.wasInputPressed;
            m_inputEvents.push(e);
        }
        break;
    }
    default:
    {
        GO_MSGASSERT(false, "Unknown system input event type.");
        break;
    }
    }
    
    return true;
}


void go::input_dispatcher::update()
{
    // Early out if no active context
    auto activeContext = m_activeContext;
    if(!activeContext)
    {
        return;
    }

    // Dispatch all queued events
    input_event e;
    while(m_inputEvents.try_pop(e))
    {
        activeContext->handle_input_event(e);
    }

    activeContext->update();
}


void go::input_dispatcher::push_event(const input_event &e)
{
    m_inputEvents.push(e);
}


void go::input_dispatcher::clear_events()
{
    m_inputEvents.clear();
}


void go::input_dispatcher::change_active_context(input_context *context) noexcept
{
    m_activeContext = context;
}


go::input_context *go::input_dispatcher::active_context() noexcept
{
    return m_activeContext;
}
