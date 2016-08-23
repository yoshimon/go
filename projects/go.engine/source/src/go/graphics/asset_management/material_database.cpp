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


#include "go/graphics/asset_management/material_database.h"


#include "go/filesystem/file.h"
#include "go/core/string_utility.h"
#include "go/graphics/asset_management/texture_database.h"
#include "go/graphics/api/blend_state.h"
#include "go/util/rapidxml_helper.h"


using namespace go;


// ================================================================================ //
// Helper functions
// ================================================================================ //

void read_textures(rapidxml::xml_node<> *n, gfx_material_definition &def, gfx_texture_database &texDB)
{
    // Initialize texture bindings to default textures
    for(auto i = 0; i < _countof(def.textureIDs); ++i)
    {
        def.textureIDs[i] = -1;
    }

    n = n->first_node("textures");
    if(!n)
    {
        return;
    }

    // Read bindings
    auto c = n->first_node("texture");
    while(c)
    {
        auto semantic = c->first_attribute("semantic");
        if(semantic)
        {
            // Find the semantic index
            uint32_t textureSlot;
            auto semanticStr = semantic->value();
            if(!go::gfx_material_shader_texture_semantic_to_slot(def.shaderID, semanticStr, textureSlot))
            {
                GO_LOGFILE_WARNING("'%s' is not a valid texture semantic.\n", semanticStr);
            }
            else
            {
                // Lookup the texture
                auto slotVal = c->value();
                auto texDef = texDB.find(slotVal);
                if(texDef)
                {
                    def.textureIDs[textureSlot] = texDef->id;
                }
                else
                {
                    GO_LOGFILE_WARNING("'%s' is not a valid texture name.\n", slotVal);
                }
            }
        }
        else
        {
            GO_LOGFILE_WARNING("Missing shader variable-usage semantic.\n");
        }

        c = c->next_sibling(c->name());
    }
}


void read_constant(rapidxml::xml_node<> *constants, const char *name, const DirectX::XMFLOAT3 &defaultValue, float output[3])
{
    auto &&n = constants ? constants->first_node(name) : nullptr;
    auto &&value = n ? go::string_to_vector3f<DirectX::XMFLOAT3>(n->value()) : defaultValue;
    output[0] = value.x;
    output[1] = value.y;
    output[2] = value.z;
}


void read_constant(rapidxml::xml_node<> *constants, const char *name, const DirectX::XMFLOAT2 &defaultValue, float output[2])
{
    auto &&n = constants ? constants->first_node(name) : nullptr;
    auto &&value = n ? go::string_to_vector2f<DirectX::XMFLOAT2>(n->value()) : defaultValue;
    output[0] = value.x;
    output[1] = value.y;
}


void read_constant(rapidxml::xml_node<> *constants, const char *name, float defaultValue, float &output)
{
    auto &&n = constants ? constants->first_node(name) : nullptr;
    output = n ? (float)std::atof(n->value()) : defaultValue;
}


// ================================================================================ //
// Global functions
// ================================================================================ //

bool go::gfx_material_shader_name_to_id(const char *shaderName, go::gfx_material_shader_id &outShaderID)
{
    return cstr_to_enum(shaderName, go::gfx_material_shader_info, outShaderID, []
        (const go::gfx_material_shader_info_type &matInfo)
    {
        return matInfo.name;
    });
}


bool go::gfx_material_texture_semantic_to_slot(gfx_material_type materialType,
    const char *semantic, uint32_t &outSlot)
{
    auto &matTextureSemantics = go::gfx_material_info[(uint32_t)materialType];
    GO_FOR_EACH_INDEX(current, matTextureSemantics.textureSemantics)
    {
        auto val = *current.iterator;
        if(!val)
        {
            break;
        }

        if(!strcmp(val, semantic))
        {
            outSlot = current.index;
            return true;
        }
    }

    return false;
}


