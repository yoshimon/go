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
    \file graphics/d3d11/texture.h
    \brief Contains classes that represent GPU textures.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/path_utility.h"
#include "go/platform/windows.h"
#include "go/graphics/d3d11/resource.h"


GO_BEGIN_NAMESPACE

//! Enumeration of possible GPU color formats.
enum class gfx_color_format
{
    //! BC color format.
    bc1_unorm = DXGI_FORMAT_BC1_UNORM,
    //! BC color format.
    bc1_unorm_sRGB = DXGI_FORMAT_BC1_UNORM_SRGB,
    //! BC color format.
    bc2_unorm = DXGI_FORMAT_BC2_UNORM,
    //! BC color format.
    bc2_unorm_sRGB = DXGI_FORMAT_BC2_UNORM_SRGB,
    //! BC color format.
    bc3_unorm = DXGI_FORMAT_BC3_UNORM,
    //! BC color format.
    bc3_unorm_sRGB = DXGI_FORMAT_BC3_UNORM_SRGB,
    //! BC color format.
    bc4_unorm = DXGI_FORMAT_BC4_UNORM,
    //! BC color format.
    bc5_unorm = DXGI_FORMAT_BC5_UNORM,
    //! BC6 (UF16) color format.
    bc6h_uf16 = DXGI_FORMAT_BC6H_UF16,
    //! 8-bit per channel BGRA format.
    b8g8r8a8_unorm = DXGI_FORMAT_B8G8R8A8_UNORM,
    //! 8-bit per channel BGRA format using gamma correction.
    b8g8r8a8_unorm_sRGB = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
    //! 8-bit per channel RGBA format.
    r8g8b8a8_unorm = DXGI_FORMAT_R8G8B8A8_UNORM,
    //! 32-bit per channel floating-point RGBA format.
    r32g32b32a32_float = DXGI_FORMAT_R32G32B32A32_FLOAT,
    //! 16-bit per channel floating point RGB format.
    r16g16b16a16_float = DXGI_FORMAT_R16G16B16A16_FLOAT,
    //! 32-bit floating-point RGB format.
    r11g11b10_float = DXGI_FORMAT_R11G11B10_FLOAT,
    //! 32-bit per channel floating-point RGB format.
    r32g32b32_float = DXGI_FORMAT_R32G32B32_FLOAT,
    //! 32-bit floating point single-channel format.
    r32_float = DXGI_FORMAT_R32_FLOAT,
    //! 16-bit floating point single-channel format.
    r16_float = DXGI_FORMAT_R16_FLOAT,
    //! 8-bit luminance format.
    r8_unorm = DXGI_FORMAT_R8_UNORM,
    //! 16-bit luminance format.
    r16_unorm = DXGI_FORMAT_R16_UNORM,
    //! 24-bit depth, 8-bit stencil (uint).
    d24_unorm_s8_uint = DXGI_FORMAT_D24_UNORM_S8_UINT,
    //! 32-bit depth buffer, no stencil (float).
    d32_float = DXGI_FORMAT_D32_FLOAT,
    //! 16-bit depth buffer, no stencil (float).
    d16_unorm = DXGI_FORMAT_D16_UNORM,
    //! 16-bit RG format.
    r8g8_unorm = DXGI_FORMAT_R8G8_UNORM
};


//! A structure, which holds information about the texture format.
struct gfx_texture_format
{
    //! The number of dimensions.
    int32_t dimension;
    //! Horizontal size of the texture in pixels.
    int32_t width;
    //! Vertical size of the texture in pixels.
    int32_t height;
    //! Depth size of the texture in pixels.
    int32_t depth;
    //! The number of array slices.
    int32_t arraySize;
    //! The number of multi-samples.
    int32_t sampleCount;
    //! The color format of the texture.
    gfx_color_format colorFormat;
    //! Object flags.
    uint32_t flags;
    //! Number of mip-map levels.
    uint32_t mipLevels;
    //! Number of SRV mip-map levels.
    uint32_t srvMipLevels;
    //! Number of UAV mip-map levels, if enabled.
    uint32_t uavMipLevels;
    /*!
        Returns a texture descriptor for 2D textures.

        \param width The width of the texture.
        \param height The height of the texture.
        \param colorFormat The color format.

        \return The texture descriptor.
     */
    static gfx_texture_format as_2d(int32_t width, int32_t height, gfx_color_format colorFormat)
    {
        gfx_texture_format fmt;
        fmt.dimension = 2;
        fmt.width = width;
        fmt.height = height;
        fmt.depth = 1;
        fmt.arraySize = 1;
        fmt.sampleCount = 1;
        fmt.colorFormat = colorFormat;
        fmt.flags = 0;
        fmt.mipLevels = 1;
        fmt.srvMipLevels = 1;
        fmt.uavMipLevels = 0;
        return fmt;
    }
};


