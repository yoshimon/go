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

#include "gat/util/unity_scene_converter.h"


#include "gat/util/converters.h"
#include "gat/files/section_file.h"
#include "gat/util/exception.h"


//===========================================================================
// gat::unity_scene_converter
//===========================================================================

gat::unity_scene_converter::unity_scene_converter(const unity_scene &scene)
    : m_scene(scene)
{
}


void gat::unity_scene_converter::save(const config_file &cfg,
    const go::file_path &sceneFilePath,
    const go::file_path &materialSetFilePath,
    const go::file_path &outAssetDirPath,
    asset_meta_generators &metaGenerators)
{
    auto assetDirPath = m_scene.data_directory_path() + _T("data\\");
    
    auto &entities = m_scene.entities();
    auto numEntities = entities.size();

    // Model cache
    std::set<std::string> convertedModels;

    // Convert each entity in the scene
    auto i = 0;
    for(auto &entity : entities)
    {
        ++i;
        _tprintf("Processing entity (%d/%d) '%s' [id=%d]...\n", i, numEntities, entity.name.c_str(), entity.id);

        // Components: MeshFilter
        if(entity.components.hasMeshFilter)
        {
            auto &&meshFilter = entity.components.meshFilter;
            auto &&modelFilePath = assetDirPath + meshFilter.modelMetaFileAssetPath;
            auto pib = convertedModels.insert(modelFilePath);
            if(pib.second)
            {
                auto ext = meshFilter.modelMetaFileAssetPath.file_extension();
                if(!tstrcmp(ext, _T(".egs")))
                {
                    gat::unity_model_converter_parameters mdlParams;
                    mdlParams.in.modelFilePath = modelFilePath;
                    mdlParams.out.modelFilePath = outAssetDirPath + meshFilter.modelMetaFileAssetPath;
                    mdlParams.out.modelFilePath.replace_extension(_T(".gmd"));
                    converters::convert_unity_model(cfg, mdlParams, metaGenerators);
                }
                else
                {
                    // TODO: handle non-*.egs files, like *.fbx (which seem to be incompatible with assimp)
                }
            }
        }
    }

    // Convert the materials / textures
    unity_material_converter_parameters mtlConvParams;
    mtlConvParams.in.materialSetFilePath = materialSetFilePath;
    mtlConvParams.in.assetDirPath = assetDirPath;
    mtlConvParams.out.assetDirPath = outAssetDirPath;
    converters::convert_unity_material_set(cfg, mtlConvParams, metaGenerators);

    // Write out the scene file
    rapidxml::xml_document<> scnDoc;
    try
    {
        auto nRoot = scnDoc.allocate_node(rapidxml::node_element, "scene");
        scnDoc.append_node(nRoot);

        for(auto &entity : entities)
        {
            // Basic entity properties
            auto entityXMLNode = go::xml_add_child(nRoot, "entity");
            go::xml_require_attribute(entityXMLNode, "name", entity.name.c_str());
            go::xml_add_child(entityXMLNode, "position", go::vector3_to_string<DirectX::XMFLOAT3>(entity.position).c_str());
            go::xml_add_child(entityXMLNode, "rotation", go::vector4_to_string<DirectX::XMFLOAT4>(entity.rotation).c_str());
            go::xml_add_child(entityXMLNode, "scale", std::to_string(entity.scale).c_str());

            // The attached components
            auto componentsXMLNode = go::xml_add_child(entityXMLNode, "components");

            if(entity.components.hasMeshFilter)
            {
                auto modelXMLNode = go::xml_add_child(componentsXMLNode, "model");
                
                // Write out the proper asset name
                auto assetName = entity.components.meshFilter.modelMetaFileAssetPath;
                assetName.replace_extension(".gmd");
                go::xml_require_attribute(modelXMLNode, "assetName", assetName.c_str());

                go::xml_add_child(modelXMLNode, "castsShadows", std::to_string(entity.castsShadows).c_str());
                go::xml_add_child(modelXMLNode, "castsShadowsOnly", std::to_string(entity.castsShadowsOnly).c_str());
			}

            if(entity.components.hasLight)
            {
                auto lightNode = go::xml_add_child(componentsXMLNode, "light");

                // Write out the proper asset name
                auto &light = entity.components.light;
                go::xml_require_attribute(lightNode, "type", light.type.c_str());
                go::xml_require_attribute(lightNode, "intensity", std::to_string(light.intensity).c_str());
                go::xml_require_attribute(lightNode, "radius", std::to_string(light.radius).c_str());
                go::xml_require_attribute(lightNode, "castsShadows", std::to_string(light.castsShadows).c_str());
                go::xml_require_attribute(lightNode, "color", go::vector4_to_string(light.color).c_str());
            }

            if(entity.components.hasReflectionProbe)
            {
                auto rProbeNode = go::xml_add_child(componentsXMLNode, "rprobe");
                auto &rProbe = entity.components.reflectionProbe;
                go::xml_require_attribute(rProbeNode, "importance", std::to_string(rProbe.importance).c_str());
                go::xml_require_attribute(rProbeNode, "hasProxyBoxProj", std::to_string(rProbe.hasProxyBoxProj).c_str());
                go::xml_require_attribute(rProbeNode, "proxyBoundsCenter", go::vector3_to_string(rProbe.proxyBoundsCenter).c_str());
                go::xml_require_attribute(rProbeNode, "proxyBoundsSize", go::vector3_to_string(rProbe.proxyBoundsSize).c_str());
                go::xml_require_attribute(rProbeNode, "proxyCenterOffset", go::vector3_to_string(rProbe.proxyCenterOffset).c_str());
                go::xml_require_attribute(rProbeNode, "proxySize", go::vector3_to_string(rProbe.proxySize).c_str());
                go::xml_require_attribute(rProbeNode, "resolution", std::to_string(rProbe.resolution).c_str());
            }
        }

        go::xml_save_document(scnDoc, sceneFilePath);
    }
    catch(...)
    {
        throw tchar_exception(_T("Could not save the scene file."));
    }
}