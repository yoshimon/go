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


#include "go/graphics/d3d11/renderer.h"


#include "go/graphics/d3d11/ui/system.h"
#include "go/graphics/d3d11/display.h"
#include "go/graphics/d3d11/render_paths/dfx_forward_plus.h"
#include "dfx/dfx.h"
#include "dfx/dfx_cpp_types.h"
#include "dfx/namespace_types.h"


static ID3D11ShaderResourceView *s_SRVNullPtr[GO_INTERNAL gfx_srv_pipeline_state::kSlotCount];
static ID3D11UnorderedAccessView *s_UAVNullPtr[GO_INTERNAL gfx_uav_pipeline_state::kSlotCount];


// ================================================================================ //
// Helper functions
// ================================================================================ //

static bool is_pending_input(GO gfx_shader_resource *resource)
{
	for(auto &m : resource->internalPendingInputBindMask)
	{
		if(!m.is_zero())
		{
			return true;
		}
	}

	return false;
}


static bool is_pending_output(GO gfx_shader_resource *resource)
{
	for(auto &m : resource->internalPendingOutputBindMask)
	{
		if(!m.is_zero())
		{
			return true;
		}
	}

	return false;
}


static inline void unbind_from_input(GO gfx_shader_resource *resource,
    decltype(GO gfx_shader_resource::internalInputBindMask) &unbindMasks)
{
    for(size_t j = 0, end = resource->internalInputBindMask.max_size(); j < end; ++j)
    {
        auto &m = resource->internalInputBindMask[j];
        unbindMasks[j] |= m;
        m.clear_all();
    }
}


static inline void unbind_from_output(GO gfx_shader_resource *resource,
    decltype(GO gfx_shader_resource::internalOutputBindMask) &unbindMasks)
{
    for(size_t j = 0, end = resource->internalOutputBindMask.max_size(); j < end; ++j)
    {
        auto &m = resource->internalOutputBindMask[j];
        unbindMasks[j] |= m;
        m.clear_all();
    }
}


static void unbind_from_output(GO gfx_shader_resource *resource,
    decltype(GO gfx_shader_resource::internalOutputBindMask) &unbindMasks, size_t skipIndex)
{
    for(size_t j = 0, end = resource->internalOutputBindMask.max_size(); j < end; ++j)
    {
        if(j != skipIndex)
        {
            auto &m = resource->internalOutputBindMask[j];
            unbindMasks[j] |= m;
            m.clear_all();
        }
    }
}


template<class UnbindFunc, typename... Args>
static void resolve_hazards(GO gfx_shader_resource *resource, size_t slot,
    uint32_t stageIndex,
    decltype(GO gfx_shader_resource::internalInputBindMask) &unbindInputs,
    decltype(GO gfx_shader_resource::internalOutputBindMask) &unbindOutputs,
    UnbindFunc unbindFunc,
    Args... args)
{
    if(is_pending_input(resource))
    {
        // Output -> Input
        unbind_from_output(resource, unbindOutputs);
        unbindFunc(slot, std::forward(args)...);
    }
    else if(is_pending_output(resource))
    {
        // Input -> Output
        unbind_from_input(resource, unbindInputs);

        if(resource->internalPendingOutputBindMask[stageIndex] == 0)
        {
            // Output -> Output (Stage X -> Stage Y)
            unbindFunc(slot, std::forward(args)...);
        }
        else
        {
            // Output -> Output (Stage X -> Stage X)
            unbind_from_output(resource, unbindOutputs, stageIndex);
        }
    }
}


// ================================================================================ //
// go::gfx_renderer member functions
// ================================================================================ //

go::gfx_renderer::gfx_renderer()
: m_frameCounter(1), observer(this, &go::gfx_renderer::on_observe_gpu_display)
{
    setup_pipeline();
    dfx::initialize_all(nullptr);
    initialize_world_render_path();

    the_display->add_observer(this);
}


go::gfx_renderer::~gfx_renderer()
{
    dfx::shutdown_all();
}


auto go::gfx_renderer::render_path() -> gfx_render_path *
{
    return m_renderPath.get();
}


void go::gfx_renderer::setup_pipeline()
{
    // Initialize helper array
    m_currentCommonStages[GO_INTERNAL kGPUPipelineStageVS_SRV] = &m_currentPipelineVS;
    m_currentCommonStages[GO_INTERNAL kGPUPipelineStagePS_SRV] = &m_currentPipelinePS;
    m_currentCommonStages[GO_INTERNAL kGPUPipelineStageCS_SRV] = &m_currentPipelineCS;
    m_currentCommonStages[GO_INTERNAL kGPUPipelineStageGS_SRV] = &m_currentPipelineGS;

    m_desiredCommonStages[GO_INTERNAL kGPUPipelineStageVS_SRV] = &m_desiredPipelineVS;
    m_desiredCommonStages[GO_INTERNAL kGPUPipelineStagePS_SRV] = &m_desiredPipelinePS;
    m_desiredCommonStages[GO_INTERNAL kGPUPipelineStageCS_SRV] = &m_desiredPipelineCS;
    m_desiredCommonStages[GO_INTERNAL kGPUPipelineStageGS_SRV] = &m_desiredPipelineGS;
}


