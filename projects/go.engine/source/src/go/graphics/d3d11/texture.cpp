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


#include "go/graphics/d3d11/texture.h"


#include "go/graphics/d3d11/display.h"
#include "go/graphics/d3d11/device.h"
#include "go/core/log.h"
#include "DirectXTex/DDSTextureLoader/DDSTextureLoader.h"
#include <comdef.h>


// ================================================================================ //
// Helper functions
// ================================================================================ //

static DXGI_FORMAT color_format_to_dxgi_format(go::gfx_color_format format)
{
    return (DXGI_FORMAT)format;
}


static go::gfx_color_format dxgi_format_to_color_format(DXGI_FORMAT format)
{
    return (go::gfx_color_format)format;
}


static DXGI_FORMAT dxgi_dsv_to_srvtex_format(DXGI_FORMAT dsvFormat)
{
    switch(dsvFormat)
    {
    case DXGI_FORMAT_D24_UNORM_S8_UINT: return DXGI_FORMAT_R24G8_TYPELESS;
    case DXGI_FORMAT_D32_FLOAT: return DXGI_FORMAT_R32_TYPELESS;
    case DXGI_FORMAT_D16_UNORM: return DXGI_FORMAT_R16_TYPELESS;
    default: GO_MSGASSERT(false, "unknown depth stencil view format");
    }
    return DXGI_FORMAT_R24G8_TYPELESS;
}


static DXGI_FORMAT dxgi_tex_to_srv_format(DXGI_FORMAT texFormat)
{
    switch(texFormat)
    {
    case DXGI_FORMAT_R24G8_TYPELESS: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    case DXGI_FORMAT_R32_TYPELESS: return DXGI_FORMAT_R32_FLOAT;
    case DXGI_FORMAT_R16_TYPELESS: return DXGI_FORMAT_R16_FLOAT;
    default: GO_MSGASSERT(false, "unknown texture format");
    }
    return DXGI_FORMAT_R24G8_TYPELESS;
}


static uint32_t bytes_per_texel(go::gfx_color_format c)
{
    switch(c)
    {
    case go::gfx_color_format::b8g8r8a8_unorm:
    case go::gfx_color_format::b8g8r8a8_unorm_sRGB:
    case go::gfx_color_format::r8g8b8a8_unorm:
    case go::gfx_color_format::r32_float:
    case go::gfx_color_format::r11g11b10_float:
    case go::gfx_color_format::d24_unorm_s8_uint:
    case go::gfx_color_format::d32_float: return 4;
    case go::gfx_color_format::r8_unorm: return 1;
    
    case go::gfx_color_format::r32g32b32a32_float:
    case go::gfx_color_format::r32g32b32_float: return 16;
    case go::gfx_color_format::r16g16b16a16_float: return 8;
    
    case go::gfx_color_format::r8g8_unorm:
    case go::gfx_color_format::r16_unorm:
    case go::gfx_color_format::r16_float:
    case go::gfx_color_format::d16_unorm: return 2;
    default: GO_MSGASSERT(false, "unknown color format");
    }
    return 0;
}


// ================================================================================ //
// go::gfx_texture member functions
// ================================================================================ //

go::gfx_texture::gfx_texture(gfx_texture_format &format, void *initialData)
{
    reset(format, initialData);
}


go::gfx_texture::gfx_texture(void *dataPtr, size_t dataSize, bool forceSRGB)
{
    if(!reset(dataPtr, dataSize, forceSRGB))
    {
        GO_THROW(data_error, "Failed to create GPU texture.");
    }
}


void go::gfx_texture::release()
{
    m_d3dSRVArray.clear();
    m_d3dSRVLayerArray.clear();
    m_d3dUAVArray.clear();
    m_texture.Reset();
    gfx_shader_resource::release();
}


void go::gfx_texture::reset(gfx_texture_format &format, void *initialData)
{
    auto texFmt = color_format_to_dxgi_format(format.colorFormat);
    _reset(format, texFmt, texFmt, D3D11_BIND_SHADER_RESOURCE, true, initialData);
}


