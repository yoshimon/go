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


#include "go/hid/system_input_driver.h"


GO system_input_driver *GO the_system_input = nullptr;


//! The focus window.
static HWND s_focusWindow = 0;
//! A hook into the message pump.
static HHOOK s_focusWindowHook = NULL;
//! Mouse button mask.
static ULONG s_mouseButtonsDown = 0;
//! Key state array.
static bool s_keysDown[256] = { false };


// ================================================================================ //
// go::system_input_driver member functions
// ================================================================================ //

static void update_keyboard_state(const RAWKEYBOARD &rawKeyboard)
{
    auto k = rawKeyboard.VKey;
    bool keyPressed;
    bool keyReleased;

    if((rawKeyboard.Flags & RI_KEY_BREAK) == RI_KEY_BREAK)
    {
        // Key released
        s_keysDown[k] = false;
        keyPressed = false;
        keyReleased = true;
    }
    else if((rawKeyboard.Flags & RI_KEY_MAKE) == RI_KEY_MAKE)
    {
        keyPressed = !s_keysDown[k];
        s_keysDown[k] = true;
        keyReleased = false;
    }

    auto keyChanged = keyPressed | keyReleased;
    if(keyChanged)
    {
        GO system_input_driver_event_data e = { GO system_input_driver_event_data::event_t::keyboard_input, keyPressed, k };
        GO the_system_input->post_event(e);
    }
}


static void update_mouse_state(const RAWMOUSE &rawMouse)
{
    GO system_input_driver_event_data e = { GO system_input_driver_event_data::event_t::mouse_input };
    if((rawMouse.usFlags & MOUSE_MOVE_RELATIVE) == MOUSE_MOVE_RELATIVE)
    {
        e.motionDelta.x = rawMouse.lLastX;
        e.motionDelta.y = rawMouse.lLastY;
    }
    else
    {
        GO_ASSERT(false);
    }

    // Update buttons
    ULONG mask = rawMouse.usButtonFlags;
    static const ULONG kAllMouseButtonsDownMask = RI_MOUSE_LEFT_BUTTON_DOWN |
        RI_MOUSE_RIGHT_BUTTON_DOWN |
        RI_MOUSE_MIDDLE_BUTTON_DOWN |
        RI_MOUSE_BUTTON_4_DOWN |
        RI_MOUSE_BUTTON_5_DOWN;
    static const ULONG kAllMouseButtonsUpMask = RI_MOUSE_LEFT_BUTTON_UP |
        RI_MOUSE_RIGHT_BUTTON_UP |
        RI_MOUSE_MIDDLE_BUTTON_UP |
        RI_MOUSE_BUTTON_4_UP |
        RI_MOUSE_BUTTON_5_UP;
    auto buttonsPressed = mask & kAllMouseButtonsDownMask;
    auto buttonsReleased = (mask & kAllMouseButtonsUpMask) >> 1;
    auto buttonsChanged = buttonsPressed | buttonsReleased;
    s_mouseButtonsDown = (s_mouseButtonsDown | buttonsPressed) & ~buttonsReleased;

    auto wasMouseMotionSend = false;
    if(buttonsReleased)
    {
        wasMouseMotionSend = true;
        e.inputMask = buttonsReleased;
        e.wasInputPressed = false;
        GO the_system_input->post_event(e);
        e.motionDelta.x = 0;
        e.motionDelta.y = 0;
    }

    if(buttonsPressed)
    {
        e.inputMask = buttonsPressed;
        e.wasInputPressed = true;
        GO the_system_input->post_event(e);
        if(!wasMouseMotionSend)
        {
            wasMouseMotionSend = true;
            e.motionDelta.x = 0;
            e.motionDelta.y = 0;
        }
    }

    if(!wasMouseMotionSend)
    {
        e.inputMask = 0;
        e.wasInputPressed = false;
        GO the_system_input->post_event(e);
    }
}


static void system_input_raw_input(HRAWINPUT hRawInput)
{
    // Grab input data size
    UINT rawInputBufSize;
    if(GetRawInputData(hRawInput, RID_INPUT, NULL, &rawInputBufSize, sizeof(RAWINPUTHEADER)) != 0)
    {
        return;
    }

    RAWINPUT newInput;

    // See if the buffer is large enough
    if(rawInputBufSize > sizeof(newInput))
    {
        // Buffer not large enough
        return;
    }

    // Grab new input state
    if(GetRawInputData(hRawInput, RID_INPUT, &newInput, &rawInputBufSize, sizeof(RAWINPUTHEADER)) != rawInputBufSize)
    {
        // Previous data buffer does not equal the current buffer..that's not good.
        GO the_system_input->reset();
        return;
    }

    // Copy to persistent buffer
    if(newInput.header.dwType == RIM_TYPEMOUSE)
    {
        update_mouse_state(newInput.data.mouse);
    }
    else if(newInput.header.dwType == RIM_TYPEKEYBOARD)
    {
        update_keyboard_state(newInput.data.keyboard);
    }
}