bool go::gfx_renderer::begin_rendering(int32_t viewportWidth, int32_t viewportHeight)
{
    if(!the_display->resize(viewportWidth, viewportHeight))
    {
        return false;
    }

    return true;
}


void go::gfx_renderer::end_rendering()
{
    if(the_gfx_device->back_buffer())
    {
        GO_THROW_HRESULT
        (
            GO_INTERNAL g_dxgiSwapChain->Present(the_display->mode() & gfx_display::synchronize ? 1 : 0, 0),
            "D3D11::Present failed."
        );
        ++m_frameCounter;
    }
}


bool go::gfx_renderer::on_observe_gpu_display(gfx_display *, gfx_display::event_type &e)
{
    switch(e.msg)
    {
    case gfx_display_event_data::event::resized:
        dfx::resize_all();
        m_renderPath->on_resize();
        break;
    }

    return true;
}


void go::gfx_renderer::bind_vertex_buffer(uint32_t slot, gfx_vertex_buffer *buffer)
{
    m_desiredPipelineIA.vertexBuffers.bind_slot(slot, std::move(buffer));
}


void go::gfx_renderer::bind_index_buffer(gfx_index_buffer *buffer, uint32_t startIndex)
{
    if(buffer)
    {
        m_desiredPipelineIA.indexBuffer = buffer->d3d_buffer();
        m_desiredPipelineIA.indexSize = buffer->element_size();
        m_desiredPipelineIA.startIndex = startIndex;
    }
    else
    {
        m_desiredPipelineIA.indexBuffer = 0;
        m_desiredPipelineIA.indexSize = 0;
        m_desiredPipelineIA.startIndex = 0;
    }
}


void go::gfx_renderer::bind_vertex_shader(const gfx_shader *shader)
{
    m_desiredPipelineVS.shader = shader;
}


void go::gfx_renderer::bind_pixel_shader(const gfx_shader *shader)
{
    m_desiredPipelinePS.shader = shader;
}


void go::gfx_renderer::bind_compute_shader(const gfx_shader *shader)
{
    m_desiredPipelineCS.shader = shader;
}


void go::gfx_renderer::bind_geometry_shader(const gfx_shader *shader)
{
    m_desiredPipelineGS.shader = shader;
}


void go::gfx_renderer::clear_render_target(const gfx_render_target *renderTarget, uint32_t index, const float *color)
{
    GO_INTERNAL g_d3d11ImmediateContext->ClearRenderTargetView(renderTarget->d3d_rtv(index), color);
}


void go::gfx_renderer::clear_render_target(const gfx_render_target *renderTarget, uint32_t index, float color)
{
    float clearColor[4] = { color, color, color, color };
    GO_INTERNAL g_d3d11ImmediateContext->ClearRenderTargetView(renderTarget->d3d_rtv(index), clearColor);
}


void go::gfx_renderer::clear_depth_stencil_buffer(const gfx_depth_stencil_buffer *depthStencilBuffer, uint32_t index, float depth, uint8_t stencilRef)
{
    GO_INTERNAL g_d3d11ImmediateContext->ClearDepthStencilView(depthStencilBuffer->d3d_dsv(index), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencilRef);
}


void go::gfx_renderer::clear_depth_stencil_buffer(const gfx_depth_stencil_buffer *depthStencilBuffer, uint32_t index, float depth)
{
    GO_INTERNAL g_d3d11ImmediateContext->ClearDepthStencilView(depthStencilBuffer->d3d_dsv(index), D3D11_CLEAR_DEPTH, depth, 0);
}


void go::gfx_renderer::dispatch_compute(uint32_t x, uint32_t y, uint32_t z)
{
    update_compute_state();
    GO_INTERNAL g_d3d11ImmediateContext->Dispatch(x, y, z);
}


void go::gfx_renderer::draw_indexed(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation)
{
    update_pipeline_state();
    GO_INTERNAL g_d3d11ImmediateContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}


void go::gfx_renderer::draw_indexed_instanced(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation,
    uint32_t numInstances, uint32_t instanceIDBias)
{
    update_pipeline_state();
    GO_INTERNAL g_d3d11ImmediateContext->DrawIndexedInstanced(indexCount, numInstances, startIndexLocation, baseVertexLocation, instanceIDBias);
}


void go::gfx_renderer::draw_triangle()
{
    update_pipeline_state();
    GO_INTERNAL g_d3d11ImmediateContext->Draw(3, 0);
}


