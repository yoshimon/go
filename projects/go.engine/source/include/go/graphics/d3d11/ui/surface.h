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
    \file graphics/d3d11/ui/surface.h
    \brief Contains classes managing UI surfaces.

    \author Gokhan Ozdogan
    \version 1.0
 */

#ifdef GO_USE_DIRECT3D11
#pragma once


#include "go/core/common.h"
#include "go/graphics/d3d11/texture.h"
#include "Awesomium/WebCore.h"


GO_BEGIN_INTERNAL_NAMESPACE

//! A class, which implements a D3D awesomium surface factory.
class awesomium_surface_factory : public Awesomium::SurfaceFactory
{
public:
    Awesomium::Surface *CreateSurface(Awesomium::WebView *, int, int) override;
    void DestroySurface(Awesomium::Surface *) override;
};


//! A class, which implements a D3D texture view of a surface.
class awesomium_surface : public Awesomium::Surface
{
public:
    //! Size of the per-pixel data in bytes.
    const int32_t kPixelSize = sizeof(uint32_t);
public:
    /*!
        Constructor.

        \param view The associated web-view.
        \param width The surface width.
        \param height The surface height.
     */
    awesomium_surface(Awesomium::WebView *webView, int width, int height);
public:
    void Paint(unsigned char *, int, const Awesomium::Rect &, const Awesomium::Rect &) override;
    void Scroll(int, int, const Awesomium::Rect &) override;
public:
    //! The world texture.
    gfx_render_target texture;
};

GO_END_INTERNAL_NAMESPACE

#endif // GO_USE_DIRECT3D11
