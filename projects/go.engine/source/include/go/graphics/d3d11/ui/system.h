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
    \file graphics/d3d11/ui/system.h
    \brief Contains a class, which manages a set of UI screens.

    \author Gokhan Ozdogan
    \version 1.0
 */


#pragma once


#include "go/core/common.h"
#include "go/core/log.h"
#include "go/core/path_utility.h"
#include "go/design/observer.h"
#include "go/hid/system_input_driver.h"
#include "go/graphics/d3d11/display.h"
#include "go/graphics/d3d11/texture.h"
#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"
#include "Awesomium/WebCore.h"


GO_BEGIN_INTERNAL_NAMESPACE

//! Load-listener implementation.
struct awesomium_debug_load_listener : Awesomium::WebViewListener::Load
{
    void OnBeginLoadingFrame(Awesomium::WebView *, int64, bool, const Awesomium::WebURL &, bool) override;
    void OnFailLoadingFrame(Awesomium::WebView *caller, int64, bool, const Awesomium::WebURL &, int, const Awesomium::WebString &) override;
    void OnFinishLoadingFrame(Awesomium::WebView *caller, int64, bool, const Awesomium::WebURL &) override;
    void OnDocumentReady(Awesomium::WebView *caller, const Awesomium::WebURL &) override;
};

GO_END_INTERNAL_NAMESPACE


GO_BEGIN_NAMESPACE

//! A class, that implements the UI system.
class GO_API ui_system : public observer<ui_system, gfx_display>, public observer<ui_system, system_input_driver>,
    public observer<ui_system, log>
{
    friend class awesomium_debug_load_listener;
public:
    /*!
        Escapes a string.

        \param s The string to escape.
     */
    static void escape_ui_string(std::string &s);
public:
    //! Constructor.
    ui_system();
    //! Destructor.
    ~ui_system();
public:
    /*!
        Creates a screen from file.

        \param fileName The filename.

        \return The screen index.
     */
    int32_t create_screen_from_file(const file_path &fileName);
    //! Updates the system.
    void update();
    //! Renders all active screens onto the current backbuffer.
    void render();
    //! Returns the UI render target.
    gfx_render_target *render_target() noexcept;
    /*!
        Returns the cursor position in pixels.

        \return The cursor position.
     */
    const DirectX::XMINT2 &cursor_position() const;
    /*!
        Injects a key-down message into the UI-system.

        \param wParam System value.
        \param lParam System value.
     */
    void inject_key_message(UINT message, WPARAM wParam, LPARAM lParam);
    /*!
        Writes an output to the console.

        \param s The string to write to the console.
     */
    void write_console_line(std::string s);
    /*!
        Writes an output to the console.

        \param s The string to write to the console.
     */
    void write_console(std::string s);
    /*!
        Sets the console text.

        \param s The text to set.
     */
    void set_console_text(std::string s);
    /*!
        Unfocuses the active screen.

        Internal-use only.

        \param webView The web-view that requested the un-focuses operation.
     */
    void internal_unfocus_active_screen(Awesomium::WebView *webView);
    /*!
        Hides the active screen.

        Internal-use only.

        \param webView The web-view that requested the hide operation.
     */
    void internal_hide_screen(Awesomium::WebView *webView);
    /*!
        Shows a screen.

        \param index The screen to show.
     */
    void show_screen(int32_t index);
    /*!
        Hides a screen.

        \param index The screen to hide.
     */
    void hide_screen(int32_t index);
    /*!
        Returns whether the UI system has input focus.

        \return If the UI has input focus, the return value is true. Otherwise, the return value is false.
     */
    bool has_input_focus() const noexcept;
    /*!
        Executes a UI script on a screen.

        \param script The script to execute.
     */
    void execute_script(int32_t index, std::string script);
    //! Shows the mouse cursor.
    void show_cursor();
    //! Hides the mouse cursor.
    void hide_cursor();
private:
    //! Initializes the UI effect.
    void initialize_ui_effect();
    //! Initializes Awesomium.
    void initialize_awesomium();
    //! Initializes the debug screen.
    void initialize_debug_screen();
    /*!
        Callback function which is invoked when a display event occurs.
        The back-buffer should be resized accordingly.

        \param sender The sender.
        \param event The event.

        \return Always true.
    */
    bool on_observe_gpu_display(gfx_display *sender, gfx_display::event_type &event);
    /*!
        Callback function which is invoked when a system-input event occurs.

        \param sender The sender.
        \param event The event.

        \return Returns false, if the UI has input focus. Otherwise, the return value is true.
     */
    bool on_observe_system_input(system_input_driver *sender, system_input_driver::event_type &event);
    /*!
        Callback function which is invoked when a log event occurs.

        \param sender The sender.
        \param event The event.

        \return Always true.
     */
    bool on_observe_log(log *sender, log::event_type &event);
private:
    //! Script-callback listener implementation.
    struct awesomium_js_listener : Awesomium::JSMethodHandler
    {
        void OnMethodCall(Awesomium::WebView *, unsigned int, const Awesomium::WebString &, const Awesomium::JSArray &) override;
        Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView*, unsigned int, const Awesomium::WebString &, const Awesomium::JSArray &) override;
    };
    friend struct awesomium_js_listener;
private:
    typedef std::vector<Awesomium::WebView *> webview_vector_t;
private:
    //! The web-core singleton.
    Awesomium::WebCore *m_webCore;
    //! The web-session singleton.
    Awesomium::WebSession *m_webSession;
    //! The D3D surface factory.
    std::unique_ptr<Awesomium::SurfaceFactory> m_d3dSurfaceFactory;
    //! The load-listener.
    internal::awesomium_debug_load_listener m_debugLoadListener;
    //! The Go-runtime callback listener.
    awesomium_js_listener m_jsListener;
    //! Screen vector.
    webview_vector_t m_visibleScreens;
    //! The invisible screens.
    webview_vector_t m_invisibleScreens;
    //! All screens.
    webview_vector_t m_screens;
    //! The active screen.
    Awesomium::WebView *m_activeScreen;
    //! The debug screen.
    Awesomium::WebView *m_debugScreen;
    //! An effect, that renders the UI.
    dfx::effects::overlay *m_renderUIEffect;
    //! The cursor position.
    DirectX::XMINT2 m_cursorPosition;
};

GO_END_NAMESPACE
