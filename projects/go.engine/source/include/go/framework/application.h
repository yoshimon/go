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
    \file framework/application.h
    \brief Contains a class for creating an application with Go.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/runtime_configuration.h"
#include "go/environment/variable.h"
#include "go/design/singleton.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! An abstract class, which can be used to create Go applications.
class GO_API application : public singleton<application>,
    public observer<application, gfx_display, gfx_display_event_data &>
{
public:
    //! Destructor.
    virtual ~application();
public:
    /*!
        Callback function to modify the runtime configuration before it is passed to Go.

        \param[in, out] runtimeConfig Runtime configuration.
        \return If Go should be initialized with the new configuration the return value should be
        true. Otherwise the return value should be false (aborting initialization).
     */
    virtual bool handle_modify_runtime_configuration(runtime_configuration *runtimeConfig) = 0;
    /*!
        Callback function to handle object initialization.
        This function will be called after Go has been initialized.
        \param numArgs Number of arguments. 
        \param cmdLine Command-line arguments.
     */
    virtual void handle_initialize(int numArguments, char *cmdLine[]) = 0;
    /*!
        Callback function to handle object destruction.
        This function will be called before Go shuts down.
     */
    virtual void handle_shutdown() = 0;
    /*!
        Callback function, which is called once per application loop to allow the application to
        perform periodic processing.
     */
    virtual void handle_update() = 0;
    /*!
        Callback function, which is called once per application loop after the scene has been
        rendered.
     */
    virtual void handle_draw_world() = 0;
    /*!
        Callback function, which is called once per application loop after the graphical user
        interface has been rendered.
     */
    virtual void handle_draw_interface() = 0;
    /*!
        Callback function to handle window resizing.

        \param clientWidth New width of the client area in pixels.
        \param clientHeight New height of the client area in pixels.
     */
    virtual void handle_resize(int32_t clientWidth, int32_t clientHeight);
    //! Callback function, which is called when the application is activated.
    virtual void handle_activate() = 0;
    //! Callback function, which is called when the application is deactivated.
    virtual void handle_deactivate() = 0;
    //! Runs the application.
    void run();
    //! Closes the application.
    void post_exit();
    /*!
        Returns the application window handle.
        \return Application window handle.
     */
    HWND window() const;
    /*!
        Returns whether the application is in the 'Running' state.

        \return If the application is running, the return value is true. Otherwise the return value
        is false.
     */
    bool is_running() const;
protected:
    //! Hidden constructor.
    application();
protected:
    /*!
        Callback function which is invoked when a display event occurs.
        Handles fullscreen state switches.

        \param[in] sender The sender.
        \param event The event.

        \return Always true.
     */
    virtual bool on_observer_display_event(gfx_display *sender, gfx_display::event_type &event);
private:
    //! Creates the main window.
    void create_main_window();
    //! Updates the game state and displays it.
    void update_and_display();
    //! Updates the game.
    void update_game();
    //! Display the current game state.
    void display_game();
    //! Enters the game message loop.
    void game_message_loop();
    //! Enters the exit-application message loop.
    void exit_message_loop();
private:
    //! State flag, indicating whether the application is currently running.
    bool m_isRunning;
    //! State flag, indicating whether the application is closing.
    bool m_isExiting;
    //! The application window handle.
    HWND m_windowHandle;
    //! The application window width.
    int32_t m_windowWidth;
    //! The application window height.
    int32_t m_windowHeight;
    //! The last frame time.
    uint64_t m_lastTime;
    //! The frame update timer.
    float m_frameUpdateTimer;
    //! The FPS counter.
    uint32_t m_fps;
};


//! Global application singleton.
extern GO_API application *the_application;

GO_END_NAMESPACE
