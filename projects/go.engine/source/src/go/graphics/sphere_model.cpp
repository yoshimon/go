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


#include "go/graphics/sphere_model.h"


using namespace DirectX;


// ================================================================================ //
// Helper functions
// ================================================================================ //

typedef std::vector<go::gfx_model::unskinned_vertex_t> vertex_cache_t;

uint32_t append_vertex(
    vertex_cache_t &vertices,
    FXMVECTOR vPosition)
{
    auto &&index = vertices.size();

    auto &&vNormal = XMVector3Normalize(vPosition);

    auto &&dx = XMVectorGetX(vNormal);
    auto &&dy = XMVectorGetY(vNormal);
    auto &&dz = XMVectorGetZ(vNormal);

    auto &&vUV = XMVectorSet(
        0.5f + std::atan2f(dz, dx) / XM_2PI,
        0.5f - std::asin(dy) / XM_PI,
        0.0f, 0.0f
    );

    auto &&vTangent = (XMVectorGetX(XMVector3Dot(g_XMIdentityR1, vNormal)) == 1.0f) ?
        g_XMIdentityR0 :
        XMVector3Normalize(XMVector3Cross(g_XMIdentityR1, vNormal));
    vTangent = XMVectorSetW(vTangent, 1.0f);

    go::gfx_model::unskinned_vertex_t v;
    DirectX::XMStoreFloat3(&v.position, vNormal);
    DirectX::XMStoreFloat3(&v.normal, vNormal);
    DirectX::XMStoreFloat4(&v.tangent, vTangent);
    DirectX::XMStoreFloat2(&v.uv, vUV);
    DirectX::XMStoreFloat2(&v.lightmapUV, vUV);
    vertices.push_back(v);

    return index;
}


typedef std::map<std::tuple<uint32_t, uint32_t>, uint32_t> halfway_vertex_index_cache_t;


uint32_t halfway_index(
    vertex_cache_t &vertices,
    halfway_vertex_index_cache_t &halfwayVertexIndexCache, 
    uint32_t i0, uint32_t i1)
{
    // Swap to (smaller, larger) index
    if(i0 > i1)
    {
        std::swap(i0, i1);
    }

    // Lookup the index pair to find the index of the vertex halfway between the two
    auto &&it = halfwayVertexIndexCache.find(std::make_tuple(i0, i1));
    if(it != halfwayVertexIndexCache.end())
    {
        return it->second;
    }

    // Calculate the new point
    auto &&p1 = XMLoadFloat3(&vertices[i0].position);
    auto &&p2 = XMLoadFloat3(&vertices[i1].position);
    auto &&p3 = (p1 + p2) * 0.5f;
    auto &&i3 = append_vertex(vertices, p3);

    // Register the new halfway point
    halfwayVertexIndexCache.emplace(std::make_tuple(i0, i1), i3);

    return i3;
}


// ================================================================================ //
// go::gfx_icosphere_model member functions
// ================================================================================ //

go::gfx_icosphere_model::gfx_icosphere_model(uint32_t refinementIterations)
{
    reset(refinementIterations);
}


