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
    \file graphics/asset_management/material_database.h
    \brief Contains classes for managing different materials.

    \author Gokhan Ozdogan
    \version 1.0
*/


#pragma once


#include "go/core/common.h"
#include "go/asset_management/asset_database.h"
#include "dfx/namespaces/materials/namespaces/constants/namespace_types.h"


GO_BEGIN_NAMESPACE

class gfx_texture_database;
enum class gfx_blend_mode : uint8_t;


//! Enumeration of possible material types.
enum class gfx_material_type : uint32_t
{
    //! The standard material.
    standard = 0,
    //! The unlit material.
    unlit,
    //! The number of material types.
    count
};


//! Enumeration of possible material types.
const char *const gfx_material_type_strings[(uint32_t)gfx_material_type::count]
{
    "standard",
    "unlit"
};


//! Enumeration of possible material shader identifiers.
enum class gfx_material_shader_id : uint8_t
{
    //! The standard material shader.
    standard = 0,
    //! The standard-cutout material shader.
    standard_cutout,
    //! Unlit.
    unlit,
    //! The number of elements in the enumeration.
    count
};


//! A structure, which holds information about a shader.
struct gfx_material_shader_info_type
{
    //! The name of the shader.
    const char *name;
    //! State flag, indicating whether the shader requires an alpha-test.
    bool requiresAlphaTest;
    //! The shader material category.
    gfx_material_type type;
};


/*!
    A table, that contains meta-information about the material shaders.
 
    The order must match the material shader ID order.
 */
const gfx_material_shader_info_type gfx_material_shader_info[(uint32_t)gfx_material_shader_id::count]
{
    { "standard", false, gfx_material_type::standard },
    { "standard_cutout", true, gfx_material_type::standard },
    { "unlit", false, gfx_material_type::unlit },
};


//! A structure, that contains meta information about a material type.
struct gfx_material_info_type
{
    //! The texture semantics.
    std::array<const char *, GO_MATERIAL_MAX_TEXTURE_COUNT> textureSemantics;
	//! The number of used texture slots.
	uint32_t usedTextureCount;
};


/*!
    A table, that contains meta-information about the materials.

    The order must match the material type order.
 */
const gfx_material_info_type gfx_material_info[(uint32_t)gfx_material_type::count]
{
    { { "albedo", "metallicSmoothness", "normal", "occlusion", "emission" }, 5 },
    { { "color" }, 1 },
};


//! Enumeration of the texture semantics in the standard materials.
enum class gfx_standard_material_textures : uint32_t
{
    //! Albedo texture slot.
    albedo = 0,
    //! Metallic smoothness slot.
    metallic_smoothness,
    //! Normal slot.
    normal,
    //! Occlusion slot.
    occlusion,
    //! Emission slot.
    emission,
    //! The number of elements in the enumeration.
    count
};


//! Enumeration of the texture semantics in the unlit materials.
enum class gfx_unlit_material_textures : uint32_t
{
    //! The color texture slot.
    color = 0,
    //! The number of elements in the enumeration.
    count
};


/*!
    Converts a material shader to a string.

    \param shaderName The name of the material shader.
    \param outShaderID The material shader ID.
    
    \return True, if the conversion was successful. Otherwise false.
 */
GO_API extern bool gfx_material_shader_name_to_id(const char *shaderName, go::gfx_material_shader_id &outShaderID);
/*!
    Converts a material texture semantic to a texture slot.

    \param material The material type.
    \param semantic The texture semantic.
    \param outSlot The texture slot.

    \return True, if the conversion was successful. Otherwise false.
 */
GO_API extern bool gfx_material_texture_semantic_to_slot(gfx_material_type materialType, const char *semantic, uint32_t &outSlot);
/*!
    Converts a material shader texture semantic to a texture slot.

    \param shaderID The shader ID.
    \param semantic The texture semantic.
    \param outSlot The texture slot.

    \return True, if the conversion was successful. Otherwise false.
 */
GO_API extern bool gfx_material_shader_texture_semantic_to_slot(go::gfx_material_shader_id shaderID, const char *semantic, uint32_t &outSlot);


//! A structure, which represents a material.
struct gfx_material_definition : public asset_record
{
    //! The material shader.
    gfx_material_shader_id shaderID;
    //! The blend mode.
    gfx_blend_mode blendMode;
    /*!
        Indicates whether the material requires alpha-testing.
     
        NOTE: this is automatically deduced from the shader and is merely for convenience.
     */
    bool32_t requiresAlphaTest;
	//! Indicates, whether the material should be rendered from both sides.
	bool32_t isTwoSided;
    //! The material texture slots.
    rid textureIDs[GO_MATERIAL_MAX_TEXTURE_COUNT];
    struct
    {
        //! The material data
        union
        {
            //! The standard material data.
            dfx_hlsl::materials::constants::standard standard;
        };
    } constants;
};


//! A class, which manages a set of material definitions.
class GO_API gfx_material_database : public asset_database<gfx_material_definition>
{
public:
    typedef std::vector<std::vector<size_t>> material_type_indices_t;
public:
    /*!
        Constructor.

        \param textureDB The texture database.
     */
    gfx_material_database(gfx_texture_database &textureDB);
public:
    /*!
        Returns the flattened standard material constant data.

        \return The flattened standard material constant data.
     */
    const material_type_indices_t &material_type_indices();
protected:
    void process_record(rapidxml::xml_node<> *n, gfx_material_definition &def) override;
private:
    //! The texture database.
    gfx_texture_database &m_textureDB;
    //! The material indices.
    material_type_indices_t m_materialTypeIndices;
};

GO_END_NAMESPACE