//! A class, which represents a texture.
class GO_API gfx_texture : public gfx_shader_resource
{
public:
    //! Allow CPU-write access.
    static const uint32_t kFlagCPUWriteAccess = (1 << 1);
    //! Allow CPU-read access.
    static const uint32_t kFlagCPUReadAccess = (1 << 2);
    //! Create an UAV of the resource.
    static const uint32_t kFlagUAV = (1 << 3);
public:
    //! \overload
    gfx_texture() = default;
    /*!
        Constructor.

        \param format The texture format descriptor.
        \param[opt] initialData Optional pointer to the initial texture data.
     */
    gfx_texture(gfx_texture_format &format, void *initialData = nullptr);
    //! \overload
    gfx_texture(void *dataPtr, size_t dataSize, bool forceSRGB);
    //! Destructor.
    virtual ~gfx_texture() {}
public:
    virtual void release() override;
public:
    /*!
        Returns the format of the texture.

        \return The format of the texture.
     */
    const gfx_texture_format &format() const noexcept { return m_format; }
    /*!
        Re-initializes the object.

        \param width The width of the texture.
        \param height The height of the texture.
        \param depth The depth of the texture.
        \param format The texture format.
        \param flags Additional flags.
        \param[opt] initialData Optional pointer to the initial texture data.
     */
    virtual void reset(gfx_texture_format &format, void *initialData = nullptr);
    //! \overload
    virtual bool reset(void *dataPtr, size_t dataSize, bool forceSRGB);
    /*!
        Resizes the texture.

        The contents of the texture will be disposed.

        \param newWidth The new width of the texture.
        \param newHeight The new height of the texture.
        \param newDepth The new depth of the texture.
        \param[opt] initialData Optional pointer to the initial texture data.
     */
    virtual void resize(int32_t newWidth, int32_t newHeight, int32_t newDepth, void *initialData = nullptr);
    /*!
        Returns the internal D3D texture object.

        \return A pointer to the D3D texture object.
     */
    ID3D11Resource *d3d_texture() const noexcept { return m_texture.Get(); }
protected:
    /*!
        Re-initializes the object.

        \param width The width of the texture.
        \param height The height of the texture.
        \param depth The depth of the texture.
        \param depth The depth of the texture.
        \param texFormat The D3D texture format.
        \param srvFormat The D3D SRV format.
        \param flags Bind flag mask.
        \param d3dBindFlags The D3D bind flags.
        \param splitSRVs Indicates, whether multiple SRVs of the resource should be created.
        \param[opt] initialData Optional pointer to the initial texture data.
     */
    void _reset(const gfx_texture_format &format, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, UINT d3dBindFlags,
        bool splitSRVs, void *initialData = nullptr);
protected:
    /*!
        Information about the texture format.
        This should be initialized by the derived class.
     */
    gfx_texture_format m_format;
    //! The texture resource.
    Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
};


//! A class, which provides a surface for World elements to be rendered onto.
class GO_API gfx_render_target : public gfx_texture
{
public:
    //! \overload 
    gfx_render_target() = default;
    /*!
        Constructor.

        \param format The texture format descriptor.
     */
    gfx_render_target(gfx_texture_format &format);
public:
    void release() override;
    void reset(gfx_texture_format &format, void *initialData = nullptr) override;
public:
    /*!
        Returns the internal D3D render-target-view.

        \param index The resource index.

        \return A pointer to the D3D RTV.
     */
    ID3D11RenderTargetView *d3d_rtv(int32_t index = 0) const noexcept { return m_textureRTVArray[index].Get(); }
protected:
    //! A RTV of the texture object.
    std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> m_textureRTVArray;
};


//! A class, which is a specialized, non-resizable render-target for the back-buffer.
class GO_API gfx_back_buffer : public gfx_render_target
{
public:
    //! Constructor.
    gfx_back_buffer();
public:
    void resize(int32_t, int32_t, int32_t, void * = nullptr) override;
    void reset(gfx_texture_format &format, void * = nullptr) override;
public:
    //! Re-initializes the object.
    void reset();
};


//! A class, which represents a depth-stencil buffer.
class GO_API gfx_depth_stencil_buffer : public gfx_texture
{
public:
    //! \overload
    gfx_depth_stencil_buffer() = default;
    /*!
        Constructor.

        \param format The texture format descriptor.
     */
    gfx_depth_stencil_buffer(gfx_texture_format &format);
public:
    void release() override;
    void reset(gfx_texture_format &format, void * = nullptr) override;
public:
    //! \overload
    ID3D11DepthStencilView *d3d_dsv(int32_t index = 0) const noexcept{ return m_textureDSVArray[index].Get(); }
private:
    //! A DSV array of the texture object.
    std::vector<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> m_textureDSVArray;
};

GO_END_NAMESPACE