void go::gfx_renderer::match_viewport(const gfx_texture *renderTarget)
{
    auto &fmt = renderTarget->format();
    m_desiredPipelineRS.viewports[0] = CD3D11_VIEWPORT(0.f, 0.f, (float)fmt.width, (float)fmt.height);
    m_desiredPipelineRS.numViewports = 1;
}


void go::gfx_renderer::match_viewports(const gfx_texture *renderTarget, uint32_t numViewportsX)
{
    auto &fmt = renderTarget->format();
    auto vpWidth = (float)fmt.width / numViewportsX;
    for(uint32_t vpi = 0; vpi < numViewportsX; ++vpi)
    {
        m_desiredPipelineRS.viewports[vpi] = CD3D11_VIEWPORT(vpi * vpWidth, 0.f, vpWidth, (float)fmt.height);
    }
    m_desiredPipelineRS.numViewports = numViewportsX;
}


void go::gfx_renderer::match_viewports(const gfx_texture *renderTarget, uint32_t numViewportsX, float yOffset, float height)
{
    auto &fmt = renderTarget->format();
    auto vpWidth = (float)fmt.width / numViewportsX;
    for(uint32_t vpi = 0; vpi < numViewportsX; ++vpi)
    {
        m_desiredPipelineRS.viewports[vpi] = CD3D11_VIEWPORT(vpi * vpWidth, yOffset, vpWidth, height);
    }
    m_desiredPipelineRS.numViewports = numViewportsX;
}


void go::gfx_renderer::match_viewport(float x, float y, float w, float h)
{
    m_desiredPipelineRS.viewports[0] = CD3D11_VIEWPORT(x, y, w, h);
    m_desiredPipelineRS.numViewports = 1;
}


void go::gfx_renderer::match_viewport(const rect<float> &bounds)
{
	match_viewport(bounds.left, bounds.top, bounds.width(), bounds.height());
}


void go::gfx_renderer::update_output_merger_counts()
{
    uint32_t numRTVs = 0;
    for(size_t i = 0, end = m_desiredPipelineOM.rtvs.max_size(); i < end; ++i)
    {
        if(m_desiredPipelineOM.rtvs[i])
        {
            numRTVs = i + 1;
        }
    }

    uint32_t numUAVs = 0;
    for(size_t i = 0, end = m_desiredPipelineOM.uavs.max_size(); i < end; ++i)
    {
        if(m_desiredPipelineOM.uavs[i])
        {
            numUAVs = i + 1;
        }
    }

    m_desiredPipelineOM.numRTVs = numRTVs;
    m_desiredPipelineOM.numUAVs = numUAVs;
}


void go::gfx_renderer::bind_render_target(uint32_t slot, gfx_render_target *renderTarget, uint32_t index)
{
    bind_render_target(m_desiredPipelineOM, slot, renderTarget, index);
}


void go::gfx_renderer::bind_render_target(GO_INTERNAL gfx_output_merger_stage &stage, uint32_t slot,
    gfx_render_target *renderTarget, uint32_t index)
{
    auto d3dRTV = renderTarget ? renderTarget->d3d_rtv(index) : nullptr;
    /*
    if(m_currentPipelineOM.d3dRTVs[slot] == d3dRTV)
    {
        return;
    }
    */

    unmark_pending_output(stage.rtvs[slot], GO_INTERNAL kGPUPipelineStageOM_RTV, slot);
    unmark_pending_input(renderTarget);
    mark_pending_output(renderTarget, GO_INTERNAL kGPUPipelineStageOM_RTV, slot);
    stage.rtvs[slot] = renderTarget;
    stage.d3dRTVs[slot] = d3dRTV;
}


void go::gfx_renderer::bind_vertex_shader_constants(uint32_t slot, gfx_constant_buffer *buffer)
{
    m_desiredPipelineVS.constantBuffers.bind_slot(slot, buffer);
}


void go::gfx_renderer::bind_pixel_shader_constants(uint32_t slot, gfx_constant_buffer *buffer)
{
    m_desiredPipelinePS.constantBuffers.bind_slot(slot, buffer);
}


void go::gfx_renderer::bind_geometry_shader_constants(uint32_t slot, gfx_constant_buffer *buffer)
{
    m_desiredPipelineGS.constantBuffers.bind_slot(slot, buffer);
}


void go::gfx_renderer::bind_compute_shader_constants(uint32_t slot, gfx_constant_buffer *buffer)
{
    m_desiredPipelineCS.constantBuffers.bind_slot(slot, buffer);
}


void go::gfx_renderer::bind_vertex_shader_sampler(uint32_t slot, gfx_sampler_state *sampler)
{
    m_desiredPipelineVS.samplerStates.bind_slot(slot, sampler);
}


void go::gfx_renderer::bind_pixel_shader_sampler(uint32_t slot, gfx_sampler_state *sampler)
{
    m_desiredPipelinePS.samplerStates.bind_slot(slot, sampler);
}


