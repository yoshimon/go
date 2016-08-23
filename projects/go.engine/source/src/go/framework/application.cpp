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


#include "go/framework/application.h"


#include "go/variables.h"
#include "go/core/timer.h"
#include "go/core/performance_counter.h"
#include "go/core/string_utility.h"
#include "go/core/path_utility.h"
#include "go/environment/manager.h"
#include "go/graphics/api/ui/system.h"
#include "go/graphics/api/device.h"
#include "go/graphics/api/renderer.h"
#include "go/hid/system_input_driver.h"
#include "go/hid/input_dispatcher.h"
#include "go/platform/windows.h"


GO application *GO the_application = nullptr;
BOOL s_isCursorClipped = FALSE;


// ============================================================================== //
// Static functions
// ============================================================================== //

static void client_to_screen(int32_t &x, int32_t &y)
{
    RECT clientArea = { 0, 0, x, y };
    AdjustWindowRect(&clientArea, WS_OVERLAPPEDWINDOW, false);
    x = clientArea.right - clientArea.left;
    y = clientArea.bottom - clientArea.top;
}


static void resize_window(HWND hWnd, int32_t clientWidth, int32_t clientHeight, bool adjustToWindowRect)
{
    RECT r;
    GetWindowRect(hWnd, &r);
    
    if(adjustToWindowRect)
    {
        client_to_screen(clientWidth, clientHeight);
    }

    MoveWindow(hWnd, r.left, r.top, clientWidth, clientHeight, false);
}


static void clip_cursor()
{
    RECT r;
    GetClientRect(GO the_application->window(), &r);
    ClientToScreen(GO the_application->window(), (POINT *)&r.left);
    ClientToScreen(GO the_application->window(), (POINT *)&r.right);
    s_isCursorClipped = ClipCursor(&r);
}


static void unclip_cursor()
{
    s_isCursorClipped = !ClipCursor(nullptr);
}


static LRESULT CALLBACK MainWndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_ACTIVATE:
        {
            if(go::the_application->is_running())
            {
                if(LOWORD(wParam) != WA_INACTIVE)
                {
                    go::the_application->handle_activate();
                }
                else
                {
                    go::the_application->handle_deactivate();
                    unclip_cursor();
                }
            }
            break;
        }
    case WM_LBUTTONDOWN:
        {
            clip_cursor();
            break;
        }
    case WM_KEYDOWN:
        {
            if(go::the_display)
            {
                if(wParam == VK_F1)
                {
                    if(go::the_display)
                    {
                        go::the_display->change_mode(go::the_display->mode() ^ go::gfx_display::fullscreen);
                        clip_cursor();
                    }
                }
                else
                {
                    go::the_ui_system->inject_key_message(message, wParam, lParam);
                }
            }
            break;
        }
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        {
            if(wParam == VK_F10)
            {
                go::the_application->post_exit();
            }
            else if(go::the_ui_system)
            {
                go::the_ui_system->inject_key_message(message, wParam, lParam);
            }
            break;
        }
    case WM_KEYUP:
        {
            if(go::the_ui_system)
            {
                if(wParam == VK_F9)
                {
                    unclip_cursor();
                }
                else
                {
                    go::the_ui_system->inject_key_message(message, wParam, lParam);
                }
            }
            break;
        }
    case WM_CHAR:
    case WM_IME_CHAR:
    case WM_SYSCHAR:
        {
            if(go::the_ui_system)
            {
                go::the_ui_system->inject_key_message(message, wParam, lParam);
            }
            break;
        }
    case WM_EXITSIZEMOVE:
        {
            RECT r;
            GetClientRect(hWindow, &r);
            if(go::the_application->is_running())
            {
                go::the_application->handle_resize(r.right - r.left, r.bottom - r.top);
            }
            break;
        }
    case WM_SETCURSOR:
        {
            // The UI-system takes care of cursors
            return FALSE;
        }
    case WM_DESTROY:
        {
            unclip_cursor();
            go::the_system_input->change_focus_window(0);
            GO_CVAR(application_window).set_value(0);
            PostQuitMessage(0);
            break;
        }
    case WM_CREATE:
        {
            GO_CVAR(application_window).set_value((int64_t)hWindow);
            break;
        }
    }

    return DefWindowProc(hWindow, message, wParam, lParam);
}


// ============================================================================== //
// go::application member functions
// ============================================================================== //

go::application::application()
    : singleton<application>(the_application), observer(this, &go::application::on_observer_display_event),
    m_isRunning(false), m_isExiting(false), m_windowHandle(NULL), m_windowWidth(640), m_windowHeight(480)
{
    // Reset performance counters
    performance.elapsedTime = 0.0f;
    performance.fps = 0;
    
    create_main_window();
}


go::application::~application()
{
}


