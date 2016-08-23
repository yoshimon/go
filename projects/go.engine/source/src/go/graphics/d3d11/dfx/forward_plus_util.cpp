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


#include "go/graphics/d3d11/dfx/forward_plus_util.h"


#include "go/graphics/asset_management/material_database.h"
#include "go/graphics/asset_management/texture_manager.h"


#include "go/graphics/render_command.h"
#include "go/graphics/d3d11/blend_state.h"
#include "go/graphics/d3d11/device.h"
#include "go/graphics/d3d11/renderer.h"
#include "tbb/tbb.h"

#include "dfx/namespaces/constants/namespace_types.h"
#include "dfx/namespaces/samplers/namespace_types.h"
#include "dfx/namespaces/forward_plus/namespaces/constants/namespace_types.h"

#include "dfx/namespaces/instance_data/namespace_types.h"
#include "dfx/namespaces/resources/namespaces/instancing/namespace_types.h"


// Materials
#include "go/graphics/d3d11/dfx/namespaces/materials/bindings.h"
#include "dfx/namespaces/materials/namespaces/textures/namespaces/standard/namespace_types.h"
#include "dfx/namespaces/materials/namespaces/textures/namespaces/unlit/namespace_types.h"


using namespace DirectX;


// The global material texture bindings
// This must always match the material texture definition!
go::dfx_materials::texture_bindings go::dfx_util::forward_plus_renderer::kStandardMaterialTextureBindings
{
    {
        dfx_hlsl::materials::textures::standard::kRegister_albedoAlphaTexture,
        dfx_hlsl::materials::textures::standard::kRegister_metallicSmoothnessTexture,
        dfx_hlsl::materials::textures::standard::kRegister_normalTexture,
        dfx_hlsl::materials::textures::standard::kRegister_occlusionTexture,
        dfx_hlsl::materials::textures::standard::kRegister_emissionTexture,
    }, (uint32_t)go::gfx_standard_material_textures::count
};

go::dfx_materials::texture_bindings go::dfx_util::forward_plus_renderer::kUnlitMaterialTextureBindings
{
    {
        dfx_hlsl::materials::textures::unlit::kRegister_colorTexture,
    }, (uint32_t)go::gfx_unlit_material_textures::count
};


// ================================================================================ //
// Common helper functions
// ================================================================================ //

void bind_instance_constants(uint32_t instanceIDBias, uint32_t materialIndex)
{
    // Update CB
    auto p = static_cast<dfx_hlsl::constants::instanced_draw *>(dfx_hlsl::constants::g_instanced_draw->begin_update());
    p->g_instancingIDBias = instanceIDBias;
    p->g_instancingMaterialIndex = materialIndex;
    dfx_hlsl::constants::g_instanced_draw->end_update();
}


void bind_depth_stencil_state(const go::dfx_util::forward_plus_render_pass &renderPass,
    go::gfx_blend_mode blendMode)
{
    // Some blend modes should not write depth
    if(!renderPass.writeDepth || blendMode == go::gfx_blend_mode::additive)
    {
        go::the_gfx_renderer->bind_depth_stencil_state(renderPass.depthTestEnabledDSS, 0);
    }
    else
    {
        go::the_gfx_renderer->bind_depth_stencil_state(renderPass.depthTestWriteEnabledDSS, 0);
    }
}


void bind_blend_and_depth_stencil_state(
    const go::dfx_util::forward_plus_render_pass &renderPass,
    go::gfx_blend_mode blendMode)
{
    // The system blend states
    auto &blendStates = go::the_gfx_device->common_blend_states();

    switch(blendMode)
    {
    case go::gfx_blend_mode::opaque: go::the_gfx_renderer->bind_blend_state(nullptr); break;
    case go::gfx_blend_mode::alpha: go::the_gfx_renderer->bind_blend_state(&blendStates.alpha); break;
    case go::gfx_blend_mode::additive: go::the_gfx_renderer->bind_blend_state(&blendStates.additive); break;
    case go::gfx_blend_mode::multiply: go::the_gfx_renderer->bind_blend_state(&blendStates.multiply); break;
    default: GO_MSGASSERT(false, "Unexpected blend mode.");
    }

    bind_depth_stencil_state(renderPass, blendMode);
}


template<typename Command>
void bind_geometry(Command &cmd, bool depthOnly)
{
    // Update IA
    go::the_gfx_renderer->bind_index_buffer(cmd.indexBuffer, 0);
    go::the_gfx_renderer->bind_vertex_buffer(0, depthOnly ? cmd.vertexBufferDepthOnly : cmd.vertexBuffer);
}


