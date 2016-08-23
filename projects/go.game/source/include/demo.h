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

#pragma once


#include "go\golib.h"
#include "action_maps.h"


extern "C"
{
    __declspec(dllexport) go::application *create_go_application();
}


class demo_game : public go::singleton<demo_game>, public go::application
{
public:
    demo_game();
    ~demo_game();
public:
    virtual bool handle_modify_runtime_configuration(go::runtime_configuration *runtimeConfig) override;
    virtual void handle_initialize(int numArguments, char *cmdLine[]) override;
    virtual void handle_shutdown() override;
    virtual void handle_update() override;
    virtual void handle_draw_world() override;
    virtual void handle_draw_interface() override;
    virtual void handle_activate() override;
    virtual void handle_deactivate() override;
private:
    std::unique_ptr<go::gfx_scene_manager> m_sceneMgr;
    scene_action_map m_sceneInputActionMap;
    go::input_context m_sceneInputContext;
    ui_action_map m_uiInputActionMap;
    go::input_context m_uiInputContext;
};


extern demo_game *the_game;