void go::gfx_renderer::bind_geometry_shader_sampler(uint32_t slot, gfx_sampler_state *sampler)
{
    m_desiredPipelineGS.samplerStates.bind_slot(slot, sampler);
}


void go::gfx_renderer::bind_compute_shader_sampler(uint32_t slot, gfx_sampler_state *sampler)
{
    m_desiredPipelineCS.samplerStates.bind_slot(slot, sampler);
}


void go::gfx_renderer::bind_vertex_shader_resource(uint32_t slot, gfx_shader_resource *resource, uint32_t index)
{
    bind_shader_resource(GO_INTERNAL kGPUPipelineStageVS_SRV, slot, resource, index);
}


void go::gfx_renderer::bind_pixel_shader_resource(uint32_t slot, gfx_shader_resource *resource, uint32_t index)
{
    bind_shader_resource(GO_INTERNAL kGPUPipelineStagePS_SRV, slot, resource, index);
}


void go::gfx_renderer::bind_geometry_shader_resource(uint32_t slot, gfx_shader_resource *resource, uint32_t index)
{
    bind_shader_resource(GO_INTERNAL kGPUPipelineStageGS_SRV, slot, resource, index);
}


void go::gfx_renderer::bind_compute_shader_resource(uint32_t slot, gfx_shader_resource *resource, uint32_t index)
{
    bind_shader_resource(GO_INTERNAL kGPUPipelineStageCS_SRV, slot, resource, index);
}


void go::gfx_renderer::bind_compute_shader_uav(uint32_t slot, gfx_shader_resource *uav, uint32_t index, uint32_t initialCount)
{
    bind_compute_shader_uav(m_desiredPipelineCS, slot, uav, index, initialCount);
}


void go::gfx_renderer::bind_compute_shader_uav(GO_INTERNAL gfx_compute_shader_stage &stage, uint32_t slot,
    gfx_shader_resource *uav, uint32_t index, uint32_t initialCount)
{
    unmark_pending_output(stage.uavs.bind_slot(slot, std::move(uav), index, initialCount), GO_INTERNAL kGPUPipelineStageCS_UAV, slot);
    unmark_pending_input(uav);
    mark_pending_output(uav, GO_INTERNAL kGPUPipelineStageCS_UAV, slot);
}


void go::gfx_renderer::bind_output_merger_uav(uint32_t slot, gfx_shader_resource *uav, uint32_t index, uint32_t initialCount)
{
    bind_output_merger_uav(m_desiredPipelineOM, slot, uav, index, initialCount);
}


void go::gfx_renderer::bind_output_merger_uav(GO_INTERNAL gfx_output_merger_stage &stage, uint32_t slot,
    gfx_shader_resource *uav, uint32_t index, uint32_t initialCount)
{
    unmark_pending_output(stage.uavs[slot], GO_INTERNAL kGPUPipelineStageOM_UAV, slot);
    unmark_pending_input(uav);
    mark_pending_output(uav, GO_INTERNAL kGPUPipelineStageOM_UAV, slot);
    stage.uavs[slot] = uav;
    stage.d3dUAVs[slot] = uav ? uav->d3d_uav(index) : nullptr;
    stage.uavInitialCount[slot] = initialCount;
}


void go::gfx_renderer::bind_blend_state(const gfx_blend_state *state, float blendFactor[4])
{
    bind_blend_state(state);
    m_desiredPipelineOM.blendFactor[0] = blendFactor[0];
    m_desiredPipelineOM.blendFactor[1] = blendFactor[1];
    m_desiredPipelineOM.blendFactor[2] = blendFactor[2];
    m_desiredPipelineOM.blendFactor[3] = blendFactor[3];
}


void go::gfx_renderer::bind_blend_state(const gfx_blend_state *state)
{
    m_desiredPipelineOM.blendState = state ? state->d3d_blend_state() : nullptr;
}


void go::gfx_renderer::bind_rasterizer_state(const gfx_rasterizer_state *state)
{
    m_desiredPipelineRS.rasterizerState = state ? state->d3d_rasterizer_state() : nullptr;
}


void go::gfx_renderer::bind_depth_stencil_state(const gfx_depth_stencil_state *state, uint32_t stencilRef)
{
    m_desiredPipelineOM.depthStencilState = state ? state->d3d_depth_stencil_state() : nullptr;
    m_desiredPipelineOM.stencilRef = stencilRef;
}


void go::gfx_renderer::bind_depth_stencil_buffer(gfx_depth_stencil_buffer *depthStencilBuffer, uint32_t index)
{
    bind_depth_stencil_buffer(m_desiredPipelineOM, depthStencilBuffer, index);
}


