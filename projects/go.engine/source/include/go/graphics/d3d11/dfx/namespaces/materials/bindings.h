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
    \file graphics/d3d11/dfx/namespaces/materials/bindings.h
    \brief Contains a class which describes a skydome.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

struct gfx_material_definition;

namespace dfx_materials
{
    /*!
        A structure, which represents a material texture binding table.

        This table translates zero-based indices to actual texture registers.
     */
    struct texture_bindings
    {
        //! The translation table.
        uint32_t table[GO_MATERIAL_MAX_TEXTURE_COUNT];
        //! The number of elements in the table.
        uint32_t elementCount;
    };
    /*!
        Binds a material to the renderer.

        \param materialDefinition The material definition.
        \param materialTextureBindings A table, that maps texture indices to texture registers.
        \param numMaterialTextureBindings The number of textures to bind.
     */
    template<class TextureMgr>
    void bind_material(
        TextureMgr *texMgr,
        go::gfx_material_definition *materialDefinition,
        const uint32_t *materialTextureBindings,
        uint32_t numMaterialTextureBindings)
    {
        bind_material(texMgr, materialDefinition, materialTextureBindings, 0, numMaterialTextureBindings);
    }
    //! \overload
    template<class TextureMgr>
    void bind_material(
        TextureMgr *texMgr,
        go::gfx_material_definition *materialDefinition,
        const uint32_t *materialTextureBindings,
        uint32_t startIndex, uint32_t count)
    {
        auto &matTextureIDs = materialDefinition->textureIDs;
        auto defaultTextures = texMgr->default_textures();

        // Bind each texture separately
        for(size_t i = startIndex, end = startIndex + count; i < end; ++i)
        {
            auto texID = matTextureIDs[i];
            auto tex = texID < 0 ? defaultTextures[i] : *texMgr->get(texID);
            go::the_gfx_renderer->bind_pixel_shader_resource(materialTextureBindings[i], tex, 0);
        }
    }
    /*!
        Binds a material to the renderer.

        \param materialDefinition The material definition.
        \param textureBindings The texture bindings to use.
     */
    template<class TextureMgr>
    void bind_material(
        TextureMgr *texMgr,
        go::gfx_material_definition *materialDefinition,
        const texture_bindings &textureBindings)
    {
        bind_material(texMgr, materialDefinition, textureBindings.table, textureBindings.elementCount);
    }
}

GO_END_NAMESPACE
