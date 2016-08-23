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


#include "go/hid/input_context.h"


#include "go/hid/input_action_map.h"
#include "go/hid/input_event.h"


// ================================================================================ //
// go::input_context member functions
// ================================================================================ //

void go::input_context::push_back(action_map_t *map)
{
    m_actionMaps.push_back(map);
}


void go::input_context::pop_back()
{
    m_actionMaps.pop_back();
}


auto go::input_context::back() -> const action_map_t *
{
    return m_actionMaps.back();
}


void go::input_context::clear()
{
    m_actionMaps.clear();
}


void go::input_context::handle_input_event(const input_event &e)
{
    bool wasHandled = false;
    for(auto &actionMap : m_actionMaps)
    {
        actionMap->handle_input_event(e, wasHandled);
    }
}


void go::input_context::update()
{
    for(auto &actionMap : m_actionMaps)
    {
        actionMap->update();
    }
}
