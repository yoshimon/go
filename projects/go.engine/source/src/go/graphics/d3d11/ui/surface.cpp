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


#include "go/graphics/d3d11/ui/surface.h"


#include "go/core/string_utility.h"
#include "go/graphics/d3d11/device.h"
#include "go/graphics/d3d11/ui/system.h"


// ================================================================================ //
// go::internal::awesomium_surface_factory member functions
// ================================================================================ //

Awesomium::Surface *go::internal::awesomium_surface_factory::CreateSurface(Awesomium::WebView *view, int width, int height)
{
    return new awesomium_surface(view, width, height);
}


void go::internal::awesomium_surface_factory::DestroySurface(Awesomium::Surface *surface)
{
    delete (awesomium_surface *)surface;
}


// ================================================================================ //
// go::internal::awesomium_surface member functions
// ================================================================================ //

go::internal::awesomium_surface::awesomium_surface(Awesomium::WebView *webView, int width, int height)
{
    auto &&fmt = go::gfx_texture_format::as_2d(width, height, gfx_color_format::b8g8r8a8_unorm);
    texture.reset(fmt);
}


void go::internal::awesomium_surface::Paint(unsigned char *image, int rowSpan, const Awesomium::Rect &srcRect, const Awesomium::Rect &destRect)
{
    GO_ASSERT(srcRect.width == destRect.width && srcRect.height == destRect.height);

    // Update texture
    D3D11_BOX destRegion;
    destRegion.left = destRect.x;
    destRegion.right = destRect.x + destRect.width;
    destRegion.top = destRect.y;
    destRegion.bottom = destRect.y + destRect.height;
    destRegion.front = 0;
    destRegion.back = 1;

    image += srcRect.x * kPixelSize + srcRect.y * rowSpan;
    
    GO_INTERNAL g_d3d11ImmediateContext->UpdateSubresource(texture.d3d_texture(), 0, &destRegion, image, rowSpan, 0);
}


void go::internal::awesomium_surface::Scroll(int dx, int dy, const Awesomium::Rect &clipRect)
{
    D3D11_BOX srcRegion;
    srcRegion.left = clipRect.x;
    srcRegion.right = clipRect.x + clipRect.width;
    srcRegion.top = clipRect.y;
    srcRegion.bottom = clipRect.y + clipRect.height;
    srcRegion.front = 0;
    srcRegion.back = 1;

    // The documentation say that it's not cool to stick in the same subresource twice...but it seems to work?
    GO_INTERNAL g_d3d11ImmediateContext->CopySubresourceRegion(texture.d3d_texture(), 0, clipRect.x + dx, clipRect.y + dy, 0, texture.d3d_texture(), 0, &srcRegion);
}