void update_static_instance_data(const go::gfx_static_render_command_buffer &cmdBuffer)
{
    auto &opaqueCommands = cmdBuffer.opaque_commands();
    auto &transparentCommands = cmdBuffer.transparent_commands();
    auto instanceBuffer = dfx_hlsl::resources::instancing::g_unskinnedInstanceData;

    auto &opaqueCmdOrder = cmdBuffer.opaque_command_order();
    auto maxNumInstances = (size_t)instanceBuffer->element_capacity();
    auto numCommands = opaqueCommands.size() + transparentCommands.size();

    if(numCommands >= maxNumInstances)
    {
        // We can't fill more data in!
        GO_LOGFILE_WARNING("Instance buffer not large enough to hold all instances.");
    }

    // Map the entire buffer
    auto p = reinterpret_cast<dfx_hlsl::instance_data::unskinned_instance_data *>(instanceBuffer->begin_update());
    
    // Opaque geometry
    size_t j = 0;
    {
        for(size_t end = std::min(maxNumInstances, opaqueCommands.size()); j < end; ++j)
        {
            auto i = opaqueCmdOrder[j];
            auto &data = std::get<1>(opaqueCommands[i]);
            auto world = XMMatrixTranspose(XMLoadFloat4x4A(data.worldTransform));
            go::XMStoreFloat3x4((XMFLOAT4X3 *)&p[j], world);
        }
    }

    // Transparent geometry
    {
        for(size_t i = 0, end = std::min(maxNumInstances, opaqueCommands.size() + transparentCommands.size()); j < end; ++j, ++i)
        {
            auto &data = std::get<1>(transparentCommands[i]);
            auto world = XMMatrixTranspose(XMLoadFloat4x4A(data.worldTransform));
            go::XMStoreFloat3x4((XMFLOAT4X3 *)&p[j], world);
        }
    }

    instanceBuffer->end_update();
}


// ================================================================================ //
// Forward+ helper functions
// ================================================================================ //

void forward_plus_bind_material(
    go::gfx_texture_manager *texMgr,
    const go::dfx_util::forward_plus_shader_cache &shaderCache,
    go::gfx_material_definition *material,
    go::gfx_material_definition *&lastMaterial,
    const go::dfx_util::forward_plus_render_pass &renderPass)
{
    if(material != lastMaterial)
    {
        lastMaterial = material;

        // Bind the right textures
        go::dfx_materials::texture_bindings *textureBindings = nullptr;
        switch(material->shaderID)
        {
        case go::gfx_material_shader_id::standard:
        case go::gfx_material_shader_id::standard_cutout: textureBindings = &go::dfx_util::forward_plus_renderer::kStandardMaterialTextureBindings; break;
        case go::gfx_material_shader_id::unlit: textureBindings = &go::dfx_util::forward_plus_renderer::kUnlitMaterialTextureBindings; break;
        }

        // Pick the pixel shader table
        auto matShaderTable = renderPass.depthOnly ? shaderCache.common.materialDepthOnlyPSTable : shaderCache.materialShaders;

        if(material->isTwoSided)
        {
            go::the_gfx_renderer->bind_rasterizer_state(renderPass.twoSidedRS);
        }
        else
        {
            go::the_gfx_renderer->bind_rasterizer_state(renderPass.frontFacesRS);
        }

        // Bind the right shader and rasterizer state
        if(material->requiresAlphaTest)
        {
            go::the_gfx_renderer->bind_pixel_shader(matShaderTable[(uint32_t)material->shaderID]);
            go::dfx_materials::bind_material(texMgr, material, *textureBindings);
        }
        else
        {
            // No shader if this is a depth-only pass
            if(renderPass.depthOnly)
            {
                go::the_gfx_renderer->bind_pixel_shader(nullptr);
            }
            else
            {
                go::the_gfx_renderer->bind_pixel_shader(matShaderTable[(uint32_t)material->shaderID]);
                go::dfx_materials::bind_material(texMgr, material, *textureBindings);
            }
        }
    }
}


// ================================================================================ //
// go::dfx_util::forward_plus_shader_cache functions
// ================================================================================ //

void go::dfx_util::forward_plus_shader_cache::reset(gfx_shader_manager *shaderMgr)
{
    common_shader_cache::reset(shaderMgr);
    
    // Ambient lighting
    ambientLightingPS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "forward_plus\\ambient_lighting\\ambient_lighting_ps.cso");

    // Prepass
    cullSphericalLightsCS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "forward_plus\\prepass\\cull_spherical_lights_cs.cso");

    // Material shaders
    materialShaders[(uint8_t)gfx_material_shader_id::standard] = shaderMgr->get(GO_PATH_SYMBOL_SHADER "forward_plus\\shading\\standard_ps.cso");
    materialShaders[(uint8_t)gfx_material_shader_id::standard_cutout] = shaderMgr->get(GO_PATH_SYMBOL_SHADER "forward_plus\\shading\\standard_alpha_test_ps.cso");
    materialShaders[(uint8_t)gfx_material_shader_id::unlit] = shaderMgr->get(GO_PATH_SYMBOL_SHADER "forward_plus\\shading\\unlit_ps.cso");
}


// ================================================================================ //
// go::dfx_util::forward_plus functions
// ================================================================================ //

go::dfx_util::forward_plus_renderer::forward_plus_renderer(gfx_texture_manager *texMgr, gfx_shader_manager *shaderMgr)
{
    reset(texMgr, shaderMgr);
}


