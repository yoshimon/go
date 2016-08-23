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


#include "go/graphics/model.h"


// ================================================================================ //
// go::gfx_model member functions
// ================================================================================ //

go::gfx_model::gfx_model(const unskinned_vertex_t *unskinnedVertices, uint32_t unskinnedVertexCount,
    const skinned_vertex_t *skinnedVertices, uint32_t skinnedVertexCount,
    const void *indices, uint32_t indexCount, size_t indexSize)
{
    reset(unskinnedVertices, unskinnedVertexCount, skinnedVertices, skinnedVertexCount, indices, indexCount, indexSize);
}


void go::gfx_model::reset(const unskinned_vertex_t *unskinnedVertices, uint32_t unskinnedVertexCount,
    const skinned_vertex_t *skinnedVertices, uint32_t skinnedVertexCount,
    const void *indices, uint32_t indexCount, size_t indexSize)
{
    if(unskinnedVertexCount > 0)
    {
        m_unskinnedVertexBuffer.reset(unskinnedVertexCount, sizeof(unskinned_vertex_t), gfx_buffer_update_frequency::never, (void *)unskinnedVertices);
    
        // Clone the vertex buffer for Pos + UV only
        std::vector<unskinned_uv_vertex_t> unskinnedUVVertices;
        unskinnedUVVertices.resize(unskinnedVertexCount);
        for(size_t i = 0; i < unskinnedVertexCount; ++i)
        {
            auto &vIn = unskinnedVertices[i];
            auto &vOut = unskinnedUVVertices[i];
            vOut.position = vIn.position;
            vOut.uv = vIn.uv;
        }

        m_unskinnedUVVertexBuffer.reset(unskinnedUVVertices.size(), sizeof(unskinned_uv_vertex_t), gfx_buffer_update_frequency::never, (void *)unskinnedUVVertices.data());
    }
    else
    {
        m_unskinnedVertexBuffer.release();
        m_unskinnedUVVertexBuffer.release();
    }

    if(skinnedVertexCount > 0)
    {
        m_skinnedVertexBuffer.reset(skinnedVertexCount, sizeof(skinned_vertex_t), gfx_buffer_update_frequency::never, (void *)skinnedVertices);
    }
    else
    {
        m_skinnedVertexBuffer.release();
    }

    if(indexCount > 0)
    {
        m_indexBuffer.reset(indexCount, indexSize, gfx_buffer_update_frequency::never, (void *)indices);
    }
    else
    {
        m_indexBuffer.release();
    }
}


void go::gfx_model::release()
{
    m_unskinnedVertexBuffer.release();
    m_unskinnedUVVertexBuffer.release();
    m_skinnedVertexBuffer.release();
    m_indexBuffer.release();
}
