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
    \file graphics/render_path.h
    \brief Contains an interface for implementing a render path.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/graphics/asset_management/asset_context.h"


GO_BEGIN_NAMESPACE

struct gfx_render_context;

//! A class, which represents an executable render path.
class GO_API gfx_render_path
{
    GO_DECLARE_NON_COPYABLE(gfx_render_path)
public:
    //! Constructor.
    gfx_render_path() = default;
    //! Destructor.
    virtual ~gfx_render_path() = default;
    /*!
        Callback function, which is invoked when the current render context is ready to be rendered.

        \param renderContext The render context to process.
        \param assetContext The asset context to use.
     */
    virtual void render(gfx_render_context &renderContext, gfx_asset_context &assetContext) = 0;
    //! Callback function, which is invoked when the swap-chain is resized.
    virtual void on_resize() = 0;
};

GO_END_NAMESPACE