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
    \file graphics/render_command.h
    \brief Contains classes for buffering and managing rendering commands.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"
#include "go/core/locks.h"
#include "tbb/concurrent_vector.h"
#include "tbb/concurrent_unordered_map.h"


GO_BEGIN_NAMESPACE

class gfx_model;
struct gfx_mesh;
class gfx_index_buffer;
class gfx_vertex_buffer;
enum class gfx_blend_mode : uint8_t;


//! A structure, which represents a model draw command.
struct gfx_static_model_render_command
{
    //! The model to render.
    gfx_model *model;
    //! Approximate view-space depth value.
    float viewZ;
    //! The world transformation.
    const DirectX::XMFLOAT4X4A *worldTransform;
};


//! A structure, which represents a static geometry draw command.
struct GO_API gfx_static_render_command
{
    typedef gfx_static_model_render_command enqueue_command_t;
    /*!
        Constructor.

        \param renderCmd The model render command.
     */
    gfx_static_render_command(const enqueue_command_t &renderCmd);
    //! The index buffer to use.
    gfx_index_buffer *indexBuffer;
    //! The vertex buffer to use.
    gfx_vertex_buffer *vertexBuffer;
    //! The vertex buffer to use for depth-only rendering.
    gfx_vertex_buffer *vertexBufferDepthOnly;
    //! The first index in the buffer.
    uint32_t startIndex;
    //! The vertex-index offset.
    uint32_t startVertex;
    //! The number of indices.
    uint32_t indexCount;
    //! The instance ID.
    uint32_t instanceID;
    //! The world transformation.
    const DirectX::XMFLOAT4X4A *worldTransform;
};


//! A structure, which represents a model draw command with animation data.
struct gfx_dynamic_model_render_command : public gfx_static_model_render_command
{
    //! A pointer to the gfx_bone matrices.
    const DirectX::XMFLOAT4X4 *skinnedBones;
    //! Number of skinned gfx_bones.
    uint32_t skinnedBonesCount;
};


//! A structure, which represents an animated geometry draw command.
struct GO_API gfx_dynamic_render_command : public gfx_static_render_command
{
    typedef gfx_dynamic_model_render_command enqueue_command_t;
    /*!
        Constructor.

        \param renderCmd The model render command.
     */
    gfx_dynamic_render_command(const enqueue_command_t &renderCmd);
    //! The skinned mesh skeleton.
    const DirectX::XMFLOAT4X4 *skinnedBones;
    //! Number of skinned gfx_bones pointed to.
    size_t skinnedBonesCount;
};


//! A structure, which represents a bitmask, that encodes a draw command.
struct GO_API gfx_render_command_key
{
    //! Constructor.
    gfx_render_command_key() = default;
    //! \overload
    gfx_render_command_key(uint32_t materialId, float depth, gfx_blend_mode blendMode) noexcept;
    /*!
        Extracts the material identifier.

        \return The material identifier.
     */
    rid material_id() const noexcept;
    /*!
        Extracts the depth value.

        \return The depth value.
     */
    float depth() const noexcept;
    /*!
        Extracts the blending flags.

        \return The blending flags.
     */
    gfx_blend_mode blend_mode() const noexcept;
    /*!
        Sets the value of the context key.

        \param materialId The material identifier.
        \param depth The world depth value.
        \param blendMode The translucency type.
     */
    void set(rid materialId, float depth, gfx_blend_mode blendMode) noexcept;
    //! The value of the key.
    uint64_t value;
};


//! A class, which buffers rendering commands.
template<class RenderCommand>
class gfx_render_command_buffer
{
public:
    typedef RenderCommand render_command_t;
    typedef tbb::concurrent_vector<std::pair<gfx_render_command_key, render_command_t>> command_vector_t;
    typedef tbb::concurrent_vector<size_t> command_index_vector_t;
public:
    //! Constructor.
    gfx_render_command_buffer() = default;
    /*!
        Constructor.

        \param bufferSizeHint The estimated size of the buffer.
     */
    gfx_render_command_buffer(size_t bufferSizeHint);
public:
    /*!
        Enqueues a new rendering command.

        This member function can be called in parallel.

        \param command The command the enqueue.
     */
    void enqueue(const typename render_command_t::enqueue_command_t &command);
    //! Clears the command buffer.
    void clear();
    //! Optimizes the command buffer to reduce rendering overhead.
    void sort();
    /*!
        Resizes the internal cache.

        \param bufferSizeHint The estimated size of the buffer.
     */
    void resize(size_t bufferSizeHint);
    /*!
        Returns a vector containing the render commands.

        \return The command vector.
     */
    const command_vector_t &opaque_commands() const noexcept;
    /*!
        The optimized command order.

        \return The optimized command order.
     */
    const command_index_vector_t &opaque_command_order() const noexcept;
    /*!
        The optimized command order.

        \return The optimized command order.
     */
    const command_vector_t &transparent_commands() const noexcept;
    /*!
        The optimized command order.

        \return The optimized command order.
     */
    const command_index_vector_t &transparent_command_order() const noexcept;
    /*!
         The instance-count buffer to use for instancing.

         \return The instance-count buffer.
     */
    const command_index_vector_t &instance_count_buffer() const noexcept;
private:
    typedef tbb::concurrent_unordered_map<uintptr_t, uint32_t> instance_cache_map_t;
private:
    /*!
        Enqueues a mesh for rendering.

        \param data The render command data.
        \param mesh The mesh to render.
        \param viewZ The approximate view-space Z value.
     */
    void enqueue_mesh(render_command_t &data, const gfx_mesh &mesh, float viewZ);
private:
    //! Render commands.
    command_vector_t m_opaqueCommands;
    //! Render commands.
    command_vector_t m_transparentCommands;
    //! Temporal coherence index buffer.
    command_index_vector_t m_opaqueCommandIndices;
    //! Temporal coherence index buffer.
    command_index_vector_t m_transparentCommandIndices;
    /*!
        A vector containing the (opaque) instance-chunk sizes.

        An entry in this vector describes how many indices a command index occupies, which corresponds
        to the number of instances.
     */
    command_index_vector_t m_instanceCountBuffer;
    //! Instance cache.
    instance_cache_map_t m_instanceCache;
    //! The next instance ID.
    std::atomic_uint32_t m_nextInstanceID;
};


typedef gfx_render_command_buffer<gfx_static_render_command> gfx_static_render_command_buffer;
typedef gfx_render_command_buffer<gfx_dynamic_render_command> gfx_dynamic_render_command_buffer;

//! A structure, which holds command buffers for rendering static and dynamic geometry.
struct GO_API gfx_render_command_buffers
{
    //! Constructor.
    gfx_render_command_buffers() = default;
    /*!
        Constructor.

        \param bufferSizeHint The estimated size of the buffer.
     */
    gfx_render_command_buffers(size_t bufferSizeHint);
    //! Clears all command buffers.
    void clear();
    //! Optimizes all command buffers.
    void sort();
    /*!
        Resizes the internal cache.

        \param bufferSizeHint The estimated size of the buffer.
     */
    virtual void resize(size_t bufferSizeHint);
    /*!
        Returns whether the command buffers are empty.

        \return True, if all command buffers are empty. Otherwise false.
     */
    bool is_empty() const noexcept;
    //! Render commands for static geometry.
    gfx_static_render_command_buffer staticGeometry;
    //! Render commands for dynamic geometry.
    gfx_dynamic_render_command_buffer dynamicGeometry;
};


#include "render_command_impl.h"

GO_END_NAMESPACE