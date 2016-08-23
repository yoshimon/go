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

#include "gat/util/unity_model.h"


#include "gat/files/section_file.h"


//===========================================================================
// gat::unity_model
//===========================================================================

gat::unity_model::unity_model(const go::file_path &filePath)
{
    load(filePath);
}


void gat::unity_model::load(const go::file_path &filePath)
{
    section_file egsFile(filePath);
    auto &egsSections = egsFile.sections();

    m_materialNames = egsFile.find_section("$materials")->listContent;

    // Build vertex buffer
    auto &posSection = egsFile.find_section("$vertexPos")->listContent;
    auto &normalsSection = egsFile.find_section("$normals")->listContent;
    auto &tangentsSection = egsFile.find_section("$tangents")->listContent;
    auto &uvSet1Section = egsFile.find_section("$uvSet1")->listContent;
    auto &uvSet2Section = egsFile.find_section("$uvSet2")->listContent;

    for(size_t i = 0, end = posSection.size(); i < end; ++i)
    {
        auto vPos = go::string_to_vector3f<DirectX::XMFLOAT3>(posSection[i].c_str());
        auto vNormal = go::string_to_vector3f<DirectX::XMFLOAT3>(normalsSection[i].c_str());
        auto vTangent = go::string_to_vector4f<DirectX::XMFLOAT4>(tangentsSection[i].c_str());

        // Handle missing UVs
        auto vUV1 = uvSet1Section.empty() ? DirectX::XMFLOAT2(0, 0) : go::string_to_vector2f<DirectX::XMFLOAT2>(uvSet1Section[i].c_str());
        auto vUV2 = uvSet2Section.empty() ? DirectX::XMFLOAT2(0, 0) : go::string_to_vector2f<DirectX::XMFLOAT2>(uvSet2Section[i].c_str());

        unskinned_vertices_t::value_type v;
        v.position = vPos;
        v.normal = vNormal;
        v.tangent = vTangent;
        v.uv = vUV1;
        v.lightmapUV = vUV2;
        m_unskinnedVertices.push_back(v);
    }

    // Build index buffer
    auto &sections = egsFile.sections();
    auto &indicesSection = egsFile.find_section("$indices")->childIndices;

    auto &meshContainer = m_skinnedVertices.empty() ? m_unskinnedMeshes : m_skinnedMeshes;

    for(auto submeshSectionIndex : indicesSection)
    {
        auto &submeshSection = sections[submeshSectionIndex];

        // Create a new submesh
        meshes_t::value_type mesh;
        mesh.indexCount = submeshSection.listContent.size();
        mesh.startIndex = m_indices.size();
        mesh.startVertex = 0;

        for(auto submeshIndex : submeshSection.listContent)
        {
            auto index = std::stoi(submeshIndex);
            m_indices.push_back(index);
        }

        meshContainer.push_back(mesh);
    }
}


auto gat::unity_model::unskinned_vertices() const noexcept -> const unskinned_vertices_t &
{
    return m_unskinnedVertices;
}


auto gat::unity_model::skinned_vertices() const noexcept -> const skinned_vertices_t &
{
    return m_skinnedVertices;
}


auto gat::unity_model::indices() const noexcept -> const indices_t &
{
    return m_indices;
}


auto gat::unity_model::unskinned_meshes() const noexcept -> const meshes_t &
{
    return m_unskinnedMeshes;
}


auto gat::unity_model::skinned_meshes() const noexcept -> const meshes_t &
{
    return m_skinnedMeshes;
}

auto gat::unity_model::material_names() const noexcept -> const material_names_t &
{
    return m_materialNames;
}
