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
    \file hid/input_action_map.h
    \brief Contains a class for creating input action mappings.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

struct input_event;

//! An abstract structure, which represents an input action map.
struct GO_API input_action_map
{
    /*!
        Callback function, which is invoked when an input event occurs.

        \param e The input event.
        \param wasHandled State flag, indicating whether the input event was
        already handled by a previous action map.
     */
    virtual void handle_input_event(const input_event &e, bool &wasHandled) = 0;
    //! Callback function, which is invoked after all input events were dispatched.
    virtual void update() = 0;
};

GO_END_NAMESPACE