static LRESULT CALLBACK system_input_wnd_hook_proc(INT Code, WPARAM wParam, LPARAM lParam)
{
    if(Code < 0)
    {
        // Pass-through
        return CallNextHookEx(s_focusWindowHook, Code, wParam, lParam);
    }

    // Handle message
    if(GO the_system_input && GO the_system_input->is_enabled())
    {
        MSG *p = reinterpret_cast<MSG *>(lParam);
        if(p->hwnd == s_focusWindow && p->message == WM_INPUT)
        {
            if(GET_RAWINPUT_CODE_WPARAM(p->wParam) == RIM_INPUT)
            {
                // Update input system
                system_input_raw_input(reinterpret_cast<HRAWINPUT>(p->lParam));
            }
        }
    }

    return CallNextHookEx(s_focusWindowHook, Code, wParam, lParam);
}


// ================================================================================ //
// go::system_input member functions
// ================================================================================ //

go::system_input_driver::system_input_driver()
    : singleton(the_system_input)
{
    enable(true);
}


void go::system_input_driver::enable(bool enabled)
{
    m_enabled = enabled;
    reset();
}


bool go::system_input_driver::is_enabled() const
{
    return m_enabled;
}


bool go::system_input_driver::key_down(uint32_t key) const
{
    return s_keysDown[key];
}


bool go::system_input_driver::key_up(uint32_t key) const
{
    return !s_keysDown[key];
}


bool go::system_input_driver::mouse_down(uint32_t buttonMask) const
{
    return (s_mouseButtonsDown & buttonMask) == buttonMask;
}


bool go::system_input_driver::mouse_up(uint32_t buttonMask) const
{
    return ((~s_mouseButtonsDown) & buttonMask) == buttonMask;
}


bool go::system_input_driver::change_focus_window(HWND hFocusWindow)
{
    if(s_focusWindow != hFocusWindow)
    {
        reset();

        // Unhook previous window
        if(s_focusWindowHook != NULL)
        {
            UnhookWindowsHookEx(s_focusWindowHook);
            s_focusWindowHook = NULL;
        }

        // Initialize new devices
        if(!IsWindow(hFocusWindow))
        {
            // Window does not exist, nothing to do.
            s_focusWindow = 0;
            return false;
        }

        s_focusWindow = hFocusWindow;

        // Install new window hook
        s_focusWindowHook = SetWindowsHookEx(WH_GETMESSAGE, system_input_wnd_hook_proc, GetModuleHandle(NULL), GetCurrentThreadId());
        if(s_focusWindowHook == NULL)
        {
            s_focusWindow = 0;
            return false;
        }

        if(!initialize_devices(hFocusWindow))
        {
            // Remove message hook
            UnhookWindowsHookEx(s_focusWindowHook);
            s_focusWindowHook = NULL;
            s_focusWindow = 0;
            return false;
        }

        s_focusWindow = hFocusWindow;
    }

    return true;
}


HWND go::system_input_driver::focus_window() const
{
    return s_focusWindow;
}


void go::system_input_driver::reset()
{
    // Mouse
    s_mouseButtonsDown = 0;

    // Keyboard
    memset(s_keysDown, 0, sizeof(s_keysDown));
}


bool go::system_input_driver::initialize_devices(HWND hFocusWindow)
{
    RAWINPUTDEVICE rids[2];

    // Keyboard
    rids[0].usUsagePage = 0x01; // Generic
    rids[0].usUsage = 0x06; // Keyboard 
    rids[0].dwFlags = 0;
    rids[0].hwndTarget = hFocusWindow;

    // Mouse
    rids[1].usUsagePage = 0x01; // Generic
    rids[1].usUsage = 0x02; // Mouse
    rids[1].dwFlags = 0;
    rids[1].hwndTarget = hFocusWindow;

    if(!RegisterRawInputDevices(rids, 2, sizeof(RAWINPUTDEVICE)))
    {
        return false;
    }

    return true;
}