void go::gfx_renderer::bind_depth_stencil_buffer(GO_INTERNAL gfx_output_merger_stage &stage,
    gfx_depth_stencil_buffer *depthStencilBuffer, uint32_t index)
{
    unmark_pending_input(depthStencilBuffer);
    unmark_pending_output(depthStencilBuffer);
    stage.depthStencilBuffer = depthStencilBuffer;
    stage.depthStencilIndex = index;
}


void go::gfx_renderer::bind_input_topology(gfx_input_topology topology)
{
    m_desiredPipelineIA.primitiveTopology = (D3D11_PRIMITIVE_TOPOLOGY)topology;
}


void go::gfx_renderer::clear_state()
{
    _clear_state();
    GO_INTERNAL g_d3d11ImmediateContext->ClearState();
}


void go::gfx_renderer::unbind_output_merger_targets()
{
    // RTV
    for(size_t i = 0, end = m_desiredPipelineOM.d3dRTVs.max_size(); i < end; ++i)
    {
        bind_render_target(i, nullptr, 0);
    }

    // OM UAV
    for(size_t i = 0, end = m_desiredPipelineOM.d3dUAVs.max_size(); i < end; ++i)
    {
        bind_output_merger_uav(i, nullptr, 0);
    }

    // DSV
    bind_depth_stencil_buffer(nullptr, 0);
}


void go::gfx_renderer::initialize_world_render_path()
{
    m_renderPath = std::make_unique<dfx_forward_plus_render_path>(this);
}


void go::gfx_renderer::update_pipeline_state()
{
    resolve_resource_hazards();
    m_desiredPipelineIA.bind(m_currentPipelineIA);
    m_desiredPipelineVS.bind(m_currentPipelineVS);
    m_desiredPipelinePS.bind(m_currentPipelinePS);
    m_desiredPipelineGS.bind(m_currentPipelineGS);
	m_desiredPipelineRS.bind(m_currentPipelineRS);
    m_desiredPipelineCS.bind(m_currentPipelineCS);
}


void go::gfx_renderer::bind_default_pipeline_state()
{
    unbind_output_merger_targets();
    bind_depth_stencil_state(nullptr, 0);
    bind_blend_state(nullptr);
    bind_rasterizer_state(nullptr);

    for(size_t i = 0; i < m_desiredPipelineIA.vertexBuffers.kSlotCount; ++i)
    {
        bind_vertex_buffer(i, nullptr);
    }
    
    bind_index_buffer(nullptr, 0);
    bind_input_topology(gfx_input_topology::triangle_list);

	for(int i = 0; i < m_desiredPipelineVS.constantBuffers.kSlotCount; ++i)
	{
		bind_vertex_shader_constants(i, nullptr);
        bind_pixel_shader_constants(i, nullptr);
        bind_geometry_shader_constants(i, nullptr);
        bind_compute_shader_constants(i, nullptr);
	}

	for(int i = 0; i < m_desiredPipelineVS.shaderResources.kSlotCount; ++i)
	{
		bind_vertex_shader_resource(i, nullptr, 0);
		bind_pixel_shader_resource(i, nullptr, 0);
        bind_geometry_shader_resource(i, nullptr, 0);
        bind_compute_shader_resource(i, nullptr, 0);
	}

    for(int i = 0; i < m_desiredPipelineCS.uavs.kSlotCount; ++i)
    {
        bind_compute_shader_uav(i, nullptr, 0);
    }
}


void go::gfx_renderer::update_compute_state()
{
    resolve_resource_hazards();
    m_desiredPipelineCS.bind(m_currentPipelineCS);
}


void go::gfx_renderer::_clear_state()
{
    m_currentPipelineIA.reset();
    m_desiredPipelineIA.reset();

    m_currentPipelineVS.reset();
    m_desiredPipelineVS.reset();

    m_currentPipelinePS.reset();
    m_desiredPipelinePS.reset();

    m_currentPipelineGS.reset();
    m_desiredPipelineGS.reset();

    m_currentPipelineRS.reset();
    m_desiredPipelineRS.reset();

    m_currentPipelineOM.reset();
    m_desiredPipelineOM.reset();

    m_currentPipelineCS.reset();
    m_desiredPipelineCS.reset();
}


void go::gfx_renderer::bind_shader_resource(uint32_t stageIndex, uint32_t slot, gfx_shader_resource *resource, uint32_t index)
{
    auto oldSlot = m_desiredCommonStages[stageIndex]->shaderResources.bind_slot(slot, resource, index);
    unmark_pending_input(oldSlot, stageIndex, slot);
    unmark_pending_output(resource);
    mark_pending_input(resource, stageIndex, slot);
}


