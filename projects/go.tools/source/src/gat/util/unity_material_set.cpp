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

#include "gat/util/unity_material_set.h"

#include "gat/util/exception.h"
#include "gat/files/section_file.h"


//===========================================================================
// gat::material_set
//===========================================================================

gat::unity_material_set::unity_material_set(const go::file_path &filePath)
{
    reset(filePath);
}

void gat::unity_material_set::reset(const go::file_path &filePath)
{
    auto &uniqueMaterials = m_entries;
    uniqueMaterials.clear();

    // Parse material file
    section_file matFile(filePath);
    auto &sections = matFile.sections();

    // Load materials
    for(auto &section : sections)
    {
        entry_t entry;
        entry.material.name = section.name;

        // Assume it's an opaque material
        entry.material.blendMode = go::gfx_blend_mode::opaque;
        entry.material.isTwoSided = false;

        auto unityShaderName = section.mappings.find("$shader")->second;
        if(unityShaderName == "Nature/SpeedTree")
        {
            entry.material.shaderID = go::gfx_material_shader_id::standard_cutout;
            entry.material.isTwoSided = true;
        }
        else if(unityShaderName == "Unlit/Texture")
        {
            entry.material.shaderID = go::gfx_material_shader_id::unlit;
        }
        else
        {
            entry.material.shaderID = go::gfx_material_shader_id::standard;
            if(unityShaderName != "Standard")
            {
                _tprintf("[WARNING] Unity material shader '%s' is not supported. Using 'Standard' shader instead.\n", unityShaderName.c_str());
            }

            // Resolve the shader mode
            auto &&sShaderMode = section.mappings.find("$shaderMode");
            if(sShaderMode != section.mappings.end())
            {
                auto &&shaderMode = sShaderMode->second;

                // TODO: use proper blend mode here
                if(shaderMode == "fade" || shaderMode == "transparent")
                {
                    entry.material.isTwoSided = true;
                    entry.material.blendMode = go::gfx_blend_mode::alpha;
                }
                else if(shaderMode == "cutout")
                {
                    entry.material.isTwoSided = true;
                    entry.material.shaderID = go::gfx_material_shader_id::standard_cutout;
                }
            }
        }

        auto materialType = go::gfx_material_shader_info[(uint32_t)entry.material.shaderID].type;
        switch(materialType)
        {
        case go::gfx_material_type::standard:
        {
            // NOTE: this avoids having to detect textures with an alpha channel
            auto albedoFmt = "BC3_UNORM_SRGB"; // entry.material.blendMode == go::gfx_blend_mode::opaque ? "BC1_UNORM_SRGB" : "BC3_UNORM_SRGB";
            entry.material.bind_texture(go::gfx_standard_material_textures::albedo, section.opt_mapping("_MainTex"), albedoFmt);
            entry.material.bind_texture(go::gfx_standard_material_textures::metallic_smoothness, section.opt_mapping("_MetallicGlossMap"), "BC3_UNORM");
            entry.material.bind_texture(go::gfx_standard_material_textures::normal, section.opt_mapping("_BumpMap"), "BC5_UNORM");
            entry.material.bind_texture(go::gfx_standard_material_textures::occlusion, section.opt_mapping("_OcclusionMap"), "BC4_UNORM");
            entry.material.bind_texture(go::gfx_standard_material_textures::emission, section.opt_mapping("_EmissionMap"), "BC1_UNORM");
            
            entry.material.bind_rgba_constant(section, "_Color", "albedoTint", "1 1 1 1");
            
            // If the material is not opaque the cutoutThreshold has to be set to 0
            if(entry.material.blendMode == go::gfx_blend_mode::opaque)
            {
                entry.material.bind_float_constant(section, "_Cutoff", "cutoutThreshold", "0.5");
            }
            else
            {
                entry.material.constants.emplace_back("cutoutThreshold", "0.0");
            }

            entry.material.bind_rgba_constant(section, "_EmissionColor", "emissiveTint", "1 1 1 1");
            entry.material.bind_float_constant(section, "_EmissiveIntensity", "emissiveIntensity", "1");
            entry.material.bind_float_constant(section, "_Metallic", "metallicScale", "1");
            
            // Remap glossiness to roughness
            auto glossiness = 0.0f;
            if(entry.material.textures[(int32_t)go::gfx_standard_material_textures::metallic_smoothness].length() == 0)
            {
                auto &&glossinessStr = section.opt_mapping("_Glossiness", "0.5");
                char *error = nullptr;
                glossiness = strtof(glossinessStr.c_str(), &error);
            }

            auto roughness = 1.0f - (glossiness * glossiness);
            entry.material.constants.emplace_back("roughnessScale", std::to_string(roughness));

            entry.material.bind_float_constant(section, "_OcclusionStrength", "occlusionScale", "1");
            entry.material.bind_float_constant(section, "_Transparency", "transparency", "0");
            entry.material.bind_float2_constant(section, "_UVTiling", "uvScaling", "1 1");
            entry.material.bind_float2_constant(section, "_UVOffsets", "uvBias", "0 0");

            break;
        }
        case go::gfx_material_type::unlit:
        {
            entry.material.bind_texture(go::gfx_unlit_material_textures::color, section.opt_mapping("_MainTex"), "BC3_UNORM_SRGB");
            break;
        }
        }

        uniqueMaterials.push_back(std::move(entry));
    }
}