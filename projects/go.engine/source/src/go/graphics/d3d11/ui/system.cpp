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


#include "go/graphics/d3d11/ui/system.h"


#include "go/environment/manager.h"
#include "go/environment/variable.h"
#include "go/framework/application.h"
#include "go/filesystem/file.h"
#include "go/hid/system_input_driver.h"
#include "go/graphics/d3d11/ui/surface.h"
#include "go/graphics/d3d11/display.h"
#include "go/graphics/d3d11/device.h"
#include "go/graphics/d3d11/renderer.h"
#include "go/graphics/d3d11/dfx/namespaces/effects/overlay.h"
#include "go/core/performance_counter.h"
#include <Awesomium/STLHelpers.h>


using namespace Awesomium;


static Awesomium::WebString s_methodNameConsoleCommand(Awesomium::WSLit("console_command"));
static Awesomium::WebString s_methodNameUnfocus(Awesomium::WSLit("unfocus"));
static Awesomium::WebString s_methodNameHide(Awesomium::WSLit("hide"));


// ================================================================================ //
// go::internal::awesomium_debug_load_listener member functions
// ================================================================================ //

void go::internal::awesomium_debug_load_listener::OnBeginLoadingFrame(Awesomium::WebView *, int64, bool, const Awesomium::WebURL &, bool)
{
}


void go::internal::awesomium_debug_load_listener::OnFailLoadingFrame(Awesomium::WebView *webView, int64, bool, const Awesomium::WebURL &, int, const Awesomium::WebString &)
{
    // TODO: re-route to default surface
}


void go::internal::awesomium_debug_load_listener::OnFinishLoadingFrame(Awesomium::WebView *webView, int64, bool, const Awesomium::WebURL &)
{
}


void go::internal::awesomium_debug_load_listener::OnDocumentReady(Awesomium::WebView *webView, const Awesomium::WebURL &)
{
    the_ui_system->set_console_text("<b>Go API</b> (Build " __DATE__ ", " __TIME__ " for " GO_BUILD_TARGET ")<br>");
}


// ================================================================================ //
// go::internal::awesomium_js_listener member functions
// ================================================================================ //

void go::ui_system::awesomium_js_listener::OnMethodCall(Awesomium::WebView *webView, unsigned int rid,
    const Awesomium::WebString &methodName, const Awesomium::JSArray &args)
{
    if(methodName.Compare(s_methodNameUnfocus) == 0)
    {
        // Unfocus the screen
        the_ui_system->internal_unfocus_active_screen(webView);
    }
    else if(methodName.Compare(s_methodNameHide) == 0)
    {
        // Hide screen
        the_ui_system->internal_hide_screen(webView);
    }
    else if(methodName.Compare(s_methodNameConsoleCommand) == 0)
    {
        // Execute a console command
        if(args.size() == 1)
        {
            auto &ws = args[0].ToString();
            auto len = ws.ToUTF8(nullptr, 0);
            std::string s;
            s.resize(len);
            ws.ToUTF8((char *)s.data(), len);
            switch(the_environment->execute_command(s))
            {
            case environment_manager::parse_result_t::unknown_command:
                {
                    auto var = the_environment->find(s);
                    if(!var)
                    {
                        if(s == "list")
                        {
                            for(auto &it : the_environment->variables())
                            {
                                the_ui_system->write_console_line(it.first + " = " + it.second->value());
                            }
                        }
                        else if(s == "exit")
                        {
                            the_application->post_exit();
                        }
                        else if(s == "log")
                        {
                            std::string logFile;
                            if(binary_file::read_string(GO_LOG_FILE, logFile) == file::io_result::okay)
                            {
                                the_ui_system->write_console_line(std::move(logFile));
                            }
                            else
                            {
                                the_ui_system->write_console_line("Could not load log file.");
                            }
                        }
                        else if(s == "fps")
                        {
                            the_ui_system->write_console_line(std::to_string(performance.fps));
                        }
                        else if(s == "help")
                        {
                            std::vector<std::string> sorted;
                            sorted.reserve(the_environment->variables().size());

                            // Sort variables
                            for(auto &it : the_environment->variables())
                            {
                                sorted.push_back(it.first + " : " + it.second->description());
                            }

                            std::sort(sorted.begin(), sorted.end());

                            for(auto &it : sorted)
                            {
                                the_ui_system->write_console_line(it);
                            }
                        }
                        else
                        {
                            the_ui_system->write_console_line("Variable not found.");
                        }
                    }
                    else
                    {
                        the_ui_system->write_console_line(var->value());
                    }
                    break;
                }
            case environment_manager::parse_result_t::invalid_variable: the_ui_system->write_console_line("Invalid variable name."); break;
            case environment_manager::parse_result_t::missing_variable: the_ui_system->write_console_line("Missing variable name."); break;
            case environment_manager::parse_result_t::missing_value: the_ui_system->write_console_line("Missing variable value."); break;
            }
        }
    }
}