void go::gfx_icosphere_model::reset(uint32_t refinementIterations)
{
    // Clamp to a reasonable refinement number
    refinementIterations = clamp(refinementIterations, 1U, 4U);
    auto sqIterations = refinementIterations * refinementIterations;
    auto requiredVertices = 10 * sqIterations + 2;
    auto requiredFaces = 20 * sqIterations;
    auto requiredIndices = requiredFaces / 3;

    // Reserve space on stack
    vertex_cache_t vertices;
    vertices.reserve(requiredVertices);

    std::vector<uint32_t> indices;
    indices.reserve(requiredIndices);

    // Base vertices (refinement level 1)
    auto t = (1.0f + std::sqrtf(5.0f)) * 0.5f;

    append_vertex(vertices, XMVectorSet(-1.0f, +t, 0.0f, 1.0f));
    append_vertex(vertices, XMVectorSet(+1.0f, +t, 0.0f, 1.0f));
    append_vertex(vertices, XMVectorSet(-1.0f, -t, 0.0f, 1.0f));
    append_vertex(vertices, XMVectorSet(+1.0f, -t, 0.0f, 1.0f));

    append_vertex(vertices, XMVectorSet(+0.0f, -1.0f, +t, 1.0f));
    append_vertex(vertices, XMVectorSet(+0.0f, +1.0f, +t, 1.0f));
    append_vertex(vertices, XMVectorSet(+0.0f, -1.0f, -t, 1.0f));
    append_vertex(vertices, XMVectorSet(+0.0f, +1.0f, -t, 1.0f));

    append_vertex(vertices, XMVectorSet(+t, 0.0f, -1.0f, 1.0f));
    append_vertex(vertices, XMVectorSet(+t, 0.0f, +1.0f, 1.0f));
    append_vertex(vertices, XMVectorSet(-t, 0.0f, -1.0f, 1.0f));
    append_vertex(vertices, XMVectorSet(-t, 0.0f, +1.0f, 1.0f));

    // Create the faces
    typedef std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> triangle_cache_t;
    triangle_cache_t faces;
    faces.reserve(requiredFaces);

    // The initial triangulation
    // 5 faces around point 0
    faces.emplace_back(0, 11, 5);
    faces.emplace_back(0, 5, 1);
    faces.emplace_back(0, 1, 7);
    faces.emplace_back(0, 7, 10);
    faces.emplace_back(0, 10, 11);

    // 5 adjacent faces 
    faces.emplace_back(1, 5, 9);
    faces.emplace_back(5, 11, 4);
    faces.emplace_back(11, 10, 2);
    faces.emplace_back(10, 7, 6);
    faces.emplace_back(7, 1, 8);

    // 5 faces around point 3
    faces.emplace_back(3, 9, 4);
    faces.emplace_back(3, 4, 2);
    faces.emplace_back(3, 2, 6);
    faces.emplace_back(3, 6, 8);
    faces.emplace_back(3, 8, 9);

    // 5 adjacent faces 
    faces.emplace_back(4, 9, 5);
    faces.emplace_back(2, 4, 11);
    faces.emplace_back(6, 2, 10);
    faces.emplace_back(8, 6, 7);
    faces.emplace_back(9, 8, 1);

    // Maps an index pair (i0, i1) to an index i2 of the vertex between the two
    halfway_vertex_index_cache_t halfWayVertexIndexCache;

    // Refinement pass
    triangle_cache_t refinedFaces;
    refinedFaces.reserve(faces.capacity());

    for(uint32_t i = 1; i < refinementIterations; ++i)
    {
        refinedFaces.clear();

        // Compute the new faces
        for(auto &&f : faces)
        {
            // replace triangle by 4 triangles
            auto &&a = halfway_index(vertices, halfWayVertexIndexCache, std::get<0>(f), std::get<1>(f));
            auto &&b = halfway_index(vertices, halfWayVertexIndexCache, std::get<1>(f), std::get<2>(f));
            auto &&c = halfway_index(vertices, halfWayVertexIndexCache, std::get<2>(f), std::get<0>(f));

            refinedFaces.emplace_back(std::get<0>(f), a, c);
            refinedFaces.emplace_back(std::get<1>(f), b, a);
            refinedFaces.emplace_back(std::get<2>(f), c, b);
            refinedFaces.emplace_back(a, b, c);
        }

        std::swap(faces, refinedFaces);
    }

    // Correct poles
    /*
    triangle_cache_t poleCorrected;
    for(auto &&face : faces)
    {
        auto &&faceIndices = { std::get<0>(face), std::get<1>(face), std::get<2>(face) };
        for(auto &&vi : faceIndices)
        {
            // See if this is a pole vertex
            if(std::abs(vertices[vi].position.y) == 1.0f)
            {
                // Create a duplicate
                auto &&newVI = append_vertex(vertices, XMLoadFloat3(&vertices[vi].position));
                auto &&newV = vertices[newVI];
                float offset = 0.5f;

                auto &&vj = (vi + 1) % 3;
                auto &&vk = (vi + 2) % 3;

                // North pole
                if(vertices[vi].position.y == 1.0f)
                {
                    // See if the texture coords wrap around
                    if(vertices[vj].uv.x > vertices[vk].uv.x)
                    {
                        offset = 0;
                    }
                }

                newVertex.uv.x = (vertices[i2].uv.x + vertices[i3].uv.x) * 0.5f + offset;
                newVertex.uv.y = (float)(0.5 - std::asinf(newVertex.position.y) / XM_PI);
                vertices[newVertexIndex] = newVertex;

                poleCorrected.emplace_back(newVI, vj, vk);
            }
        }
    }
    faces = poleCorrected;
    */

    // Build triangle list
    for(auto &&f : faces)
    {
        indices.push_back(std::get<2>(f));
        indices.push_back(std::get<1>(f));
        indices.push_back(std::get<0>(f));
    }

    // Correct seams
    /*
    std::vector<int32_t> newIndices;
    newIndices.reserve(indices.size());

    std::map<int32_t, int32_t> oldNewVertexIndexCache;
    for(int i = indices.Count - 3; i >= 0; i -= 3)
    {
        auto &&v0 = (vertices[indices[i + 0]].TexCoord, 0);
        auto &&v1 = (vertices[indices[i + 1]].TexCoord, 0);
        auto &&v2 = (vertices[indices[i + 2]].TexCoord, 0);

        Vector3 cross = Vector3.Cross(v0 - v1, v2 - v1);

        if(cross.z > 0)
        {
            // This should only happen if the face crosses a texture boundary.
            for(var j = i; j < i + 3; j++)
            {
                int index = indices[j];

                VertexPosTex vertex = vertices[index];
                // 0.9 UV fudge factor - should be able to get rid of this when I get more sleep.
                if(vertex.TexCoord.X >= 0.8f)
                {
                    // Need to correct this vertex.
                    if(correctionList.ContainsKey(index))
                        newIndices.Add(correctionList[index]);
                    else
                    {
                        Vector2 texCoord = vertex.TexCoord;

                        texCoord.X -= 1;
                        vertex.TexCoord = texCoord;

                        vertices.Add(vertex);

                        int correctedVertexIndex = vertices.Count - 1;

                        correctionList.Add(index, correctedVertexIndex);

                        newIndices.Add(correctedVertexIndex);
                    }
                }
                else
                    newIndices.Add(index);
            }
        }
        else
        {
            newIndices.AddRange(indices.GetRange(i, 3));
        }
    }

    indices.clear();
    indices.insert(newIndices.begin(), newIndices.end());
    */

    gfx_model::reset(vertices.data(), vertices.size(), nullptr, 0, indices.data(), indices.size(), sizeof(uint32_t));
}


void go::gfx_icosphere_model::lod(uint32_t level, uint32_t &indexOffset, uint32_t &indexCount)
{
    GO_ASSERT(false);
}