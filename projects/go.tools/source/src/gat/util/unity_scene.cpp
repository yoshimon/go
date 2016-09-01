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

#include "gat/util/unity_scene.h"


#include "gat/files/section_file.h"
#include "gat/util/exception.h"


 //===========================================================================
 // Helper functions
 //===========================================================================

template<typename F, typename T>
bool section_read_opt(const gat::section_file::section_t *section, F f, T &val,
    const std::string &key, const T &defaultVal = T())
{
    auto it = section->mappings.find(key);
    auto result = it != section->mappings.end();
    val = result ? f(it->second) : defaultVal;
    return result;
}


template<typename F, typename T>
void section_read(const gat::section_file::section_t *section, F f, T &val, const std::string &key)
{
    T temp;
    if(!section_read_opt<F, T>(section, f, val, key, temp))
    {
        throw gat::tchar_exception(_T("Missing required section key."));
    }
}


//===========================================================================
// gat::unity_scene
//===========================================================================

gat::unity_scene::unity_scene(const go::file_path &filePath)
{
    load(filePath);
}


void gat::unity_scene::load(const go::file_path &filePath)
{
    _tprintf(_T("Loading scene '%s'...\n"), filePath.c_str());

    // Load the section file
    section_file sceneFile(filePath);
    auto &sections = sceneFile.sections();

    // The data path is hard-coded at the moment
    m_dataDirPath = filePath.directory_path();

    // Section conversion functions
    auto mapInt = [](const std::string &s) { return std::stoi(s); };
    auto mapStr = [](const std::string &s) { return s; };
    auto mapFloat = [](const std::string &s) { return std::stof(s); };
    auto mapVec3 = [](const std::string &s) { return go::string_to_vector3f<DirectX::XMFLOAT3>(s.c_str()); };
    auto mapVec3Paren = [](const std::string &s)
    {
        auto substr = s.substr(1, s.length() - 1);
        std::replace(substr.begin(), substr.end(), ',', ' ');
        return go::string_to_vector3f<DirectX::XMFLOAT3>(substr.c_str());
    };
    auto mapVec3X = [](const std::string &s) { return go::string_to_vector3f<DirectX::XMFLOAT3>(s.c_str()).x; };
    auto mapVec4 = [](const std::string &s) { return go::string_to_vector4f<DirectX::XMFLOAT4>(s.c_str()); };
    auto mapRGBA = [](const std::string &s)
    {
        auto substr = s.substr(5, s.length() - 1);
        std::replace(substr.begin(), substr.end(), ',', ' ');
        return go::string_to_vector4f<DirectX::XMFLOAT4>(substr.c_str());
    };
    auto mapBool = [](const std::string &s) { return s == "True"; };
    auto mapCastsShadows = [](const std::string &s) { return s != "None"; };

    // Extract entity information out of the section file
    if(!sections.empty())
    {
        auto section = &sections.front();
        while(section)
        {
            unity_entity e;
            e.name = section->name;
            section_read(section, mapInt, e.id, "$id");
            section_read_opt(section, mapInt, e.parentID, "$parentID", -1);
            section_read(section, mapStr, e.tag, "$tag");
            section_read(section, mapBool, e.isStatic, "$isStatic");
            section_read(section, mapBool, e.activeSelf, "$activeSelf");
            section_read(section, mapBool, e.activeInHierarchy, "$activeInHierarchy");
            section_read(section, mapStr, e.hideFlags, "$hideFlags");
            section_read(section, mapVec3, e.position, "$position");
            section_read(section, mapVec4, e.rotation, "$rotation");
            section_read(section, mapVec3X, e.scale, "$scale");

            section_read_opt(section, mapBool, e.castsShadows, "$castsShadows", false);
            section_read_opt(section, mapBool, e.castsShadowsOnly, "$castsShadowsOnly", false);

            // Component: MeshFilter
            e.components.hasMeshFilter = section_read_opt(section, mapStr, e.components.meshFilter.modelMetaFileAssetPath, "$model");

            // Subsections
            for(auto ci : section->childIndices)
            {
                auto &childSection = sections[ci];

                if(childSection.name == "Reflection Probe")
                {
                    // Component: Reflection Probe
                    e.components.hasReflectionProbe = true;
                    auto &reflectionProbe = e.components.reflectionProbe;
                    section_read(&childSection, mapFloat, reflectionProbe.importance, "$importance");
                    section_read(&childSection, mapInt, reflectionProbe.resolution, "$resolution");
                    section_read(&childSection, mapBool, reflectionProbe.hasProxyBoxProj, "$proxyboxproj");
                    section_read(&childSection, mapVec3Paren, reflectionProbe.proxySize, "$proxysize");
                    section_read(&childSection, mapVec3Paren, reflectionProbe.proxyCenterOffset, "$proxycenteroffset");
                    section_read(&childSection, mapVec3Paren, reflectionProbe.proxyBoundsCenter, "$proxyboundscenter");
                    section_read(&childSection, mapVec3Paren, reflectionProbe.proxyBoundsSize, "$proxyboundssize");
                }
                else if(childSection.name == "Light")
                {
                    // Component: Light
                    e.components.hasLight = true;
                    auto &light = e.components.light;
                    section_read(&childSection, mapStr, light.type, "$type");
                    section_read(&childSection, mapFloat, light.radius, "$range");
                    section_read(&childSection, mapFloat, light.intensity, "$intensity");
                    section_read(&childSection, mapCastsShadows, light.castsShadows, "$castShadows");
                    section_read(&childSection, mapRGBA, light.color, "$color");
                    section_read(&childSection, mapFloat, light.spotAngle, "$spotAngle");
                }
            }

            m_entities.push_back(e);

            sceneFile.next_sibling(&section);
        }
    }
}


auto gat::unity_scene::entities() const noexcept -> const entity_vector_t &
{
    return m_entities;
}


const go::file_path &gat::unity_scene::data_directory_path() const noexcept
{
    return m_dataDirPath;
}