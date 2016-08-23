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
    \file hid/system_input_driver.h
    \brief Contains common type declarations.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/design/singleton.h"
#include "go/design/observer.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! A structure, which holds information about a System Input event.
struct system_input_driver_event_data
{
    //! Enumeration of possible event types.
    enum class event_t
    {
        //! A keyboard event was occurred.
        keyboard_input,
        //! A mouse event was occurred.
        mouse_input
    };
    //! The event code.
    event_t event;
    /*!
        Flag, which indicates whether the input was pressed or released.

        Only valid for keyboard-input events.
     */
    bool wasInputPressed;
    //! The key or button mask.
    uint32_t inputMask;
    /*!
        Relative movement.

        Only valid for mouse input.
     */
    DirectX::XMINT2 motionDelta;
};


//! A class, which provides access to the system keyboard and mouse.
class GO_API system_input_driver : public singleton<system_input_driver>,
    public observable<system_input_driver, system_input_driver_event_data &>
{
public:
    //! Left mouse button constant.
    static const uint32_t kLeftMouseButton = RI_MOUSE_LEFT_BUTTON_DOWN;
    //! Right mouse button constant.
    static const uint32_t kRightMouseButton = RI_MOUSE_RIGHT_BUTTON_DOWN;
    //! Middle mouse button constant.
    static const uint32_t kMiddleMouseButton = RI_MOUSE_MIDDLE_BUTTON_DOWN;
    //! Button4 mouse button constant.
    static const uint32_t kButton4MouseButton = RI_MOUSE_BUTTON_4_DOWN;
    //! Button5 mouse button constant.
    static const uint32_t kButton5MouseButton = RI_MOUSE_BUTTON_5_DOWN;
public:
    //! Default constructor.
    system_input_driver();
public:
    /*!
        Sets the input query state.

        \param enabled If set to true, the input state will be updated on Update().
     */
    void enable(bool enabled);
    /*!
        \return If the input system is enabled the return value is true. Otherwise the return value
        is false.
     */
    bool is_enabled() const;
    /*!
        Returns whether a key-group is down.

        \param key The key scan-code. See Windows Virtual Key Codes.

        \return If the key is down, then the return value is true. Otherwise the return value is false.
     */
    bool key_down(uint32_t key) const;
    /*!
        Returns whether a button-group is up.

        \param key The key scan-code. See Windows Virtual Key Codes.

        \return If the key is up, then the return value is true. Otherwise the return value is false.
     */
    bool key_up(uint32_t key) const;
    /*!
        Returns whether a button-group is down.

        \param buttonMask The button mask.

        \return If all buttons are down, then the return value is true. Otherwise the return value
        is false.
     */
    bool mouse_down(uint32_t buttonMask) const;
    /*!
        Returns whether a button-group is up.

        \param buttonMask The button mask.

        \return If all buttons are up, then the return value is true. Otherwise the return value is
        false.
     */
    bool mouse_up(uint32_t buttonMask) const;
    /*!
        Binds the focus to a window.

        Switching window focus will destroy previous virtual devices. Call SetFocusWindow with
        hFocusWindow set to zero to free system resources once SystemInput is no longer needed.

        \param[in] hFocusWindow Focus window handle.
        */
    bool change_focus_window(HWND hFocusWindow);
    /*!
        Unbinds the input system from the current window.

        \return Current focus window handle.
     */
    HWND focus_window() const;
    //! Resets all device states.
    void reset();
private:
    /*!
        Initializes mouse an keyboard.

        \param hFocusWindow The focus window handle.

        \return True, if all devices were initialized. Otherwise false.
     */
    bool initialize_devices(HWND hFocusWindow);
private:
    //! State flag.
    bool m_enabled;
};


//! The global System Input instance.
extern GO_API system_input_driver *the_system_input;

GO_END_NAMESPACE