bool go::gfx_texture::reset(void *dataPtr, size_t dataSize, bool forceSRGB)
{
    Microsoft::WRL::ComPtr<ID3D11Resource> resource;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

    auto hr = DirectX::CreateDDSTextureFromMemoryEx(
        GO_INTERNAL g_d3d11Device,
        (uint8_t *)dataPtr, dataSize, 0,
        D3D11_USAGE_IMMUTABLE,
        D3D11_BIND_SHADER_RESOURCE,
        0, 0, forceSRGB, &resource, &srv);
    if(FAILED(hr))
    {
        _com_error err(hr);
        GO_LOGFILE_WARNING("Failed to create DDS from memory: '%s'.\n", err.ErrorMessage());
        return false;
    }

    D3D11_RESOURCE_DIMENSION dim;
    resource->GetType(&dim);

    switch(dim)
    {
    case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
    {
        Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
        resource.As<ID3D11Texture2D>(&texture);
        D3D11_TEXTURE2D_DESC td;
        texture->GetDesc(&td);
        m_format.dimension = 2;
        m_format.width = td.Width;
        m_format.height = td.Height;
        m_format.depth = 1;
        m_format.colorFormat = dxgi_format_to_color_format(td.Format);
        m_format.sampleCount = td.SampleDesc.Count;
        m_format.arraySize = td.ArraySize;
        m_format.mipLevels = td.MipLevels;
        uint32_t flags = td.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE ? kFlagCPUWriteAccess : 0;
        flags |= td.CPUAccessFlags & D3D11_CPU_ACCESS_READ ? kFlagCPUReadAccess : 0;
        m_format.flags = flags;
        break;
    }
    case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
    {
        Microsoft::WRL::ComPtr<ID3D11Texture3D> texture;
        resource.As<ID3D11Texture3D>(&texture);
        D3D11_TEXTURE3D_DESC td;
        texture->GetDesc(&td);
        m_format.dimension = 2;
        m_format.width = td.Width;
        m_format.height = td.Height;
        m_format.depth = td.Depth;
        m_format.colorFormat = dxgi_format_to_color_format(td.Format);
        m_format.sampleCount = 1;
        m_format.arraySize = 1;
        m_format.mipLevels = td.MipLevels;
        uint32_t flags = td.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE ? kFlagCPUWriteAccess : 0;
        flags |= td.CPUAccessFlags & D3D11_CPU_ACCESS_READ ? kFlagCPUReadAccess : 0;
        m_format.flags = flags;
        break;
    }
    case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
    {
        GO_MSGASSERT(false, "1D textures are not implemented yet!");
        break;
    }
    default: GO_MSGASSERT(false, "invalid texture dimension"); break;
    }

    release();
    m_texture = std::move(resource);
    m_d3dSRVArray.push_back(srv);
    
    return true;
}