bool go::gfx_material_shader_texture_semantic_to_slot(go::gfx_material_shader_id shaderID,
    const char *semantic, uint32_t &outSlot)
{
    auto &shaderMetaInfo = go::gfx_material_shader_info[(uint32_t)shaderID];
    return gfx_material_texture_semantic_to_slot(shaderMetaInfo.type, semantic, outSlot);
}


// ================================================================================ //
// go::gfx_material_database member functions
// ================================================================================ //

go::gfx_material_database::gfx_material_database(gfx_texture_database &textureDB)
    : asset_database("materialdb", "materials", "material"), m_textureDB(textureDB)
{
    m_materialTypeIndices.resize((size_t)gfx_material_type::count);
}


auto go::gfx_material_database::material_type_indices() -> const material_type_indices_t &
{
    return m_materialTypeIndices;
}


void go::gfx_material_database::process_record(rapidxml::xml_node<> *n, gfx_material_definition &def)
{
    auto isShaderValid = go::xml_get_enum_attribute(n, "shader", go::gfx_material_shader_info, def.shaderID, go::gfx_material_shader_id::standard, []
        (const gfx_material_shader_info_type *table, size_t i)
    {
        return table[i].name;
    });

    if(!isShaderValid)
    {
        GO_LOGFILE_WARNING("Invalid material shader.\n");
        return;
    }

    // Bind shader data to definition
    auto &matShaderInfo = go::gfx_material_shader_info[(uint32_t)def.shaderID];
    def.requiresAlphaTest = matShaderInfo.requiresAlphaTest;

    // Blend mode
    go::xml_get_enum_attribute(n, "blendmode", gfx_blend_mode_strings, def.blendMode);

    // Two sided
    go::xml_get_bool_attribute(n, "isTwoSided", *(bool *)&def.isTwoSided, false);

    // Material constants
    switch(def.shaderID)
    {
    case gfx_material_shader_id::standard:
    case gfx_material_shader_id::standard_cutout:
    {
        auto &&nConstants = n->first_node("constants");
        read_constant(nConstants, "albedoTint", DirectX::XMFLOAT3(1, 1, 1), def.constants.standard.albedoTint);
        read_constant(nConstants, "cutoutThreshold", 0.5f, def.constants.standard.cutoutThreshold);
        read_constant(nConstants, "emissiveTint", DirectX::XMFLOAT3(1, 1, 1), def.constants.standard.emissiveTint);
        read_constant(nConstants, "emissiveIntensity", 1.0f, def.constants.standard.emissiveIntensity);
        read_constant(nConstants, "metallicScale", 1.0f, def.constants.standard.metallicScale);
        read_constant(nConstants, "roughnessScale", 1.0f, def.constants.standard.roughnessScale);
        read_constant(nConstants, "occlusionScale", 1.0f, def.constants.standard.occlusionScale);
        read_constant(nConstants, "transparency", 0.0f, def.constants.standard.transparency);
        read_constant(nConstants, "uvScaling", DirectX::XMFLOAT2(1, 1), def.constants.standard.uvTiling);
        read_constant(nConstants, "uvBias", DirectX::XMFLOAT2(0, 0), def.constants.standard.uvOffsets);

        // Remap colors to non-linear space
        def.constants.standard.albedoTint[0] *= def.constants.standard.albedoTint[0];
        def.constants.standard.albedoTint[1] *= def.constants.standard.albedoTint[1];
        def.constants.standard.albedoTint[2] *= def.constants.standard.albedoTint[2];

        def.constants.standard.emissiveTint[0] *= def.constants.standard.emissiveTint[0];
        def.constants.standard.emissiveTint[1] *= def.constants.standard.emissiveTint[1];
        def.constants.standard.emissiveTint[2] *= def.constants.standard.emissiveTint[2];

        break;
    }
    }

    // Material textures
    read_textures(n, def, m_textureDB);

    // Register material index
    m_materialTypeIndices[(uint32_t)matShaderInfo.type].push_back(def.id);
}