void go::gfx_renderer::unbind_srvs(uint32_t stageIndex, size_t groupStart, size_t groupSize)
{
    auto &cs = m_currentCommonStages[stageIndex];
    auto &ds = m_desiredCommonStages[stageIndex];
    for(size_t i = groupStart, end = groupStart + groupSize; i < end; ++i)
    {
        // FIXME: should we take the SRV index into consideration here?
        // If the SRV remains unchanged we need to get rid of it manually
        auto currentSRV = std::get<0>(cs->shaderResources[i]);
        auto desiredSRV = std::get<0>(ds->shaderResources[i]);
        if(currentSRV == desiredSRV)
        {
            bind_shader_resource(stageIndex, i, nullptr, 0);
        }

        // It's no longer bound here
        currentSRV->internalInputBindMask[stageIndex].clear(i);

        // Overwrite with zero
        cs->shaderResources[i] = std::make_tuple(nullptr, 0);
    }
}


void go::gfx_renderer::unbind_compute_shader_uavs(size_t groupStart, size_t groupSize)
{
    for(size_t i = groupStart, end = groupStart + groupSize; i < end; ++i)
    {
        // If the UAV remains unchanged we need to get rid of it manually
        auto &currentUAV = m_currentPipelineCS.uavs[i];
        auto &desiredUAV = m_desiredPipelineCS.uavs[i];
        if(currentUAV == desiredUAV)
        {
            bind_compute_shader_uav(i, nullptr, 0);
        }

        // It's no longer bound here
        std::get<0>(currentUAV)->internalOutputBindMask[GO_INTERNAL kGPUPipelineStageCS_UAV].clear(i);
        
        // Overwrite with zero
        currentUAV = std::make_tuple(nullptr, 0, 0);
    }
}


void go::gfx_renderer::resolve_resource_hazards()
{
    update_output_merger_counts();

    decltype(gfx_shader_resource::internalInputBindMask) unbindInputs;
    decltype(gfx_shader_resource::internalOutputBindMask) unbindOutputs;

    // NOTE: the total number of D3D calls can be further reduced

    // Resolve hazards by tagging them and accumulating the input & output masks
    resolve_input_hazards(unbindInputs, unbindOutputs); // Input -> Output
    resolve_output_hazards(unbindInputs, unbindOutputs); // Output -> Input, Output -> Output

    // Unbind based on the accumulated masks
    unbind_resource_hazards(unbindInputs, unbindOutputs);

    update_output_merger_counts();

    // These two have to be bound before any other stage can be bound
    m_desiredPipelineOM.bind(m_currentPipelineOM);
    m_desiredPipelineCS.bind_uavs(m_currentPipelineCS);
}


void go::gfx_renderer::resolve_input_hazards(
    decltype(GO gfx_shader_resource::internalInputBindMask) &unbindInputs,
    decltype(GO gfx_shader_resource::internalOutputBindMask) &unbindOutputs)
{
    // Input -> Output
    // RTV
    for(size_t i = 0; i < m_desiredPipelineOM.numRTVs; ++i)
    {
        auto rtv = m_desiredPipelineOM.rtvs[i];
        if(rtv)
        {
            // RTV will be bound to OM
            if(rtv->internalPendingOutputBindMask[GO_INTERNAL kGPUPipelineStageOM_RTV] != 0)
            {
                unbind_from_input(rtv, unbindInputs);
                unbind_from_output(rtv, unbindOutputs, GO_INTERNAL kGPUPipelineStageOM_RTV);
            }
        }
    }

    // UAV
    for(size_t i = 0; i < m_desiredPipelineOM.numUAVs; ++i)
    {
        auto uav = m_desiredPipelineOM.uavs[i];
        if(uav)
        {
            // UAV will be bound to OM
            if(uav->internalPendingOutputBindMask[GO_INTERNAL kGPUPipelineStageOM_UAV] != 0)
            {
                unbind_from_input(uav, unbindInputs);
                unbind_from_output(uav, unbindOutputs, GO_INTERNAL kGPUPipelineStageOM_UAV);
            }
        }
    }

    // CS UAV
    m_desiredPipelineCS.uavs.modified_mask().for_each_set([this](size_t slot,
        decltype(GO gfx_shader_resource::internalInputBindMask) &unbindInputs,
        decltype(GO gfx_shader_resource::internalOutputBindMask) &unbindOutputs)
    {
        auto uav = std::get<0>(m_desiredPipelineCS.uavs[slot]);
        if(uav)
        {
            // UAV will be bound to CS
            if(uav->internalPendingOutputBindMask[GO_INTERNAL kGPUPipelineStageCS_UAV] != 0)
            {
                unbind_from_input(uav, unbindInputs);
                unbind_from_output(uav, unbindOutputs, GO_INTERNAL kGPUPipelineStageCS_UAV);
            }
        }
    }, unbindInputs, unbindOutputs);

    // DSV
    {
        auto dsb = m_desiredPipelineOM.depthStencilBuffer;
        if(dsb)
        {
            unbind_from_input(dsb, unbindInputs);
            unbind_from_output(dsb, unbindOutputs);
        }
    }
}


