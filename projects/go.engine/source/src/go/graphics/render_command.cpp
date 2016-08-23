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


#include "go/graphics/render_command.h"


#include "go/graphics/model.h"


// ================================================================================ //
// go::gfx_static_render_command member functions
// ================================================================================ //

go::gfx_static_render_command::gfx_static_render_command(const enqueue_command_t &renderCmd)
    :
	indexBuffer(&renderCmd.model->index_buffer()),
	vertexBuffer(&renderCmd.model->unskinned_vertex_buffer()),
    vertexBufferDepthOnly(&renderCmd.model->unskinned_uv_vertex_buffer()),
    worldTransform(renderCmd.worldTransform)
{
}


// ================================================================================ //
// go::gfx_dynamic_render_command member functions
// ================================================================================ //

go::gfx_dynamic_render_command::gfx_dynamic_render_command(const enqueue_command_t &renderCmd)
    :
	gfx_static_render_command(renderCmd),
    skinnedBones(renderCmd.skinnedBones),
    skinnedBonesCount(renderCmd.skinnedBonesCount)
{
    // Overwrite vertex buffer
    vertexBuffer = &renderCmd.model->skinned_vertex_buffer();
    vertexBufferDepthOnly = vertexBuffer;
}


// ================================================================================ //
// go::gfx_render_command_buffers member functions
// ================================================================================ //

go::gfx_render_command_buffers::gfx_render_command_buffers(size_t bufferSizeHint)
    : staticGeometry(bufferSizeHint), dynamicGeometry(bufferSizeHint)
{
}


void go::gfx_render_command_buffers::clear()
{
    staticGeometry.clear();
    dynamicGeometry.clear();
}


void go::gfx_render_command_buffers::sort()
{
    staticGeometry.sort();
    dynamicGeometry.sort();
}


void go::gfx_render_command_buffers::resize(size_t bufferSizeHint)
{
    staticGeometry.resize(bufferSizeHint);
    dynamicGeometry.resize(bufferSizeHint);
}


bool go::gfx_render_command_buffers::is_empty() const noexcept
{
	return
		staticGeometry.opaque_commands().empty() &&
		staticGeometry.transparent_commands().empty() &&
		dynamicGeometry.opaque_commands().empty() &&
		dynamicGeometry.transparent_commands().empty();
}


// ================================================================================ //
// go::gfx_render_command_key member functions
// ================================================================================ //

go::gfx_render_command_key::gfx_render_command_key(uint32_t materialId, float depth, gfx_blend_mode blendMode) noexcept
{
    set(materialId, depth, blendMode);
}


go::rid go::gfx_render_command_key::material_id() const noexcept
{
    return (uint32_t)(value >> 32);
}


float go::gfx_render_command_key::depth() const noexcept
{
    // 27-bits depth
    return (float)(value & 0xFFFFFFF0);
}


auto go::gfx_render_command_key::blend_mode() const noexcept -> gfx_blend_mode
{
    // 3-bit blend-mode
    return (gfx_blend_mode)(value & 0xF);
}


void go::gfx_render_command_key::set(rid materialId, float depth, gfx_blend_mode blendMode) noexcept
{
    // 64-bit value = 32-bit matID + 28-bit depth + 4-bit blendMode
    value = ((uint64_t)materialId << 32) | ((uint32_t)depth & 0xFFFFFFF0) | (uint8_t)blendMode;
}