Awesomium::JSValue go::ui_system::awesomium_js_listener::OnMethodCallWithReturnValue(Awesomium::WebView *webView,
    unsigned int rid, const Awesomium::WebString &methodName, const Awesomium::JSArray &args)
{
    return Awesomium::JSValue(0);
}


// ================================================================================ //
// go::ui_system member functions
// ================================================================================ //

go::ui_system::ui_system()
    : observer<ui_system, gfx_display>(this, &go::ui_system::on_observe_gpu_display),
    observer<ui_system, system_input_driver>(this, &go::ui_system::on_observe_system_input),
    observer<ui_system, log>(this, &go::ui_system::on_observe_log),
    m_debugScreen(nullptr), m_activeScreen(nullptr)
{
    initialize_awesomium();
    initialize_ui_effect();
    initialize_debug_screen();

    the_display->add_observer(this);
    the_system_input->add_observer(this);
    the_logger->add_observer(this);
}


go::ui_system::~ui_system()
{
    if(m_webCore)
    {
        try
        {
            // Stop WebViews
            for(auto &screen : m_screens)
            {
                while(screen->IsLoading())
                {
                    screen->Stop();
                    m_webCore->Update();
                    Sleep(20);
                }

                screen->Destroy();
            }

            // WebCore crashes if we don't wait here...(bug)
            m_webSession->Release();
            m_webCore->Shutdown();
        }
        catch(...)
        {
        }
    }
}


void go::ui_system::internal_unfocus_active_screen(Awesomium::WebView *webView)
{
    webView->Unfocus();
    if(webView == m_activeScreen)
    {
        m_activeScreen = nullptr;
    }
}


void go::ui_system::internal_hide_screen(Awesomium::WebView *webView)
{
    auto it = std::find(m_visibleScreens.begin(), m_visibleScreens.end(), webView);
    if(it != m_visibleScreens.end())
    {
        auto index = (int32_t)(it - m_visibleScreens.begin());
        hide_screen(index);
    }
}


void go::ui_system::initialize_awesomium()
{
    // Creates D3D texture surfaces
    m_d3dSurfaceFactory = std::make_unique<go::internal::awesomium_surface_factory>();

    // WebKit
    Awesomium::WebConfig cfg;
#if defined(DEBUG) || defined(_DEBUG)
    cfg.log_level = kLogLevel_Verbose;
#else
    cfg.log_level = kLogLevel_None;
#endif
    m_webCore = WebCore::Initialize(cfg);
    m_webCore->set_surface_factory(m_d3dSurfaceFactory.get());

    {
        WebPreferences prefs;
        prefs.enable_gpu_acceleration = true;
        prefs.enable_web_gl = true;
        prefs.allow_scripts_to_access_clipboard = true;
        m_webSession = m_webCore->CreateWebSession(WSLit(""), prefs);
    }
}


bool go::ui_system::on_observe_gpu_display(gfx_display *, gfx_display::event_type &event)
{
    if(event.msg == gfx_display_event_data::event::resized)
    {
        auto newWidth = the_display->width();
        auto newHeight = the_display->height();

        for(auto &screen : m_screens)
        {
            screen->Resize(newWidth, newHeight);
        }
    }

    return true;
}