void go::gfx_renderer::resolve_output_hazards(
    decltype(GO gfx_shader_resource::internalInputBindMask) &unbindInputs,
    decltype(GO gfx_shader_resource::internalOutputBindMask) &unbindOutputs)
{
    // Output -> Input
    // Output -> Output
    // DSV
    {
        auto dsb = m_currentPipelineOM.depthStencilBuffer;
        if(dsb)
        {
            auto isPendingInput = is_pending_input(dsb);
            auto isPendingOutput = is_pending_output(dsb);
            if(isPendingInput)
            {
                // Output -> Input
                unbind_from_output(dsb, unbindOutputs);
            }
            else if(isPendingOutput)
            {
                // Input -> Output
                unbind_from_input(dsb, unbindInputs);
            }

            if(isPendingInput || isPendingOutput)
            {
                // Unbind the depth-buffer if it remains unchanged
                if(dsb == m_desiredPipelineOM.depthStencilBuffer &&
                    m_currentPipelineOM.depthStencilIndex == m_desiredPipelineOM.depthStencilIndex)
                {
                    bind_depth_stencil_buffer(nullptr, 0);
                }
            }
        }
    }

    // RTV
    for(size_t i = 0; i < m_currentPipelineOM.numRTVs; ++i)
    {
        auto rtv = m_currentPipelineOM.rtvs[i];
        if(rtv)
        {
            resolve_hazards(rtv, i, GO_INTERNAL kGPUPipelineStageOM_RTV, unbindInputs, unbindOutputs, [this](size_t slot)
            {
                if(m_desiredPipelineOM.d3dRTVs[slot] == m_currentPipelineOM.d3dRTVs[slot])
                {
                    bind_render_target(slot, nullptr, 0);
                }
            });
        }
        else
        {
            // GO_MSGASSERT(rtv, "Unexpected nullptr encountered in render-target array!");
        }
    }

    // UAV
    for(size_t i = 0; i < m_currentPipelineOM.numUAVs; ++i)
    {
        auto uav = m_currentPipelineOM.uavs[i];
        if(uav)
        {
            resolve_hazards(uav, i, GO_INTERNAL kGPUPipelineStageOM_UAV, unbindInputs, unbindOutputs, [this](size_t slot)
            {
                if(m_desiredPipelineOM.d3dUAVs[slot] == m_currentPipelineOM.d3dUAVs[slot])
                {
                    bind_output_merger_uav(slot, nullptr, 0);
                }
            });
        }
        else
        {
            // GO_MSGASSERT(uav, "Unexpected nullptr encountered in UAV array!");
        }
    }

    // CS UAV
    for(size_t i = 0; i < GO_INTERNAL gfx_uav_pipeline_state::kSlotCount; ++i)
    {
        auto uav = std::get<0>(m_currentPipelineCS.uavs[i]);
        if(uav)
        {
            resolve_hazards(uav, i, GO_INTERNAL kGPUPipelineStageCS_UAV, unbindInputs, unbindOutputs, [this](size_t slot)
            {
                if(m_desiredPipelineCS.uavs[slot] == m_currentPipelineCS.uavs[slot])
                {
                    bind_compute_shader_uav(slot, nullptr, 0);
                }
            });
        }
    }
}

