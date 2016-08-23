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
    \file hid/input_context.h
    \brief Contains a type that represents an input context.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

struct input_action_map;
struct input_event;

//! A class, which represents an input context.
class GO_API input_context
{
public:
    typedef input_action_map action_map_t;
    typedef std::vector<action_map_t *> action_maps_t;
public:
    /*!
        Pushes a new action map into the context.

        \param map The action map to push.
     */
    void push_back(action_map_t *map);
    //! Pops the last action map from the context.
    void pop_back();
    /*!
        Returns the last action map in the context.

        \return The last action map in the context.
     */
    const action_map_t *back();
    //! Removes all action maps from the context.
    void clear();
    /*!
        Handles an input event by forwarding it to all action maps.

        \param e The input event.
     */
    void handle_input_event(const input_event &e);
    //! Updates all action maps in the context.
    void update();
private:
    //! The action maps.
    action_maps_t m_actionMaps;
};

GO_END_NAMESPACE
