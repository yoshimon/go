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
    \file graphics/model.h
    \brief Contains a class which represents a 3D model.

    \author Gokhan Ozdogan
    \version 1.0
*/


#pragma once


#include "go/core/common.h"
#include "go/graphics/api/buffer.h"
#include "go/graphics/skeletal_animation.h"


GO_BEGIN_NAMESPACE

struct gfx_model_definition;
struct gfx_material_definition;


//! A structure, which represents a single mesh.
struct gfx_mesh
{
    //! The first index in the index buffer.
    uint32_t startIndex;
    //! The number of indices in the index buffer.
	uint32_t indexCount;
    //! The vertex offset.
	uint32_t startVertex;
    //! The material instance.
    gfx_material_definition *material = nullptr;
};


typedef std::vector<gfx_mesh> gfx_mesh_vector;


//! A class, which represents a 3D model that might consist of multiple meshes.
class GO_API gfx_model
{
    GO_DECLARE_NON_COPYABLE(gfx_model)
public:
    typedef std::vector<float3x4> skinned_bone_transform_vector_t; // Column-major matrix!
    //! A structure, which represents an unskinned mesh vertex.
    struct unskinned_vertex_t
    {
        /*!
            Equality-operator.

            \param o The vertex to compare with.

            \return True, if both vertices are equal. Otherwise false.
         */
        bool operator==(const unskinned_vertex_t &o) const { return memcmp(this, &o, sizeof(unskinned_vertex_t)) == 0; }
        //! The position.
        DirectX::XMFLOAT3 position;
        //! The tangent.
        DirectX::XMFLOAT4 tangent;
        //! The normal.
        DirectX::XMFLOAT3 normal;
        //! The UV coordinates.
        DirectX::XMFLOAT2 uv;
        //! The UV coordinates for lightmapping.
        DirectX::XMFLOAT2 lightmapUV;
    };
	//! A structure, which represents an unskinned mesh vertex.
	struct unskinned_uv_vertex_t
	{
		/*!
			Equality-operator.

			\param o The vertex to compare with.

			\return True, if both vertices are equal. Otherwise false.
		 */
		bool operator==(const unskinned_uv_vertex_t &o) const { return memcmp(this, &o, sizeof(unskinned_uv_vertex_t)) == 0; }
		//! The position.
		DirectX::XMFLOAT3 position;
		//! The UV coordinates.
		DirectX::XMFLOAT2 uv;
	};
    //! A structure, which represents a skinned vertex.
    struct skinned_vertex_t
    {
        //! The position.
        DirectX::XMFLOAT3 position;
        //! The gfx_bone indices (4).
        union
        {
            //! The gfx_bone indices.
            uint8_t gfx_boneIndices[4];
            //! The gfx_bone index mask.
            uint32_t gfx_boneIndexMask;
        };
        //! The gfx_bone weights (4).
        float gfx_boneWeights[4];
        //! The tangent.
        DirectX::XMFLOAT4 tangent;
        //! The normal.
        DirectX::XMFLOAT3 normal;
        //! The UV coordinates.
        DirectX::XMFLOAT2 uv;
        //! The UV coordinates for lightmapping.
        DirectX::XMFLOAT2 lightmapUV;
    };
public:
    /*!
        Constructor.

        \param unskinnedVertices The unskinned vertex data.
        \param unskinnedVertexCount The number of unskinned vertices.
        \param skinnedVertices The skinned vertex data.
        \param skinnedVertexCount The number of skinned vertices.
        \param indices The index data.
        \param indexCount The number of indices.
		\param indexSize The size of a single index in bytes.
	 */
    gfx_model(
        const unskinned_vertex_t *unskinnedVertices, uint32_t unskinnedVertexCount,
        const skinned_vertex_t *skinnedVertices, uint32_t skinnedVertexCount,
		const void *indices, uint32_t indexCount, size_t indexSize);
    //! \overload
    gfx_model() = default;
    //! Destructor.
    virtual ~gfx_model() = default;
public:
    //! A pointer to the model definition.
    const gfx_model_definition *definition;
    //! The unskinned meshes.
    gfx_mesh_vector unskinnedMeshes;
    //! The skinned meshes.
    gfx_mesh_vector skinnedMeshes;
public:
    /*!
        Returns the unskinned vertex buffer.

        \return The unskinned vertex buffer.
     */
    gfx_vertex_buffer &unskinned_vertex_buffer() noexcept{ return m_unskinnedVertexBuffer; }
	/*!
		Returns the unskinned UV vertex buffer.

		\return The unskinned UV vertex buffer.
	 */
	gfx_vertex_buffer &unskinned_uv_vertex_buffer() noexcept { return m_unskinnedUVVertexBuffer; }
    /*!
        Returns the skinned vertex buffer.

        \return The skinned vertex buffer.
     */
    gfx_vertex_buffer &skinned_vertex_buffer() noexcept{ return m_skinnedVertexBuffer; }
    /*!
        Returns the index buffer.

        \return The index buffer.
     */
    gfx_index_buffer &index_buffer() noexcept{ return m_indexBuffer; }
    /*!
        Re-initializes the object.

        \param unskinnedVertices The unskinned vertex data.
        \param unskinnedVertexCount The number of unskinned vertices.
        \param skinnedVertices The skinned vertex data.
        \param skinnedVertexCount The number of skinned vertices.
        \param indices The index data.
        \param indexCount The number of indices.
		\param indexSize The size of a single index in bytes.
     */
    virtual void reset(const unskinned_vertex_t *unskinnedVertices, uint32_t unskinnedVertexCount,
        const skinned_vertex_t *skinnedVertices, uint32_t skinnedVertexCount,
        const void *indices, uint32_t indexCount, size_t indexSize);
    //! Releases the vertex- and index buffer.
    void release();
private:
    //! The unskinned vertex buffer.
	gfx_vertex_buffer m_unskinnedVertexBuffer;
	//! The unskinned vertex buffer.
	gfx_vertex_buffer m_unskinnedUVVertexBuffer;
    //! The skinned vertex buffer.
    gfx_vertex_buffer m_skinnedVertexBuffer;
    //! The mesh index buffer.
    gfx_index_buffer m_indexBuffer;
};

GO_END_NAMESPACE
