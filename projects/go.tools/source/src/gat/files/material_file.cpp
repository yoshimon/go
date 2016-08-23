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

#include "gat/files/material_file.h"


#include "gat/util/file_util.h"
#include "gat/util/exception.h"
#include "gat/util/rapidxml_helper.h"


//===========================================================================
// gat::material_file
//===========================================================================

gat::material_file::material_file(const go::file_path &filePath)
{
    load(filePath);
}


void gat::material_file::load(const go::file_path &filePath)
{
    rapidxml::xml_document<> doc;
    try
    {
        auto &&txt = file_util::read_text_file(filePath);
        doc.parse<0>((char *)txt.c_str());

        auto &&nMaterial = doc.first_node("material");
        if(!nMaterial)
        {
            throw tchar_exception(_T("Invalid material file."));
        }

        auto &&shader = nMaterial->first_attribute("shader")->value();
        if(!go::gfx_material_shader_name_to_id(shader, shaderID))
        {
            throw tchar_exception(_T("Invalid material shader."));
        }
        
        name = nMaterial->first_attribute("name")->value();
        go::xml_get_bool_attribute(nMaterial, "isTwoSided", isTwoSided, false);
        go::xml_get_enum_attribute(nMaterial, "blendmode", go::gfx_blend_mode_strings, blendMode);

        // Grab material and shader info
        auto &&shaderInfo = go::gfx_material_shader_info[(uint32_t)shaderID];

        // Read the material textures
		auto &&nTextures = nMaterial->first_node("textures");
		auto &&nTex = nTextures->first_node("texture");
		GO_FOR_EACH_INDEX(current, textures)
        {
            if(!nTex)
            {
                throw tchar_exception(_T("Expected texture node in material file."));
            }

            *current.iterator = nTex->value();
			textureFormats[current.index] = nTex->first_attribute("format")->value();

            nTex = nTex->next_sibling(nTex->name());
        }

		// Read the material constants
		auto &&nConstants = nMaterial->first_node("constants");
		auto &&nConstant = nConstants->first_node();
		while(nConstant)
		{
			auto &&semantic = nConstant->name();
			auto &&value = nConstant->value();

			constants.emplace_back(semantic, value);

			nConstant = nConstant->next_sibling();
		}
    }
    catch(...)
    {
        throw tchar_exception(_T("Could not load material file."));
    }
}


void gat::material_file::save(const go::file_path &filePath)
{
    rapidxml::xml_document<> doc;
    try
    {
        auto &&nMaterial = doc.allocate_node(rapidxml::node_element, "material");
        doc.append_node(nMaterial);

        // Lookup material info
        auto &&shaderInfo = go::gfx_material_shader_info[(uint32_t)shaderID];

        // Material attributes
        go::xml_require_attribute(nMaterial, "name", name.c_str());
        go::xml_require_attribute(nMaterial, "shader", shaderInfo.name);
        go::xml_require_attribute(nMaterial, "isTwoSided", "true");
        go::xml_require_attribute(nMaterial, "blendmode", go::gfx_blend_mode_strings[(uint32_t)blendMode]);

        // Material textures
		auto &&nTextures = go::xml_add_child(nMaterial, "textures");
        GO_FOR_EACH_INDEX(current, textures)
        {
            auto &&nTex = go::xml_add_child(nTextures, "texture", textures[current.index].c_str());
            go::xml_require_attribute(nTex, "format", textureFormats[current.index].c_str());
        }

		// Material constants
		auto &&nConstants = go::xml_add_child(nMaterial, "constants");
		for(auto &&e : constants)
		{
			go::xml_add_child(nConstants, e.first.c_str(), e.second.c_str());
		}

        go::xml_save_document(doc, filePath);
    }
    catch(...)
    {
        throw tchar_exception(_T("Could not save material file."));
    }
}


void gat::material_file::bind_rgba_constant(
	const section_file::section_t &section,
	const std::string &semantic,
	const std::string &newSemantic,
	const std::string &defaultValue)
{
	auto &&value = section.opt_mapping(semantic, defaultValue);

	// Remove the surrounding RGBA()
	auto &&values = value.substr(5, value.length() - 6);

	constants.emplace_back(newSemantic, values);
}


void gat::material_file::bind_float_constant(
	const section_file::section_t &section,
	const std::string &semantic,
	const std::string &newSemantic,
	const std::string &defaultValue)
{
	constants.emplace_back(newSemantic, section.opt_mapping(semantic, defaultValue));
}


void gat::material_file::bind_float2_constant(
	const section_file::section_t &section,
	const std::string &semantic,
	const std::string &newSemantic,
	const std::string &defaultValue)
{
	constants.emplace_back(newSemantic, section.opt_mapping(semantic, defaultValue));
}