void go::gfx_renderer::unbind_resource_hazards(
    decltype(GO gfx_shader_resource::internalInputBindMask) &unbindInputs,
    decltype(GO gfx_shader_resource::internalOutputBindMask) &unbindOutputs)
{
    // Unbind inputs
    unbindInputs[GO_INTERNAL kGPUPipelineStageVS_SRV].for_each_groupset([this](size_t groupStart, size_t groupSize)
    {
        GO_INTERNAL g_d3d11ImmediateContext->VSSetShaderResources(groupStart, groupSize, s_SRVNullPtr);
        unbind_srvs(GO_INTERNAL kGPUPipelineStageVS_SRV, groupStart, groupSize);
    });
    m_desiredPipelineVS.shaderResources.modified_mask() |= unbindInputs[GO_INTERNAL kGPUPipelineStageVS_SRV];

    unbindInputs[GO_INTERNAL kGPUPipelineStagePS_SRV].for_each_groupset([this](size_t groupStart, size_t groupSize)
    {
        GO_INTERNAL g_d3d11ImmediateContext->PSSetShaderResources(groupStart, groupSize, s_SRVNullPtr);
        unbind_srvs(GO_INTERNAL kGPUPipelineStagePS_SRV, groupStart, groupSize);
    });
    m_desiredPipelinePS.shaderResources.modified_mask() |= unbindInputs[GO_INTERNAL kGPUPipelineStagePS_SRV];

    unbindInputs[GO_INTERNAL kGPUPipelineStageGS_SRV].for_each_groupset([this](size_t groupStart, size_t groupSize)
    {
        GO_INTERNAL g_d3d11ImmediateContext->GSSetShaderResources(groupStart, groupSize, s_SRVNullPtr);
        unbind_srvs(GO_INTERNAL kGPUPipelineStageGS_SRV, groupStart, groupSize);
    });
    m_desiredPipelineGS.shaderResources.modified_mask() |= unbindInputs[GO_INTERNAL kGPUPipelineStageGS_SRV];

    unbindInputs[GO_INTERNAL kGPUPipelineStageCS_SRV].for_each_groupset([this](size_t groupStart, size_t groupSize)
    {
        GO_INTERNAL g_d3d11ImmediateContext->CSSetShaderResources(groupStart, groupSize, s_SRVNullPtr);
        unbind_srvs(GO_INTERNAL kGPUPipelineStageCS_SRV, groupStart, groupSize);
    });
    m_desiredPipelineCS.shaderResources.modified_mask() |= unbindInputs[GO_INTERNAL kGPUPipelineStageCS_SRV];

    // Unbind outputs
    unbindOutputs[GO_INTERNAL kGPUPipelineStageOM_RTV].for_each_set([this](size_t slot)
    {
        // Unbind if the slot is not overwritten
        if(m_desiredPipelineOM.d3dRTVs[slot] == m_currentPipelineOM.d3dRTVs[slot])
        {
            bind_render_target(slot, nullptr, 0);
        }
    });
    unbindOutputs[GO_INTERNAL kGPUPipelineStageOM_UAV].for_each_set([this](size_t slot)
    {
        // Unbind if the slot is not overwritten
        if(m_desiredPipelineOM.d3dUAVs[slot] == m_currentPipelineOM.d3dUAVs[slot])
        {
            bind_output_merger_uav(slot, nullptr, 0);
        }
    });
    unbindOutputs[GO_INTERNAL kGPUPipelineStageCS_UAV].for_each_groupset([this](size_t groupStart, size_t groupSize)
    {
        GO_INTERNAL g_d3d11ImmediateContext->CSSetUnorderedAccessViews(groupStart, groupSize, s_UAVNullPtr, nullptr);
        unbind_compute_shader_uavs(groupStart, groupSize);
    });
    m_desiredPipelineCS.uavs.modified_mask() |= unbindOutputs[GO_INTERNAL kGPUPipelineStageCS_UAV];
}


void go::gfx_renderer::unmark_pending_output(gfx_shader_resource *resource)
{
    if(resource)
    {
        // OM RTV
        {
            auto &bm = resource->internalPendingOutputBindMask[GO_INTERNAL kGPUPipelineStageOM_RTV];
            bm.for_each_set([this](size_t slot)
            {
                m_desiredPipelineOM.rtvs[slot] = nullptr;
                m_desiredPipelineOM.d3dRTVs[slot] = nullptr;
            });
            bm.clear_all();
        }

        // OM UAV
        {
            auto &bm = resource->internalPendingOutputBindMask[GO_INTERNAL kGPUPipelineStageOM_UAV];
            bm.for_each_set([this](size_t slot)
            {
                m_desiredPipelineOM.uavs[slot] = nullptr;
                m_desiredPipelineOM.d3dUAVs[slot] = nullptr;
                m_desiredPipelineOM.uavInitialCount[slot] = 0;
            });
            bm.clear_all();
        }

        // CS UAV
        {
            auto &bm = resource->internalPendingOutputBindMask[GO_INTERNAL kGPUPipelineStageCS_UAV];
            m_desiredPipelineCS.uavs.unbind_by_mask(resource, bm);
            bm.clear_all();
        }
    }
}


void go::gfx_renderer::unmark_pending_input(gfx_shader_resource *resource)
{
    if(resource)
    {
        // Revert all pending inputs
        for(size_t i = 0, end = resource->internalPendingInputBindMask.max_size(); i < end; ++i)
        {
            auto &m = resource->internalPendingInputBindMask[i];
            m_desiredCommonStages[i]->shaderResources.unbind_by_mask(resource, m);
            m.clear_all();
        }
    }
}


void go::gfx_renderer::unmark_pending_output(gfx_shader_resource *resource, uint32_t stageIndex, uint32_t slot)
{
    if(resource)
    {
        resource->internalPendingOutputBindMask[stageIndex].clear(slot);
    }
}


void go::gfx_renderer::unmark_pending_input(gfx_shader_resource *resource, uint32_t stageIndex, uint32_t slot)
{
    if(resource)
    {
        resource->internalPendingInputBindMask[stageIndex].clear(slot);
    }
}


void go::gfx_renderer::mark_pending_input(gfx_shader_resource *resource, uint32_t stageIndex, uint32_t slot)
{
    if(resource)
    {
        resource->internalPendingInputBindMask[stageIndex].set(slot);
    }
}


void go::gfx_renderer::mark_pending_output(gfx_shader_resource *resource, uint32_t stageIndex, uint32_t slot)
{
    if(resource)
    {
        resource->internalPendingOutputBindMask[stageIndex].set(slot);
    }
}