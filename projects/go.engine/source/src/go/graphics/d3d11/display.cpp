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


#include "go/graphics/d3d11/display.h"


#include "go/graphics/d3d11/device.h"
#include "go/graphics/d3d11/ui/system.h"
#include "go/graphics/d3d11/renderer.h"
#include "go/core/log.h"
#include "go/variables.h"
#include "go/core/string_utility.h"


GO gfx_display *GO the_display = nullptr;
GO ui_system *GO the_ui_system = nullptr;
GO gfx_device *GO the_gfx_device = nullptr;
GO gfx_renderer *GO the_gfx_renderer = nullptr;

IDXGISwapChain *GO_INTERNAL g_dxgiSwapChain = nullptr;
ID3D11Device *GO_INTERNAL g_d3d11Device = nullptr;
ID3D11DeviceContext *GO_INTERNAL g_d3d11ImmediateContext = nullptr;


// ============================================================================== //
// Helper functions
// ============================================================================== //

static bool is_viewport_size_valid(int32_t width, int32_t height)
{
    return (width > 0 && height > 0); // && (width % 16 == 0) && (height % 16 == 0);
}


static void initialize_dxgi_mode_desc(DXGI_MODE_DESC &md, int32_t viewportWidth, int32_t viewportHeight, uint32_t flags)
{
    memset(&md, 0, sizeof(md));
    md.Width = viewportWidth;
    md.Height = viewportHeight;
    md.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

    if(flags & GO gfx_display::synchronize)
    {
        md.RefreshRate.Numerator = 60;
        md.RefreshRate.Denominator = 1;
    }
}


// ================================================================================ //
// go::gfx_display_configuration_ex member functions
// ================================================================================ //

go::gfx_display_configuration_ex::gfx_display_configuration_ex(const cdefault_value &)
{
    targetWindow = 0;
    mode = !gfx_display::fullscreen; // Windowed mode
    viewportWidth = 800;
    viewportHeight = 600;
}


go::gfx_display_configuration_ex::gfx_display_configuration_ex(const cenvironment_value &)
{
    targetWindow = (HWND)(GO_CVAR(application_window).valuel());
    mode = GO_CVAR(display_mode).valuei();

    auto viewportSize = mode & gfx_display::fullscreen ?
        string_to_vector2l<DirectX::XMINT2>(GO_CVAR(fullscreen_viewport).value().c_str()) :
        string_to_vector2l<DirectX::XMINT2>(GO_CVAR(windowed_viewport).value().c_str());
    viewportWidth = viewportSize.x;
    viewportHeight = viewportSize.y;
}


// ============================================================================== //
// go::gfx_display member functions
// ============================================================================== //

go::gfx_display::~gfx_display()
{
    // Exit fullscreen mode
    if(m_dxgiSwapChain)
    {
        m_dxgiSwapChain->SetFullscreenState(FALSE, nullptr);
    }

    // This will not be called if the initialization throws an exception.
    // If that happens, the runtime will be shutdown anyways so we don't have to worry about
    // invalid pointers. (not a nice solution but it works)
    m_uiSystem.reset();
    m_worldRenderer.reset();
    m_worldDevice.reset();

    // Last-minute release of resources
    if(m_d3dImmediateContext)
    {
        m_d3dImmediateContext->ClearState();
        m_d3dImmediateContext->Flush();
    }

    m_d3dImmediateContext.Reset();
    m_dxgiSwapChain.Reset();

    if(m_d3dDevice)
    {
        report_live_d3d_objects();
    }

    the_ui_system = nullptr;
    the_gfx_device = nullptr;
    the_gfx_renderer = nullptr;

    GO_INTERNAL g_dxgiSwapChain = nullptr;
    GO_INTERNAL g_d3d11Device = nullptr;
    GO_INTERNAL g_d3d11ImmediateContext = nullptr;
}


go::gfx_display::gfx_display(const gfx_display_configuration &config)
    : singleton<gfx_display>(the_display),
    observable<gfx_display, gfx_display_event_data &>(), m_canRender(true)
{
    GO_LOGFILE_INFO("Initializing Direct3D11 display driver...\n");

    if(!is_viewport_size_valid(config.viewportWidth, config.viewportHeight))
    {
        GO_THROW(std::invalid_argument, "Invalid viewport size.");
    }

    // Query display DPI
    auto hdc = GetDC(NULL);
    if (hdc)
    {
        m_dpiScaleX = (float)GetDeviceCaps(hdc, LOGPIXELSX) / GO_DISPLAY_DEFAULT_DPI_X;
        m_dpiScaleY = (float)GetDeviceCaps(hdc, LOGPIXELSY) / GO_DISPLAY_DEFAULT_DPI_Y;
        ReleaseDC(NULL, hdc);
    }
    else
    {
        m_dpiScaleX = GO_DISPLAY_DEFAULT_DPI_X;
        m_dpiScaleY = GO_DISPLAY_DEFAULT_DPI_Y;
    }

    // Initialize D3D11
    initialize_d3d11(config);

    m_config = config;

    initialize_world_and_ui();
}