void go::dfx_util::forward_plus_renderer::reset(gfx_texture_manager *texMgr, gfx_shader_manager *shaderMgr)
{
    m_texMgr = texMgr;
    m_shaderCache.reset(shaderMgr);
}


void go::dfx_util::forward_plus_renderer::submit_draw_calls(const forward_plus_render_pass &renderPass) const
{
    // Material cache
    gfx_material_definition *lastMaterial = nullptr;

    auto &materialDB = renderPass.materialDB;
    auto cmdBuffers = renderPass.cmdBuffers;

    // Pre-fetch all command queues
    auto &staticOpaqueCmds = cmdBuffers->staticGeometry.opaque_commands();
    auto &staticOpaqueCmdOrders = cmdBuffers->staticGeometry.opaque_command_order();
    auto &staticInstanceCounts = cmdBuffers->staticGeometry.instance_count_buffer();
    auto &staticTransparentCmds = cmdBuffers->staticGeometry.transparent_commands();

    auto &texMgr = m_texMgr;
    auto cmdOrderIndex = 0;
    auto wasStaticDataUploaded = false;

    if(renderPass.mask != geometry_mask::transparent_only)
    {
        if(!staticOpaqueCmds.empty())
        {
            GO_PERF_BEGIN_EVENT("Opaque Geometry");

            // OM
            bind_blend_and_depth_stencil_state(renderPass, gfx_blend_mode::opaque);

            if(!staticOpaqueCmds.empty())
            {
                GO_PERF_BEGIN_EVENT("Static Geometry");

                // Static vertex shader
                the_gfx_renderer->bind_vertex_shader(renderPass.unskinnedGeometryVS);

                // Upload instance data
                if(!wasStaticDataUploaded)
                {
                    update_static_instance_data(cmdBuffers->staticGeometry);
                    wasStaticDataUploaded = true;
                }

                // Submit the instance groups
                for(auto &numInstances : staticInstanceCounts)
                {
                    auto cmdIndex = staticOpaqueCmdOrders[cmdOrderIndex];
                    auto &cmdTuple = staticOpaqueCmds[cmdIndex];
                    auto &key = std::get<0>(cmdTuple);
                    auto &cmd = std::get<1>(cmdTuple);

                    auto materialID = key.material_id();
                    auto material = materialDB->find(materialID);

                    bind_geometry(cmd, renderPass.depthOnly);
                    bind_instance_constants(cmdOrderIndex, materialID);
                    forward_plus_bind_material(texMgr, m_shaderCache, material, lastMaterial, renderPass);

                    the_gfx_renderer->draw_indexed_instanced(cmd.indexCount, cmd.startIndex, cmd.startVertex, numInstances, 0);

                    cmdOrderIndex += numInstances;
                }

                GO_PERF_END_EVENT;
            }

            GO_PERF_END_EVENT;
        }
    }

    if(renderPass.mask != geometry_mask::opaque_only)
    {
        if(!staticTransparentCmds.empty())
        {
            GO_PERF_BEGIN_EVENT("Transparent Geometry");

            if(!staticTransparentCmds.empty())
            {
                GO_PERF_BEGIN_EVENT("Static Geometry");

                // Static vertex shader
                the_gfx_renderer->bind_vertex_shader(renderPass.unskinnedGeometryVS);

                // Upload instance data
                if(!wasStaticDataUploaded)
                {
                    update_static_instance_data(cmdBuffers->staticGeometry);
                    wasStaticDataUploaded = true;
                }

                for(auto &cmdTuple : staticTransparentCmds)
                {
                    auto &key = std::get<0>(cmdTuple);
                    auto &cmd = std::get<1>(cmdTuple);

                    auto materialID = key.material_id();
                    auto material = materialDB->find(materialID);

                    bind_geometry(cmd, renderPass.depthOnly);
                    bind_instance_constants(cmdOrderIndex, materialID);
                    forward_plus_bind_material(texMgr, m_shaderCache, material, lastMaterial, renderPass);

                    // BS & DSS
                    bind_blend_and_depth_stencil_state(renderPass, key.blend_mode());

                    // Depth-only rendering doesn't require two passes for blending
                    if(renderPass.depthOnly)
                    {
                        the_gfx_renderer->draw_indexed_instanced(cmd.indexCount, cmd.startIndex, cmd.startVertex, 1, 0);
                    }
                    else
                    {
                        // Back faces
                        the_gfx_renderer->bind_rasterizer_state(renderPass.backFacesRS);
                        the_gfx_renderer->draw_indexed_instanced(cmd.indexCount, cmd.startIndex, cmd.startVertex, 1, 0);

                        // Front faces
                        the_gfx_renderer->bind_rasterizer_state(renderPass.frontFacesRS);
                        the_gfx_renderer->draw_indexed_instanced(cmd.indexCount, cmd.startIndex, cmd.startVertex, 1, 0);
                    }

                    ++cmdOrderIndex;
                }

                GO_PERF_END_EVENT;
            }

            GO_PERF_END_EVENT;
        }
    }
}