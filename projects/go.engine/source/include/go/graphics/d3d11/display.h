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
    \file graphics/d3d11/display.h
    \brief Contains a class which represents a GPU-based display.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once

#include "go/core/common.h"
#include "go/platform/windows.h"
#include "go/design/singleton.h"
#include "go/design/observer.h"


GO_BEGIN_NAMESPACE

class ui_device;
class ui_renderer;
class ui_system;
class gfx_device;
class gfx_renderer;


//! A structure, which encodes an event within a display driver.
struct GO_API gfx_display_event_data
{
    //! Possible display event codes.
    enum class event
    {
        //! The display is being resized.
        resizing,
        //! The display has been resized.
        resized,
        //! Called after the resized event is completed.
        late_resized,
        //! The fullscreen-state is about to be entered.
        pre_enter_fullscreen,
        //! The fullscreen-state is has been entered.
        post_enter_fullscreen,
        //! The fullscreen-state is about to be been exited.
        pre_exit_fullscreen,
        //! The fullscreen-state has been exited.
        post_exit_fullscreen
    };
    //! The event message.
    event msg;
};


//! A structure, which holds all the initialization parameters needed for a display driver.
struct gfx_display_configuration
{
    //! The target window to draw onto.
    HWND targetWindow;
    //! Height of the viewport in pixels.
    int32_t viewportHeight;
    //! Width of the viewport in pixels.
    int32_t viewportWidth;
    /*!
        Optional flag-mask to change the display mode.
        See the flags parameter in #gpu_display::change_mode for a list of possible values.
     */
    uint32_t mode;
};