void go::gfx_display::report_live_d3d_objects()
{
    Microsoft::WRL::ComPtr<ID3D11Debug> d3dDebug;
    if(SUCCEEDED(m_d3dDevice.As<ID3D11Debug>(&d3dDebug)))
    {
        d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
    }
}


void go::gfx_display::initialize_d3d11(const gfx_display_configuration &config)
{
    uint32_t createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if (defined(DEBUG) | defined(_DEBUG)) && (GO_USE_D3D_DEBUG_LAYER)
    // Enable debug output
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    GO_LOGFILE_INFO("Initializing Direct3D11 device (Flags=0x%x) and contexts...\n", createDeviceFlags);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };
    int32_t numFeatureLevels = _countof(featureLevels);

    DXGI_SWAP_CHAIN_DESC backBufferDesc;
    memset(&backBufferDesc, 0, sizeof(backBufferDesc));
    initialize_dxgi_mode_desc(backBufferDesc.BufferDesc, config.viewportWidth, config.viewportHeight, config.mode);
    
    backBufferDesc.BufferCount = 2;
    backBufferDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    backBufferDesc.OutputWindow = (HWND)config.targetWindow;
    backBufferDesc.SampleDesc.Count = 1;
    backBufferDesc.SampleDesc.Quality = 0;
    backBufferDesc.Windowed = !(config.mode & fullscreen);
    backBufferDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    backBufferDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // Grab the first non-Intel adapter
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
    auto &&hr = CreateDXGIFactory(__uuidof(IDXGIFactory), &dxgiFactory);
    if(SUCCEEDED(hr))
    {
        for(UINT adapterIndex = 0; SUCCEEDED(hr); ++adapterIndex)
        {
            hr = dxgiFactory->EnumAdapters(adapterIndex, &adapter);
            if(adapter != nullptr)
            {
                // Break if this is a non-Intel adapter
                DXGI_ADAPTER_DESC adapterDesc;
                adapter->GetDesc(&adapterDesc);
                if(adapterDesc.VendorId != 0x8086)
                {
                    break;
                }
            }
        }
    }

    GO_THROW_HRESULT
    (
        D3D11CreateDeviceAndSwapChain(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr,
            createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
            &backBufferDesc, &m_dxgiSwapChain, &m_d3dDevice, nullptr, &m_d3dImmediateContext),
        "could not create D3D11 device and swap chain"
    );

    // Disable ALT+ENTER
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    if(SUCCEEDED(m_d3dDevice.As<IDXGIDevice>(&dxgiDevice)))
    {
        Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
        if(SUCCEEDED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&dxgiAdapter)))
        {
            Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
            if(SUCCEEDED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory)))
            {
                dxgiFactory->MakeWindowAssociation(config.targetWindow, DXGI_MWA_NO_ALT_ENTER);
            }
        }
    }

#if (defined(DEBUG) | defined(_DEBUG)) && (GO_USE_D3D_DEBUG_LAYER)
    // Disable D3D warnings that we know are not an issue
    Microsoft::WRL::ComPtr<ID3D11Debug> d3dDebug;
    if(SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        Microsoft::WRL::ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if(SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
            D3D11_MESSAGE_ID hide[] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET
            };
            D3D11_INFO_QUEUE_FILTER filter;
            memset(&filter, 0, sizeof(filter));
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    GO_INTERNAL g_dxgiSwapChain = m_dxgiSwapChain.Get();
    GO_INTERNAL g_d3d11Device = m_d3dDevice.Get();
    GO_INTERNAL g_d3d11ImmediateContext = m_d3dImmediateContext.Get();

    d3d_debug_name(m_d3dDevice, "Main D3D device");
    d3d_debug_name(m_d3dImmediateContext, "Main D3D context");
    d3d_debug_name(m_dxgiSwapChain, "Main DXGI swap chain");

    GO_LOGFILE_INFO("The Direct3D11 runtime was successfully initialized.\n");
}


void go::gfx_display::initialize_world_and_ui()
{
    the_gfx_device = new gfx_device();
    m_worldDevice.reset(the_gfx_device);

    the_gfx_renderer = new gfx_renderer();
    m_worldRenderer.reset(the_gfx_renderer);

    the_ui_system = new ui_system();
    m_uiSystem.reset(the_ui_system);
}


