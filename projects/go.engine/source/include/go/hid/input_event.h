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
    \file hid/input_event.h
    \brief Contains structures to represent input events.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! The device that triggered the event.
enum class input_event_device : int32_t
{
    //! The device is a keyboard.
    keyboard,
    //! The device is a mouse.
    mouse
};


//! Enumeration of possible input event types.
enum class input_event_type : int32_t
{
    //! The input event comes from a digital sensor.
    digital,
    //! The input event comes from an analog sensor.
    analog
};


//! A structure, which represents an input event.
struct GO_API input_event
{
    //! The global engine timestamp.
    uint32_t timeStamp;
    //! The device that fired the event.
    input_event_device device;
    //! The event type.
    input_event_type type;
    union
    {
        //! The digital input data.
        struct
        {
            //! The key or button ID.
            uint32_t keyID;
            //! Indicates whether the button is up or down.
            bool32_t isDown;
        } digitalData;
        //! The analog input data.
        struct
        {
            //! The axis data along both axis.
            int32_t motionDelta[2];
        } dualAnalogData;
    };
};

GO_END_NAMESPACE
