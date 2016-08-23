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
    \file hid/input_dispatcher.h
    \brief Contains a class for dispatching input events.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/design/singleton.h"
#include "go/design/observer.h"
#include "go/hid/system_input_driver.h"
#include "go/hid/input_event.h"
#include "go/hid/input_context.h"
#include "tbb/concurrent_queue.h"


GO_BEGIN_NAMESPACE

//! A class, that dispatches input events to input action maps.
class GO_API input_dispatcher : public singleton<input_dispatcher>,
    public observer<input_dispatcher, system_input_driver>
{
public:
    /*!
        Constructor.

        \param maxEventCountHint The maximum number of input events to cache.
     */
    input_dispatcher(size_t maxEventCountHint = 128);
public:
    /*!
        Updates the dispatcher.

        Flushes the event cache.
     */
    void update();
    /*!
        Pushes a new input event into the cache.

        \param e The event to register.
     */
    void push_event(const input_event &e);
    //! Clears all input events.
    void clear_events();
    /*!
        Changes the active input context.

        \param context The new input context.
     */
    void change_active_context(input_context *context) noexcept;
    /*!
        Returns the active input context.
        
        \return The active input context.
     */
    input_context *active_context() noexcept;
private:
    //! Callback function, which is invoked when a system input event occurs.
    bool handle_system_input_event(system_input_driver *, system_input_driver::event_type &);
private:
    //! The input event cache.
    tbb::concurrent_queue<input_event> m_inputEvents;
    //! The active input context.
    input_context *m_activeContext;
};

//! Global input dispatcher.
extern GO_API input_dispatcher *the_input_dispatcher;

GO_END_NAMESPACE