bool go::gfx_display::resize(int32_t viewportWidth, int32_t viewportHeight)
{
    // TODO: get rid of this and fix shaders to not assume this
    // Round to nearest power of 2
    // viewportWidth = 1U << (int32_t)(log2(viewportWidth - 1) + 1);
    // viewportHeight = 1U << (int32_t)(log2(viewportHeight - 1) + 1);
    /*
    while(viewportWidth % 16 != 0)
    {
        ++viewportWidth;
    }

    while(viewportHeight % 16 != 0)
    {
        ++viewportHeight;
    }
    */

    if(!is_viewport_size_valid(viewportWidth, viewportHeight))
    {
        return false;
    }

    auto bb = the_gfx_device->back_buffer();
    if(bb)
    {
        auto &bbFmt = bb->format();
        if(bbFmt.width == viewportWidth && bbFmt.height == viewportHeight)
        {
            return true;
        }
    }

    m_canRender = false;

    // This needs to be done before anything else (releases all BackBuffer refs by the Renderer)
    the_gfx_renderer->clear_state();

    post_resizing_event();

    GO_MAPC_RETURN_HRESULT
    (
        m_dxgiSwapChain->ResizeBuffers
        (
            0, viewportWidth, viewportHeight,
            DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
        ),
        false
    );

    m_config.viewportWidth = viewportWidth;
    m_config.viewportHeight = viewportHeight;

    post_resized_event();

    m_canRender = true;

    return true;
}


bool go::gfx_display::change_mode(uint32_t mode)
{
    if(this->mode() == mode)
    {
        return true;
    }

    bool enterFullscreen = mode & fullscreen;
    if(enterFullscreen)
    {
        DXGI_MODE_DESC md;
        auto viewport = string_to_vector2l<DirectX::XMINT2>(GO_CVAR(fullscreen_viewport).value().c_str()); 
        initialize_dxgi_mode_desc(md, viewport.x, viewport.y, mode);
        
        // Resize the target buffer
        GO_MAPC_RETURN_HRESULT(m_dxgiSwapChain->ResizeTarget(&md), false);

        post_pre_enter_fullscreen_event();

        // Switch state
        if(FAILED(m_dxgiSwapChain->SetFullscreenState(true, nullptr)))
        {
            // Revert changes
            md.Width = width();
            md.Height = height();
            m_dxgiSwapChain->ResizeTarget(&md);
            return false;
        }

        // Avoids issues with the refresh rate
        md.RefreshRate.Numerator = 0;
        md.RefreshRate.Denominator = 0;
        m_dxgiSwapChain->ResizeTarget(&md);

        m_config.viewportWidth = md.Width;
        m_config.viewportHeight = md.Height;
        m_config.mode = mode;

        post_post_enter_fullscreen_event();
    }
    else
    {
        post_pre_exit_fullscreen_event();

        // Switch state
        if(FAILED(m_dxgiSwapChain->SetFullscreenState(false, nullptr)))
        {
            return false;
        }

        // Needs to be resized externally
        m_config.mode = mode;

        post_post_exit_fullscreen_event();
    }

    return true;
}


int32_t go::gfx_display::width() const
{
    return m_config.viewportWidth;
}


int32_t go::gfx_display::height() const
{
    return m_config.viewportHeight;
}


uint32_t go::gfx_display::mode() const
{
    return m_config.mode;
}


bool go::gfx_display::is_fullscreen() const
{
    return m_config.mode & fullscreen;
}


float go::gfx_display::aspect_ratio() const
{
    return (float)m_config.viewportWidth / (float)m_config.viewportHeight;
}


float go::gfx_display::dpi_scale_x() const
{
    return m_dpiScaleX;
}


float go::gfx_display::dpi_scale_y() const
{
    return m_dpiScaleY;
}


DirectX::XMFLOAT2 go::gfx_display::dpi_scale() const
{
    return DirectX::XMFLOAT2(m_dpiScaleX, m_dpiScaleY);
}


auto go::gfx_display::config() const -> const gfx_display_configuration &
{
    return m_config;
}


void go::gfx_display::post_resized_event()
{
    gfx_display_event_data e = { gfx_display_event_data::event::resized };
    post_event(e);

    e.msg = gfx_display_event_data::event::late_resized;
    post_event(e);
}


void go::gfx_display::post_resizing_event()
{
    gfx_display_event_data e = { gfx_display_event_data::event::resizing };
    post_event(e);
}


void go::gfx_display::post_pre_enter_fullscreen_event()
{
    gfx_display_event_data e = { gfx_display_event_data::event::pre_enter_fullscreen };
    post_event(e);
}


void go::gfx_display::post_post_enter_fullscreen_event()
{
    gfx_display_event_data e = { gfx_display_event_data::event::post_enter_fullscreen };
    post_event(e);
}


void go::gfx_display::post_pre_exit_fullscreen_event()
{
    gfx_display_event_data e = { gfx_display_event_data::event::pre_exit_fullscreen };
    post_event(e);
}


void go::gfx_display::post_post_exit_fullscreen_event()
{
    gfx_display_event_data e = { gfx_display_event_data::event::post_exit_fullscreen };
    post_event(e);
}