void go::gfx_texture::_reset(const gfx_texture_format &format, DXGI_FORMAT texFormat,
    DXGI_FORMAT srvFormat, UINT d3dBindFlags, bool splitSRVs, void *initialData)
{
    release();

    if(format.flags & kFlagUAV)
    {
        d3dBindFlags |= D3D11_BIND_UNORDERED_ACCESS;
    }

    D3D11_SUBRESOURCE_DATA srd;
    srd.pSysMem = initialData;
    srd.SysMemPitch = format.width * bytes_per_texel(format.colorFormat);
    srd.SysMemSlicePitch = 0;

    m_format = format;

    switch(format.dimension)
    {
    case 2:
        {
            // Texture
            D3D11_TEXTURE2D_DESC td;
            memset(&td, 0, sizeof(td));
            td.Width = format.width;
            td.Height = format.height;
            td.Format = texFormat;
            td.MipLevels = format.mipLevels;
            td.ArraySize = format.arraySize;
            td.SampleDesc.Count = 1;
            td.BindFlags = d3dBindFlags;
            auto allowCPURead = format.flags & kFlagCPUReadAccess;
            auto allowCPUWrite = format.flags & kFlagCPUWriteAccess;
            td.Usage = allowCPUWrite ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT; // TODO: implement Staging resource for read
            td.CPUAccessFlags = allowCPUWrite ? D3D11_CPU_ACCESS_WRITE : 0;
            td.CPUAccessFlags |= allowCPURead ? D3D11_CPU_ACCESS_READ : 0;
            td.MiscFlags = ((format.mipLevels != 1) && (d3dBindFlags & D3D11_BIND_RENDER_TARGET)) ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

            ID3D11Texture2D *texture = nullptr;
            GO_THROW_HRESULT
            (
                GO_INTERNAL g_d3d11Device->CreateTexture2D(&td, initialData ? &srd : nullptr, &texture),
                "could not create 2D texture"
            );
            m_texture.Attach(texture);

            // Update the actual mip count
            texture->GetDesc(&td);
            m_format.mipLevels = td.MipLevels;

            // Auto-deduce SRV mips
            if(m_format.srvMipLevels == 0)
            {
                m_format.srvMipLevels = td.MipLevels;
            }

            // SRV
            auto srvSize = splitSRVs ? format.arraySize : 1;

            D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
            memset(&srvd, 0, sizeof(srvd));
            srvd.Format = srvFormat;
            srvd.ViewDimension = format.arraySize == 1 ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
            srvd.Texture2D.MipLevels = format.mipLevels;
            srvd.Texture2DArray.MipLevels = srvd.Texture2D.MipLevels;
            srvd.Texture2DArray.ArraySize = splitSRVs ? 1 : format.arraySize;

            m_d3dSRVArray.reserve(srvSize);
            for(auto i = 0; i < srvSize; ++i)
            {
                srvd.Texture2DArray.FirstArraySlice = i; // NOTE: if the SRVs are not split then this will be 0 and we only create 1 SRV

                // View over all MIPs
                Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
                GO_THROW_HRESULT
                (
                    GO_INTERNAL g_d3d11Device->CreateShaderResourceView(m_texture.Get(), &srvd, &srv),
                    "could not create SRV"
                );

                m_d3dSRVArray.push_back(std::move(srv));
            }

            // Create an additional view over all MIP levels
            if(m_format.srvMipLevels != m_format.mipLevels)
            {
                srvd.Texture2D.MipLevels = m_format.srvMipLevels;
                srvd.Texture2DArray.MipLevels = srvd.Texture2D.MipLevels;

                for(auto i = 0; i < srvSize; ++i)
                {
                    srvd.Texture2DArray.FirstArraySlice = i; // NOTE: if the SRVs are not split then this will be 0 and we only create 1 SRV

                    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
                    GO_THROW_HRESULT
                    (
                        GO_INTERNAL g_d3d11Device->CreateShaderResourceView(m_texture.Get(), &srvd, &srv),
                        "could not create SRV"
                    );

                    m_d3dSRVLayerArray.push_back(std::move(srv));
                }
            }

            m_format.arraySize = td.ArraySize;
            m_format.sampleCount = td.SampleDesc.Count;
            break;
        }
    case 3:
        {
            GO_MSGASSERT(false, "3D textures are not implemented yet!");
            break;
        }
    case 1:
        {
            GO_MSGASSERT(false, "1D textures are not implemented yet!");
            break;
        }
    default: GO_ASSERT(false); break;
    }

    if(format.flags & kFlagUAV)
    {
        if(format.uavMipLevels > 1)
        {
            // Only texture 2Ds
            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
            uavDesc.Buffer.FirstElement = 0;
            uavDesc.Buffer.Flags = 0;
            uavDesc.Buffer.NumElements = format.width * format.height;
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
            uavDesc.Format = texFormat;

            m_d3dUAVArray.reserve(format.uavMipLevels);
            for(auto i = 0U; i < format.uavMipLevels; ++i)
            {
                Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav;
                uavDesc.Texture2D.MipSlice = i;
                GO_THROW_HRESULT
                (
                    GO_INTERNAL g_d3d11Device->CreateUnorderedAccessView(m_texture.Get(), &uavDesc, &uav),
                    "could not create UAV"
                );
                m_d3dUAVArray.push_back(std::move(uav));
            }
        }
        else
        {
            Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav;
            GO_THROW_HRESULT
            (
                GO_INTERNAL g_d3d11Device->CreateUnorderedAccessView(m_texture.Get(), nullptr, &uav),
                "could not create UAV"
            );
            m_d3dUAVArray.push_back(std::move(uav));
        }
    }
}


void go::gfx_texture::resize(int32_t newWidth, int32_t newHeight, int32_t newDepth, void *initialData)
{
    m_format.width = newWidth;
    m_format.height = newHeight;
    m_format.depth = newDepth;
    reset(m_format, initialData);
}


// ================================================================================ //
// go::render_target member functions
// ================================================================================ //

go::gfx_render_target::gfx_render_target(gfx_texture_format &format)
{
    reset(format);
}


void go::gfx_render_target::release()
{
    gfx_texture::release();
    m_textureRTVArray.clear();
}