bool go::ui_system::on_observe_system_input(system_input_driver *, system_input_driver::event_type &event)
{
    if(!has_input_focus())
    {
        return true;
    }

    if(event.event == system_input_driver_event_data::event_t::mouse_input)
    {
        auto leftButton = event.inputMask & system_input_driver::kLeftMouseButton;
        auto rightButton = event.inputMask & system_input_driver::kRightMouseButton;
        auto middleButton = event.inputMask & system_input_driver::kMiddleMouseButton;
        auto wasPressed = event.wasInputPressed;

        if(wasPressed)
        {
            // Down
            if(leftButton)
            {
                m_activeScreen->InjectMouseDown(Awesomium::kMouseButton_Left);
            }

            if(rightButton)
            {
                m_activeScreen->InjectMouseDown(Awesomium::kMouseButton_Right);
            }

            if(middleButton)
            {
                m_activeScreen->InjectMouseDown(Awesomium::kMouseButton_Middle);
            }
        }
        else
        {
            // Up
            if(leftButton)
            {
                m_activeScreen->InjectMouseUp(Awesomium::kMouseButton_Left);
            }

            if(rightButton)
            {
                m_activeScreen->InjectMouseUp(Awesomium::kMouseButton_Right);
            }

            if(middleButton)
            {
                m_activeScreen->InjectMouseUp(Awesomium::kMouseButton_Middle);
            }
        }
    }

    return false;
}


void go::ui_system::inject_key_message(UINT message, WPARAM wParam, LPARAM lParam)
{
    if(m_activeScreen)
    {
        Awesomium::WebKeyboardEvent wbe(message, wParam, lParam);
        m_activeScreen->InjectKeyboardEvent(wbe);
    }
}


void go::ui_system::write_console_line(std::string s)
{
    escape_ui_string(s);
    std::string js = "go_devconsole_append_line('";
    js.append(s);
    js.append("')");
    m_debugScreen->ExecuteJavascript(Awesomium::ToWebString(js), Awesomium::WebString());
}


void go::ui_system::write_console(std::string s)
{
    escape_ui_string(s);
    std::string js = "go_devconsole_append('";
    js.append(s);
    js.append("')");
    m_debugScreen->ExecuteJavascript(Awesomium::ToWebString(js), Awesomium::WebString());
}


void go::ui_system::execute_script(int32_t index, std::string script)
{
    m_screens[index]->ExecuteJavascript(Awesomium::ToWebString(script), Awesomium::WebString());
}


void go::ui_system::set_console_text(std::string s)
{
    escape_ui_string(s);
    std::string js = "go_devconsole_set('";
    js.append(s);
    js.append("')");
    m_debugScreen->ExecuteJavascript(Awesomium::ToWebString(js), Awesomium::WebString());
}


void go::ui_system::initialize_ui_effect()
{
    auto &&renderPath = (dfx_forward_plus_render_path *)go::the_gfx_renderer->render_path();
    m_renderUIEffect = &renderPath->effects().overlay;
}


void go::ui_system::initialize_debug_screen()
{
    create_screen_from_file(GO_PATH_SYMBOL_UI_BASE "html5/debug/main.html");
}


int32_t go::ui_system::create_screen_from_file(const file_path &fileName)
{
    auto &&webView = m_webCore->CreateWebView(the_display->width(), the_display->height(), m_webSession);
    if(!webView)
    {
        return -1;
    }

    // Register screen
    auto &&screenIndex = (int32_t)m_screens.size();
    m_screens.emplace_back(webView);

    // Build URL
    auto &&urlPath = "file://" + std::string(file_path(fileName));
    WebURL url(Awesomium::ToWebString(urlPath));

    // Setup global environment
    {
        webView->set_js_method_handler(&m_jsListener);
        
        auto &&jsValue = webView->CreateGlobalJavascriptObject(WSLit("go"));
        auto &&jsVarGo = jsValue.ToObject();
        jsVarGo.SetProperty(WSLit("build_target"), WSLit(GO_BUILD_TARGET));
        jsVarGo.SetCustomMethod(s_methodNameConsoleCommand, false);
        jsVarGo.SetCustomMethod(s_methodNameUnfocus, false);
        jsVarGo.SetCustomMethod(s_methodNameHide, false);   
    }

    // Configure WebView properties
    webView->SetTransparent(true);

    // The first screen is always the debug screen
    if(!m_debugScreen)
    {
        m_debugScreen = webView;

        // Special load listener for debugging
        webView->set_load_listener((WebViewListener::Load *)&m_debugLoadListener);
    }

    // Load the page
    webView->LoadURL(url);

    return screenIndex;
}