void go::application::handle_resize(int32_t width, int32_t height)
{
    m_windowWidth = width;
    m_windowHeight = height;
}


HWND go::application::window() const
{
    return m_windowHandle;
}


void go::application::create_main_window()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    {
        // Register window class
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = MainWndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
        wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = _T("CGoApplicationWindow");
        RegisterClassEx(&wcex);
    }

    // Create window
    m_windowHandle = CreateWindow(_T("CGoApplicationWindow"),
                                  _T("Go (" _T(GO_BUILD_TARGET) " Build ") _T(__DATE__) _T(", ") _T(__TIME__) ")",
                                  WS_OVERLAPPEDWINDOW, // WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  320, 240,
                                  NULL, NULL, hInstance, NULL);

    if(m_windowHandle == NULL)
    {
        return;
    }

    ShowWindow(m_windowHandle, SW_SHOWNORMAL);
    UpdateWindow(m_windowHandle);
}


void go::application::run()
{
    // Change global focus to this window
    go::the_system_input->change_focus_window(m_windowHandle);

    // Handle pre-run exit request
    if(m_isExiting)
    {
        exit_message_loop();
    }
    else
    {
        game_message_loop();
    }

    DestroyWindow(m_windowHandle);
    handle_shutdown();
    m_isRunning = false;
    delete this;
}


void go::application::exit_message_loop()
{
    MSG msg = { 0 };
    do
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    while(msg.message != WM_QUIT);
}


void go::application::game_message_loop()
{
    if(go::the_display)
    {
        // Match window size with display viewport
        resize_window(m_windowHandle, go::the_display->width(), go::the_display->height(), !go::the_display->is_fullscreen());
        handle_resize(the_display->width(), the_display->height());

        // Register as observer for Display events
        go::the_display->add_observer((go::gfx_display::observer_type *)this);
    }

    // Variables for timing
    auto currentTime = timer::query_current_time_cycles();
    auto accumulator = GO_UPDATE_PERIOD;
    
    auto startTime = currentTime;
    go::the_game_time = 0;

    m_isRunning = true;
    MSG msg = { 0 };
    do
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) && msg.message != WM_QUIT)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        if(msg.message == WM_QUIT)
        {
            break;
        }

        update_and_display(accumulator, startTime, currentTime);
    }
    while(true);
}


void go::application::post_exit()
{
    m_isExiting = true;
    PostQuitMessage(0);
}


bool go::application::is_running() const
{
    return m_isRunning;
}


void go::application::update_and_display(float &accumulator, uint64_t startTime, uint64_t &currentTime)
{
    auto newTime = timer::query_current_time_cycles();
    auto frameTime = std::min(GO_MAX_FRAME_TIME, timer::cycles_to_seconds(newTime - currentTime));

    // Increase global game time counter
    go::the_game_time = timer::cycles_to_seconds(newTime - startTime);

    currentTime = newTime;
    accumulator += frameTime;
    
    // TODO: fix your timestep!
    //while(accumulator >= GO_UPDATE_PERIOD)
    //{
        update_game();
    //    accumulator -= GO_UPDATE_PERIOD;
    //}

    performance.blendFactor = accumulator * GO_UPDATE_FREQUENCY;

    display_game();

    auto endTime = timer::query_current_time_cycles();
    performance.elapsedTime = timer::cycles_to_seconds(endTime - newTime);
    performance.fps = (int32_t)(1 / performance.elapsedTime);
}


void go::application::update_game()
{
    // Dispatch input
    go::the_input_dispatcher->update();

    // Update the UI layer
    if(go::the_ui_system)
    {
        go::the_ui_system->update();
    }

    handle_update();

    go::performance.engineFrameTimeMS += (uint32_t)(GO_UPDATE_PERIOD * 1000.f);
}


void go::application::display_game()
{
    if(go::the_gfx_renderer)
    {
        // Start rendering the world
        if(go::the_gfx_renderer->begin_rendering(m_windowWidth, m_windowHeight))
        {
            // Allow user to render stuff
            handle_draw_world();

            // Render the UI on top
            go::the_ui_system->render();

            // Again, allow the user to handle the final backbuffer
            handle_draw_interface();

            // Finish everything up
            go::the_gfx_renderer->end_rendering();
        }
    }
}


bool go::application::on_observer_display_event(gfx_display *sender, gfx_display::event_type &event)
{
    UNREFERENCED_PARAMETER(sender);
    switch(event.msg)
    {
    case gfx_display_event_data::event::post_exit_fullscreen:
        {
            // Restore windowed size
            auto viewport = string_to_vector2l<DirectX::XMINT2>(GO_CVAR(windowed_viewport).value().c_str());
            resize_window(m_windowHandle, viewport.x, viewport.y, true);   
            break;
        }
    }

    return true;
}
