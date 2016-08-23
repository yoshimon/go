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

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"
#include "go/graphics/d3d11/dfx/namespaces/materials/bindings.h"
#include "go/graphics/d3d11/dfx/render_util.h"
#include "go/graphics/asset_management/material_database.h"


GO_BEGIN_NAMESPACE

struct gfx_render_command_buffers;
class gfx_depth_stencil_state;
class gfx_rasterizer_state;
class gfx_shader;
class gfx_shader_manager;


namespace dfx_util
{
    //! A structure, that holds the shaders used in the Forward+ pipeline.
    struct forward_plus_shader_cache : public common_shader_cache
    {
        //! The ambient lighting shader.
        gfx_shader *ambientLightingPS;
        //! The pre-pass shader to cull spherical lights.
        gfx_shader *cullSphericalLightsCS;
        //! The material shader table.
        gfx_shader *materialShaders[(uint32_t)go::gfx_material_shader_id::count];
        //! Resets the shader cache.
        void reset(gfx_shader_manager *manager);
    };


    //! A structure, which represents 
    struct forward_plus_render_pass
    {
        //! The material database to use.
        gfx_material_database *materialDB;
        //! The geometry mask
        geometry_mask mask;
        //! State flag, indicating whether this is a depth-only pass.
        bool depthOnly;
        //! State flag, indicating whether depth-writes are enabled.
        bool writeDepth;
        //! The command buffers to use.
        const go::gfx_render_command_buffers *cmdBuffers;
        //! The vertex shader to use for unskinned geometry data.
        const go::gfx_shader *unskinnedGeometryVS;
        //! The rasterizer state for back-facing primitives.
        const go::gfx_rasterizer_state *backFacesRS;
        //! The rasterizer state for front-facing primitives.
        const go::gfx_rasterizer_state *frontFacesRS;
        //! The rasterizer state to use for two-sided rendering.
        const go::gfx_rasterizer_state *twoSidedRS;
        //! The DSS to use when depth-writing is enabled.
        const go::gfx_depth_stencil_state *depthTestWriteEnabledDSS;
        //! The DSS to use when depth-writing is disabled.
        const go::gfx_depth_stencil_state *depthTestEnabledDSS;
    };


    //! A class that provides utility functions to render using the Forward+ pipeline.
    class GO_API forward_plus_renderer
    {
        GO_DECLARE_NON_COPYABLE(forward_plus_renderer)
    public:
        //! Standard material texture bindings.
        static go::dfx_materials::texture_bindings kStandardMaterialTextureBindings;
        //! Unlit material texture bindings.
        static go::dfx_materials::texture_bindings kUnlitMaterialTextureBindings;
    public:
        //! Constructor.
        forward_plus_renderer() = default;
        //! \overload
        forward_plus_renderer(gfx_texture_manager *texMgr, gfx_shader_manager *shaderMgr);
    public:
        /*!
            Resets the renderer.

            \param texMgr The texture manager.
            \param shaderMgr The shader manager.
         */
        void reset(gfx_texture_manager *texMgr, gfx_shader_manager *shaderMgr);
        /*!
            Returns the shader cache.

            \return The shader cache.
         */
        const forward_plus_shader_cache &shader_cache() const noexcept { return m_shaderCache; }
        /*!
            Submits a series of Draw-calls to the GPU for rendering.

            \param renderPass The render-pass to execute.
         */
        void submit_draw_calls(const forward_plus_render_pass &renderPass) const;
    private:
        //! The texture manager to use.
        gfx_texture_manager *m_texMgr;
        //! The shader cache.
        forward_plus_shader_cache m_shaderCache;
    };
}

GO_END_NAMESPACE