void go::ui_system::show_screen(int32_t index)
{
    if(index >= (int32_t)m_screens.size())
    {
        return;
    }

    auto &&s = m_screens[index];
    auto &&it = std::find(m_visibleScreens.begin(), m_visibleScreens.end(), s);
    if(it == m_visibleScreens.end())
    {
        m_visibleScreens.push_back(s);
        m_activeScreen = s;
        s->Focus();
        s->ResumeRendering();
    }
}


void go::ui_system::hide_screen(int32_t index)
{
    if(index >= (int32_t)m_screens.size())
    {
        return;
    }

    auto &&s = m_screens[index];
    auto &&it = std::find(m_visibleScreens.begin(), m_visibleScreens.end(), s);
    if(it != m_visibleScreens.end())
    {
        s->Unfocus();
        s->PauseRendering();

        if(s == m_activeScreen)
        {
            m_activeScreen = nullptr;
        }

        m_visibleScreens.erase(it);
    }
}


const DirectX::XMINT2 &go::ui_system::cursor_position() const
{
    return m_cursorPosition;
}


bool go::ui_system::has_input_focus() const noexcept
{
    return m_activeScreen != nullptr;
}


void go::ui_system::show_cursor()
{
    SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
}


void go::ui_system::hide_cursor()
{
    SetCursor(NULL);
}


void go::ui_system::update()
{
    // Update cursor
    if(GetCursorPos((LPPOINT)&m_cursorPosition))
    {
        ScreenToClient(go::the_system_input->focus_window(), (LPPOINT)&m_cursorPosition);
        
        if(m_activeScreen)
        {
            m_activeScreen->InjectMouseMove(m_cursorPosition.x, m_cursorPosition.y);
        }
    }

    // Update frame stats on the debug screen
    if(std::find(m_visibleScreens.begin(), m_visibleScreens.end(), m_debugScreen) != m_visibleScreens.end())
    {
        std::string fps(std::to_string(go::performance.fps));
        std::string js = "go_statusbar_update_fps('";
        js.append(fps);
        js.append("')");
        m_debugScreen->ExecuteJavascript(Awesomium::ToWebString(js), Awesomium::WebString());
    }

    // Update WebKit
    m_webCore->Update();
}


void go::ui_system::render()
{
    GO_PERF_BEGIN_EVENT("UI");
    the_gfx_renderer->bind_depth_stencil_state(nullptr, 0);
    the_gfx_renderer->bind_blend_state(&the_gfx_device->common_blend_states().alpha);

    the_gfx_renderer->bind_render_target(0, the_gfx_device->back_buffer(), 0);

    for(auto &screen : m_visibleScreens)
    {
        auto surface = (GO_INTERNAL awesomium_surface *)screen->surface();
        if(surface)
        {
			dfx_effects::overlay_parameters overlayParams;
			overlayParams.in.overlayTexture = &surface->texture;
			overlayParams.out.outputTexture = the_gfx_device->back_buffer();
            m_renderUIEffect->run(&overlayParams);
        }
    }
    GO_PERF_END_EVENT;
}


bool go::ui_system::on_observe_log(log *, log::event_type &event)
{
    write_console(event.text);
    return true;
}


void go::ui_system::escape_ui_string(std::string &s)
{
    for(size_t i = 0; i < s.size(); ++i)
    {
        /*
        switch(s[i])
        {
        case '\r': s.replace(i, 1, ""); --i; break;
        case '\n': s.replace(i, 1, "\\n"); ++i; break;
        case '\"': s.replace(i, 1, "\\\""); ++i; break;
        case '\'': s.replace(i, 1, "\\'"); ++i; break;
        case '\\': s.replace(i, 1, "\\\\"); ++i; break;
        }
        */
        switch(s[i])
        {
        case '\r': s.replace(i, 1, ""); --i; break;
        case '\n': s.replace(i, 1, "<br>"); i += 3; break;
        case '\t': s.replace(i, 1, "    "); i += 3; break;
        case '\"': s.replace(i, 1, "\\\""); ++i; break;
        case '\'': s.replace(i, 1, "\\'"); ++i; break;
        case '\\': s.replace(i, 1, "\\\\"); ++i; break;
        }
    }
}