void go::gfx_render_target::reset(gfx_texture_format &format, void *)
{
    format.dimension = 2;
    format.depth = 1;
    format.sampleCount = 1;

    auto texFmt = color_format_to_dxgi_format(format.colorFormat);
    _reset(format, texFmt, texFmt, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, true);

    D3D11_RENDER_TARGET_VIEW_DESC rtvd;
    memset(&rtvd, 0, sizeof(rtvd));
    rtvd.Format = texFmt;
    rtvd.ViewDimension = format.arraySize == 1 ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    rtvd.Texture2DArray.ArraySize = 1;
    
    m_textureRTVArray.clear();
    m_textureRTVArray.reserve(format.arraySize);
    for(auto i = 0; i < format.arraySize; ++i)
    {
        rtvd.Texture2DArray.FirstArraySlice = i;

        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
        GO_THROW_HRESULT
        (
            GO_INTERNAL g_d3d11Device->CreateRenderTargetView(m_texture.Get(), nullptr, &rtv),
            "could not create RTV"
        );
        m_textureRTVArray.push_back(std::move(rtv));
    }
}


// ================================================================================ //
// go::gfx_back_buffer member functions
// ================================================================================ //

go::gfx_back_buffer::gfx_back_buffer()
{
    reset();
}


void go::gfx_back_buffer::resize(int32_t, int32_t, int32_t, void *)
{
}


void go::gfx_back_buffer::reset(gfx_texture_format &format, void *)
{
    reset();
}


void go::gfx_back_buffer::reset()
{
    // Pull the data from the swap chain
    GO_THROW_HRESULT
    (
        GO_INTERNAL g_dxgiSwapChain->GetBuffer(0, IID_ID3D11Texture2D, (void **)&m_texture),
        "could not pull backbuffer reference from swap chain"
    );

    // RTV
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
    GO_THROW_HRESULT
    (
        GO_INTERNAL g_d3d11Device->CreateRenderTargetView(m_texture.Get(), nullptr, &rtv),
        "could not create a RTV of the backbuffer"
    );
    m_textureRTVArray.clear();
    m_textureRTVArray.push_back(std::move(rtv));

    // NOTE: there is no SRV / UAV support for the backbuffer as of now!

    D3D11_TEXTURE2D_DESC td;
    static_cast<ID3D11Texture2D *>(m_texture.Get())->GetDesc(&td);

    m_format.dimension = 2;
    m_format.width = td.Width;
    m_format.height = td.Height;
    m_format.depth = 1;
    m_format.sampleCount = td.SampleDesc.Count;
    m_format.colorFormat = dxgi_format_to_color_format(td.Format);
    m_format.arraySize = td.ArraySize;
    m_format.flags = 0;
    m_format.mipLevels = 1;
    m_format.srvMipLevels = 1;
    m_format.uavMipLevels = 0;
}


// ================================================================================ //
// go::gfx_depth_stencil_buffer member functions
// ================================================================================ //

go::gfx_depth_stencil_buffer::gfx_depth_stencil_buffer(gfx_texture_format &format)
{
    reset(format, nullptr);
}


void go::gfx_depth_stencil_buffer::reset(gfx_texture_format &format, void *)
{
    format.dimension = 2;
    format.depth = 1;
    format.flags = 0;
    format.sampleCount = 1;

    auto d3dFmt = color_format_to_dxgi_format(format.colorFormat);
    auto d3dViewDim = format.arraySize == 1 ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DARRAY;

    CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd{ d3dViewDim, d3dFmt, 0, 0, 1 };

    DXGI_FORMAT texFmt = dxgi_dsv_to_srvtex_format(dsvd.Format);
    DXGI_FORMAT srvFmt = dxgi_tex_to_srv_format(texFmt);
    _reset(format, texFmt, srvFmt, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, false);

    m_textureDSVArray.clear();
    m_textureDSVArray.reserve(format.arraySize);
    for(auto i = 0; i < format.arraySize; ++i)
    {
        dsvd.Texture2DArray.FirstArraySlice = i;

        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv;
        GO_THROW_HRESULT
        (
            GO_INTERNAL g_d3d11Device->CreateDepthStencilView(m_texture.Get(), &dsvd, &dsv),
            "could not create RTV"
        );
        m_textureDSVArray.push_back(std::move(dsv));
    }
}


void go::gfx_depth_stencil_buffer::release()
{
    gfx_texture::release();
    m_textureDSVArray.clear();
}