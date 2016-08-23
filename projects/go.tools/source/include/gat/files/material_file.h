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

#include "gat/defines.h"
#include "go/golib.h"
#include "gat/files/section_file.h"


GAT_BEGIN_NAMESPACE

//! A class, which represents a material.
struct material_file
{
    //! Constructor.
    material_file() = default;
    //! \overload
    material_file(const go::file_path &filePath);
    //! The material name.
    go::file_path name;
    //! The shader to use.
    go::gfx_material_shader_id shaderID = go::gfx_material_shader_id::standard;
    //! Specifies whether culling should be disabled for this mesh.
    bool isTwoSided = false;
    //! The textures.
    go::file_path textures[GO_MATERIAL_MAX_TEXTURE_COUNT];
    //! The target texture format.
    std::string textureFormats[GO_MATERIAL_MAX_TEXTURE_COUNT];
    //! The blend mode.
    go::gfx_blend_mode blendMode = go::gfx_blend_mode::opaque;
    //! Per-material constants.
    std::vector<std::pair<std::string, std::string>> constants;
    /*!
        Loads the material definition from file.

        \param filePath The file path.
     */
    void load(const go::file_path &filePath);
    /*!
        Saves the material definition to a file.

        \param filePath The file path.
     */
    void save(const go::file_path &filePath);
    /*!
        Binds a texture to the material file.

        \param slot The texture slot.
        \param texture The texture value.
        \param format The texture format value.
     */
    template<typename T>
    void bind_texture(T slot, const go::file_path &texture, const std::string &format)
    {
        textures[(uint32_t)slot] = texture;
        textureFormats[(uint32_t)slot] = format;
    }
    /*!
        Binds an RGBA constant to the material file.

        \param sectionFile The section to query.
        \param semantic The constant semantic.
        \param newSemantic The new constant semantic.
        \param defaultValue The default constant value.
     */
    void bind_rgba_constant(const section_file::section_t &section, const std::string &semantic, const std::string &newSemantic, const std::string &defaultValue);
    /*!
        Binds an float constant to the material file.

        \param sectionFile The section to query.
        \param semantic The constant semantic.
        \param newSemantic The new constant semantic.
        \param defaultValue The default constant value.
     */
    void bind_float_constant(const section_file::section_t &section, const std::string &semantic, const std::string &newSemantic, const std::string &defaultValue);
    /*!
        Binds an float constant to the material file.

        \param sectionFile The section to query.
        \param semantic The constant semantic.
        \param newSemantic The new constant semantic.
        \param defaultValue The default constant value.
     */
    void bind_float2_constant(const section_file::section_t &section, const std::string &semantic, const std::string &newSemantic, const std::string &defaultValue);
};

typedef std::vector<material_file> material_file_vector;

GAT_END_NAMESPACE