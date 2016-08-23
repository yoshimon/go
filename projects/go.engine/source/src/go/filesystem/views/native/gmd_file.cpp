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


#include "go/filesystem/views/native/gmd_file.h"


#include "go/filesystem/file.h"


// ================================================================================ //
// go::gmd_file member functions
// ================================================================================ //

bool go::gmd_file::load(const uint8_t *data, size_t dataSize, gfx_model &out)
{
    auto header = reinterpret_cast<const header_t *>(data);
    auto meshData = reinterpret_cast<const submesh_t *>(header + 1);

    // Unskinned meshes
    out.unskinnedMeshes.clear();
    if(header->numUnskinnedMeshes > 0)
    {
        out.unskinnedMeshes.reserve(header->numUnskinnedMeshes);
        for(auto i = 0U; i < header->numUnskinnedMeshes; ++i, ++meshData)
        {
            gfx_mesh mesh;
            mesh.startIndex = meshData->startIndex;
            mesh.indexCount = meshData->numIndices;
            mesh.startVertex = meshData->startVertex;
            out.unskinnedMeshes.push_back(std::move(mesh));
            // Remark: the mesh material identifier will be patched in later
        }
    }

    // Skinned meshes
    out.skinnedMeshes.clear();
    if(header->numSkinnedMeshes > 0)
    {
        out.skinnedMeshes.reserve(header->numSkinnedMeshes);
        for(auto i = 0U; i < header->numSkinnedMeshes; ++i, ++meshData)
        {
            gfx_mesh mesh;
            mesh.startIndex = meshData->startIndex;
            mesh.indexCount = meshData->numIndices;
            mesh.startVertex = meshData->startVertex;
            out.skinnedMeshes.push_back(std::move(mesh));
            // Remark: the mesh material identifier will be patched in later
        }
    }

    // Index data
    auto indexSize = (header->numUnskinnedVertices + header->numSkinnedVertices) <= USHRT_MAX ? sizeof(index16_t) : sizeof(index32_t);
    auto indexData = reinterpret_cast<const char *>(meshData);

    // Vertex data
    auto unskinnedVertexData = reinterpret_cast<const go::gfx_model::unskinned_vertex_t *>(indexData + header->numIndices * indexSize);
    auto skinnedVertexData = reinterpret_cast<const go::gfx_model::skinned_vertex_t *>(unskinnedVertexData + header->numUnskinnedVertices);

    // Bind data to model
    out.reset(unskinnedVertexData, header->numUnskinnedVertices,
              skinnedVertexData, header->numSkinnedVertices,
              indexData, header->numIndices, indexSize);

    // gfx_bones
    /*
    if(header->numBones)
    {
        auto gfx_bonesData = reinterpret_cast<const go::world_bone *>(skinnedVertexData + header->numSkinnedVertices);
        out.bones.clear();
        out.bones.reserve(header->numBones);
        for(auto i = 0U; i < header->numBones; ++i, ++bonesData)
        {
            auto &bone = *bonesData;
            out.bones.push_back(bone);
            DirectX::XMStoreFloat4x3(&out.invBindPose[i], DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x3((DirectX::XMFLOAT4X3 *)&bone.localTransform)));
        }
    }
    */
    
    return true;
}


bool go::gmd_file::load(const file_path &fileName, gfx_model &out)
{
    auto dataVector = binary_file::read(fileName);
    return load(dataVector.data(), dataVector.size(), out);
}