//! A class, which manages the display properties and related resources.
class GO_API gfx_display : public singleton<gfx_display>,
    public observable<gfx_display, gfx_display_event_data &>
{
    GO_DECLARE_NON_COPYABLE(gfx_display)
public:
    //! The engine is shown full-screen, and no operating system controls are visible.
    static const uint32_t fullscreen = (1 << 0);
    //! Synchronizes the framebuffer display rate with the monitor refresh rate (VSYNC).
    static const uint32_t synchronize = (1 << 1);
public:
    /*!
        Constructs the object and initializes it.

        \param config The configuration parameters.
     */
    gfx_display(const gfx_display_configuration &config);
    //! Destructor.
    ~gfx_display();
public:
    /*!
        Returns, whether the driver is ready to accept rendering commands.

        \return State flag, indicating whether the device is ready to accept rendering commands.
     */
    bool can_render() const { return m_canRender; }
    /*!
        Changes the display mode.

        \param mode Controls various options for the display mode. It can be a combination
        (through logical OR) of the following values: #fullscreen, #synchronize.

        \return If the display mode was successfully changed to the new mode, the return value is
        true. Otherwise the return value is false.
     */
    bool change_mode(uint32_t mode);
    /*!
        Resizes the viewport.

        \param viewportWidth New width of the viewport in pixels.
        \param viewportHeight New height of the viewport in pixels.

        \return If the display was successfully resized, the return value is true. Otherwise the
        return value is false.
     */
    bool resize(int32_t viewportWidth, int32_t viewportHeight);
    /*!
        Returns the height of the viewport.

        \return Height of the viewport in pixels.
     */
    int32_t height() const;
    /*!
        Returns the current width of the viewport.
        
        \return Width of the viewport in pixels.
     */
    int32_t width() const;
    /*!
        Returns the current display mode.
        
        \return Display mode.
     */
    uint32_t mode() const;
    /*!
        Returns whether the display is currently in fullscreen-mode.
        
        \return If the display is in fullscreen-mode, the return value is true. Otherwise the
        return value is false.
     */
    bool is_fullscreen() const;
    /*!
        Returns the aspect ratio of the display.
        
        \return The current aspect ratio.
     */
    float aspect_ratio() const;
    /*!
        Returns the horizontal DPI scaling factor of the display.
        
        \return Horizontal DPI scaling factor of the display.
     */
    float dpi_scale_x() const;
    /*!
        Returns the vertical DPI scaling factor of the display.
        
        \return Vertical DPI scaling factor of the display.
     */
    float dpi_scale_y() const;
    /*!
        Returns the horizontal and vertical DPI of the display.
        
        \return The DPI of the display.
     */
    DirectX::XMFLOAT2 dpi_scale() const;
    //! Reports live D3D objects.
    void report_live_d3d_objects();
    /*!
        Converts pixel coordinates to DPI-scaled coordinates.

        \tparam T The pixel coordinate class.

        \param px The pixel coordinates to convert.
     */
    template<typename TPixel>
    void pixel_to_dpi(TPixel &px) const { px.x *= m_dpiScaleX; px.y *= m_dpiScaleY; }
    //! \overload
    template<typename T>
    T pixel_to_dpi(const T &px) const { T o; o.x = px.x * m_dpiScaleX; o.y = px.y * m_dpiScaleY; return o; }
    //! \overload
    template<typename T>
    DirectX::XMFLOAT2 pixel_to_dpi(const T &x, const T &y) const { DirectX::XMFLOAT2 o; o.x = x * m_dpiScaleX; o.y = y * m_dpiScaleY; return o; }
    /*!
        Converts DPI-scaled coordinates to pixel coordinates.

        \tparam T The DPI-scaled coordinate class.

        \param dpi The DPI-scaled coordinates.
     */
    template<typename T>
    void dpi_to_pixel(T &dpi) const { dpi.x /= m_dpiScaleX; dpi.y /= m_dpiScaleY; }
    //! \overload
    template<typename T>
    T dpi_to_pixel(const T &dpi) const { T o; o.x = dpi.x * m_dpiScaleX; o.y = dpi.y * m_dpiScaleY; return o; }
    /*
        Converts DIPs to physical pixels.

        \tparam T The DIP coordinate class.
     
        \param dip The DIPs.
     */
    template<typename T>
    void dip_to_pixel(T &dip) const { dip.x = floorf(dip.x * dpi.x / GO_DISPLAY_DEFAULT_DPI_X + 0.5f); dip.y = floorf(dip.y * dpi.y / GO_DISPLAY_DEFAULT_DPI_Y + 0.5f); }
    /*
        \overload
     */
    template<typename T>
    T dip_to_pixel(const T &dpi) const { T o; o.x = floorf(dip.x * dpi.x / GO_DISPLAY_DEFAULT_DPI_X + 0.5f); o.y = floorf(dip.y * dpi.y / GO_DISPLAY_DEFAULT_DPI_Y + 0.5f); return o; }
    /*!
        Returns the current display configuration.
        
        \return The current display configuration.
     */
    const gfx_display_configuration &config() const;
protected:
    //! Posts a message to the event queue.
    void post_resizing_event();
    //! Posts a message to the event queue.
    void post_resized_event();
    //! Posts a message to the event queue.
    void post_pre_enter_fullscreen_event();
    //! Posts a message to the event queue.
    void post_post_enter_fullscreen_event();
    //! Posts a message to the event queue.
    void post_pre_exit_fullscreen_event();
    //! Posts a message to the event queue.
    void post_post_exit_fullscreen_event();
private:
    //! Initializes Direct3D11.
    void initialize_d3d11(const gfx_display_configuration &config);
    //! Initializes the UI system and the world.
    void initialize_world_and_ui();
private:
    //! Indiciates, whether the driver is ready to render.
    bool m_canRender;
    //! The display configuration.
    gfx_display_configuration m_config;
    //! The DPI scale along the horizontal axis.
    float m_dpiScaleX;
    //! The DPI scale along the vertical axis.
    float m_dpiScaleY;
    //! Global UI system instance.
    std::unique_ptr<ui_system> m_uiSystem;
    //! Global World device instance.
    std::unique_ptr<gfx_device> m_worldDevice;
        //! Global World renderer instance.
    std::unique_ptr<gfx_renderer> m_worldRenderer;
    //! Global DXGI swap-chain instance.
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_dxgiSwapChain;
    //! Global Direct3D device instance.
    Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
    //! Global Direct3D immediate device context.
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;
};


GO_DECLARE_ENVIRONMENT_HELPER_CLASS_EX(gfx_display_configuration, gfx_display_configuration_ex)


//! Global display driver instance.
extern GO_API gfx_display *the_display;
//! Global UI system instance.
extern GO_API ui_system *the_ui_system;
//! Global world device instance.
extern GO_API gfx_device *the_gfx_device;
//! Global world renderer instance.
extern GO_API gfx_renderer *the_gfx_renderer;

GO_END_NAMESPACE


GO_BEGIN_INTERNAL_NAMESPACE

// DXGI
//! A global pointer for the DXGI swap-chain.
extern IDXGISwapChain *g_dxgiSwapChain;

// D3D11
//! A global pointer for the D3D11 device.
extern ID3D11Device *g_d3d11Device;
//! A global pointer for the D3D11 immediate context.
extern ID3D11DeviceContext *g_d3d11ImmediateContext;

GO_END_INTERNAL_NAMESPACE