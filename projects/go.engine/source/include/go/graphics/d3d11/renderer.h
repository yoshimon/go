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
    \file graphics/d3d11/renderer.h
    \brief The renderer implementation class.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/graphics/d3d11/display.h"
#include "go/graphics/d3d11/device.h"
#include "go/graphics/d3d11/rasterizer_state.h"
#include "go/graphics/d3d11/pipeline_state.h"
#include "go/graphics/d3d11/input_topology.h"
#include "go/graphics/render_path.h"
#include "dfx/namespace_types.h"


GO_BEGIN_NAMESPACE

class gfx_index_buffer;
class gfx_vertex_buffer;
struct gfx_render_context;


//! A class, which implements a D3D11 world-renderer.
class GO_API gfx_renderer : public observer<gfx_renderer, gfx_display>
{
public:
    //! Constructor.
    gfx_renderer();
    //! Destructor.
    ~gfx_renderer();
public:
    /*!
        Begins the process of rendering a new frame.

        Resets the renderer to a default state.

        \param viewportWidth The target viewport width.
        \param viewportHeight The target viewport height.

        \return Returns true, if the renderer is in a valid state. Otherwise, the return value is false.
     */
    bool begin_rendering(int32_t viewportWidth, int32_t viewportHeight);
    /*!
        Ends the process of rendering the current frame.

        Flushes the draw command pipeline.
     */
    void end_rendering();
    /*!
        Binds a vertex buffer to the renderer.

        \param slot The slot to bind.
        \param buffer The buffer to bind.
     */
    void bind_vertex_buffer(uint32_t slot, gfx_vertex_buffer *buffer);
    /*!
        Binds an index buffer to the renderer.

        \param buffer The buffer to bind.
        \param startIndex The index offset to apply to the buffer.
     */
    void bind_index_buffer(gfx_index_buffer *buffer, uint32_t startIndex);
    /*!
        Binds a vertex shader to the renderer.

        \param shader The shader to bind.
     */
    void bind_vertex_shader(const gfx_shader *shader);
    /*!
        Binds a pixel shader to the renderer.

        \param shader The shader to bind.
     */
    void bind_pixel_shader(const gfx_shader *shader);
    /*!
        Binds a pixel shader to the renderer.

        \param shader The shader to bind.
     */
    void bind_geometry_shader(const gfx_shader *shader);
    /*!
        Binds a compute shader to the renderer.

        \param shader The shader to bind.
     */
    void bind_compute_shader(const gfx_shader *shader);
    /*!
        Clears a render-target.

        \param renderTarget The render-target to clear.
        \param color The clear-color.
        \param index The index of the internal render-target.
     */
    void clear_render_target(const gfx_render_target *renderTarget, uint32_t index, const float *color);
    //! \overload
    void clear_render_target(const gfx_render_target *renderTarget, uint32_t index, float color);
    /*!
        Clears a depth-stencil buffer.

        \param depthStencilBuffer The depth-stencil buffer to clear.
        \param index The index of the depth-stencil view to clear.
        \param depth The depth value to write.
        \param stencilRef The stencil value to write.
     */
    void clear_depth_stencil_buffer(const gfx_depth_stencil_buffer *depthStencilBuffer, uint32_t index, float depth, uint8_t stencilRef);
    //! \overload
    void clear_depth_stencil_buffer(const gfx_depth_stencil_buffer *depthStencilBuffer, uint32_t index, float depth);
    /*!
        Returns the frame counter.

        \return The number of frames rendered.
     */
    uint32_t frame_counter() const noexcept { return m_frameCounter; }
    /*!
        Executes the current compute shader.

        \param x The number of thread groups.
        \param y The number of thread groups.
        \param z The number of thread groups.
     */
    void dispatch_compute(uint32_t x, uint32_t y, uint32_t z);
    /*!
        Draws the current primitive buffer using index rendering.

        \param indexCount The number of indices.
        \param startIndexLocation The offset to the first index.
        \param baseVertexLocation The offset to apply to the indices.
     */
    void draw_indexed(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation);
    /*!
        Draws the current primitive buffer using index rendering.

        \param indexCount The number of indices.
        \param startIndexLocation The offset to the first index.
        \param baseVertexLocation The offset to apply to the indices.
        \param numInstances The number of instances.
        \param instanceIDBias The instanceID bias.
     */
    void draw_indexed_instanced(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation,
        uint32_t numInstances, uint32_t instanceIDBias);
    //! Emits three vertices to the pipeline.
    void draw_triangle();
    /*!
        Matches the viewport to a given render-target.

        \param renderTarget The render-target to match the viewport to.
     */
    void match_viewport(const gfx_texture *renderTarget);
    /*!
        Binds a set of viewports for a given render-target.

        Matches the viewports using horizontal viewport tiling.

        \param renderTarget The render-target to match the viewports to.
        \param numViewportsX The number of viewports to bind.
     */
    void match_viewports(const gfx_texture *renderTarget, uint32_t numViewportsX);
    //! \overload
    void match_viewports(const gfx_texture *renderTarget, uint32_t numViewportsX, float yOffset, float height);
    //! \overload
    void match_viewport(float x, float y, float w, float h);
	//! \overload
	void match_viewport(const go::rect<float> &bounds);
    /*!
        Binds a render-target to the pipeline.

        \param slot The slot to bind the render-target to.
        \param renderTarget The render-target to bind.
        \param index The sub-resource index.
     */
    void bind_render_target(uint32_t slot, gfx_render_target *renderTarget, uint32_t index);
    /*!
        Binds a constant buffer to the vertex shader.

        \param slot The slot to bind to.
        \param buffer The buffer to use.
     */
    void bind_vertex_shader_constants(uint32_t slot, gfx_constant_buffer *buffer);
    /*!
        Binds a constant buffer to the pixel shader.

        \param slot The slot to bind to.
        \param buffer The buffer to use.
     */
    void bind_pixel_shader_constants(uint32_t slot, gfx_constant_buffer *buffer);
    /*!
        Binds a constant buffer to the geometry shader.

        \param slot The slot to bind to.
        \param buffer The buffer to use.
     */
    void bind_geometry_shader_constants(uint32_t slot, gfx_constant_buffer *buffer);
    /*!
        Binds a constant buffer to the compute shader.

        \param slot The slot to bind to.
        \param buffer The buffer to use.
     */
    void bind_compute_shader_constants(uint32_t slot, gfx_constant_buffer *buffer);
    /*!
        Binds a sampler state to the vertex shader.

        \param slot The slot to bind to.
        \param buffer The buffer to use.
     */
    void bind_vertex_shader_sampler(uint32_t slot, gfx_sampler_state *sampler);
    /*!
        Binds a sampler state to the pixel shader.

        \param slot The slot to bind to.
        \param buffer The buffer to use.
     */
    void bind_pixel_shader_sampler(uint32_t slot, gfx_sampler_state *sampler);
    /*!
        Binds a sampler state to the geometry shader.

        \param slot The slot to bind to.
        \param buffer The buffer to use.
     */
    void bind_geometry_shader_sampler(uint32_t slot, gfx_sampler_state *sampler);
    /*!
        Binds a sampler state to the compute shader.

        \param slot The slot to bind to.
        \param buffer The buffer to use.
     */
    void bind_compute_shader_sampler(uint32_t slot, gfx_sampler_state *sampler);
    /*!
        Binds a resource to the vertex shader.

        \param slot The slot to bind the resource to.
        \param resource The resource to bind.
        \param index The resource sub-index.
     */
    void bind_vertex_shader_resource(uint32_t slot, gfx_shader_resource *resource, uint32_t index);
    /*!
        Binds a resource to the pixel shader.

        \param slot The slot to bind the resource to.
        \param resource The resource to bind.
        \param index The resource sub-index.
     */
    void bind_pixel_shader_resource(uint32_t slot, gfx_shader_resource *resource, uint32_t index);
    /*!
        Binds a resource to the geometry shader.

        \param slot The slot to bind the resource to.
        \param resource The resource to bind.
        \param index The resource sub-index.
     */
    void bind_geometry_shader_resource(uint32_t slot, gfx_shader_resource *resource, uint32_t index);
    /*!
        Binds a resource to the compute shader.

        \param slot The slot to bind the resource to.
        \param resource The resource to bind.
        \param index The resource sub-index.
     */
    void bind_compute_shader_resource(uint32_t slot, gfx_shader_resource *resource, uint32_t index);
    /*!
        Binds a UAV to the compute shader.

        \param slot The slot to bind the resource to.
        \param uav The UAV to bind.
        \param index The UAV resource-sub-index.
        \param initialCount The initial UAV counter.
     */
    void bind_compute_shader_uav(uint32_t slot, gfx_shader_resource *uav, uint32_t index, uint32_t initialCount = 0);
    /*!
        Binds a UAV to the output merger.

        \param slot The slot to bind the resource to.
        \param uav The UAV to bind.
        \param index The UAV resource-sub-index.
        \param initialCount The initial UAV counter.
     */
    void bind_output_merger_uav(uint32_t slot, gfx_shader_resource *uav, uint32_t index, uint32_t initialCount = 0);
    /*!
        Binds the blend-state.

        \param state The state to bind.
        \param blendFactor The blend-factor.
     */
    void bind_blend_state(const gfx_blend_state *state, float blendFactor[4]);
    //! \overload
    void bind_blend_state(const gfx_blend_state *state);
    /*!
        Binds the rasterizer-state.

        \param state The state to bind.
     */
    void bind_rasterizer_state(const gfx_rasterizer_state *state);
    /*!
        Binds the depth-stencil state.

        \param state The state to bind.
        \param stencilRef The stencil-reference value to use for comparison tests.
     */
    void bind_depth_stencil_state(const gfx_depth_stencil_state *state, uint32_t stencilRef);
    /*!
        Binds the depth-stencil buffer.

        \param depthStencilBuffer The buffer to bind.
        \param index The DSV resource-sub-index.
     */
    void bind_depth_stencil_buffer(gfx_depth_stencil_buffer *depthStencilBuffer, uint32_t index);
    /*!
        Changes the input topology.

        \param topology the new input topology.
     */
    void bind_input_topology(gfx_input_topology topology);
    //! Resets the device state.
    void clear_state();
    //! Unbinds all target buffers from the output merger.
    void unbind_output_merger_targets();
    /*!
        Returns the render-path.

        \return The render-path.
     */
    gfx_render_path *render_path();
    //! Updates the pipeline state.
    void update_pipeline_state();
    //! Binds the default pipeline state.
    void bind_default_pipeline_state();
private:
    /*!
        Observer function.

        \param e The event.
     */
    bool on_observe_gpu_display(gfx_display *, gfx_display::event_type &e);
    //! Creates the world render path.
    void initialize_world_render_path();
    //! Updates the compute-stage state.
    void update_compute_state();
    //! Internal clear-state function.
    void _clear_state();
    //! Resolves all resource hazards.
    void resolve_resource_hazards();
    /*!
        Unmarks a resource to no longer be used for output.

        \param resource The resource to unmark.
     */
    void unmark_pending_output(gfx_shader_resource *resource);
    /*!
        Unmarks a resource to no longer be used for input.

        \param resource The resource to unmark.
     */
    void unmark_pending_input(gfx_shader_resource *resource);
    //! \overload
    void unmark_pending_output(gfx_shader_resource *resource, uint32_t stageIndex, uint32_t slot);
    //! \overload
    void unmark_pending_input(gfx_shader_resource *resource, uint32_t stageIndex, uint32_t slot);
    /*!
        Marks a resource for input.

        \param resource The resource to mark.
        \param stageIndex The stage index.
        \param slot The slot index.
     */
    void mark_pending_input(gfx_shader_resource *resource, uint32_t stageIndex, uint32_t slot);
    /*!
        Marks a resource for output.

        \param resource The resource to mark.
        \param stageIndex The stage index.
        \param slot The slot index.
     */
    void mark_pending_output(gfx_shader_resource *resource, uint32_t stageIndex, uint32_t slot);
    //! Updates the output merger to reflect RTV / UAV changes.
    void update_output_merger_counts();
    //! Initializes the pipeline.
    void setup_pipeline();
    //! \overload
    void bind_depth_stencil_buffer(GO_INTERNAL gfx_output_merger_stage &stage, gfx_depth_stencil_buffer *depthStencilBuffer, uint32_t index);
    //! \overload
    void bind_render_target(GO_INTERNAL gfx_output_merger_stage &stage, uint32_t slot, gfx_render_target *renderTarget, uint32_t index);
    //! \overload
    void bind_output_merger_uav(GO_INTERNAL gfx_output_merger_stage &stage, uint32_t slot, gfx_shader_resource *uav, uint32_t index, uint32_t initialCount = 0);
    //! \overload
    void bind_compute_shader_uav(GO_INTERNAL gfx_compute_shader_stage &stage, uint32_t slot, gfx_shader_resource *uav, uint32_t index, uint32_t initialCount = 0);
    /*!
        Resolves all Input -> Output hazards.

        \param unbindInputs Input unbind masks.
        \param unbindOutputs Output unbind masks.
     */
    void resolve_input_hazards(decltype(GO gfx_shader_resource::internalInputBindMask) &unbindInputs,
        decltype(GO gfx_shader_resource::internalOutputBindMask) &unbindOutputs);
    /*!
        Resolves all Output -> X hazards.

        \param unbindInputs Input unbind masks.
        \param unbindOutputs Output unbind masks.
     */
    void resolve_output_hazards(decltype(GO gfx_shader_resource::internalInputBindMask) &unbindInputs,
        decltype(GO gfx_shader_resource::internalOutputBindMask) &unbindOutputs);
    /*!
        Unbinds all hazard-tagged resources.

        \param unbindInputs Input unbind masks.
        \param unbindOutputs Output unbind masks.
     */
    void unbind_resource_hazards(decltype(GO gfx_shader_resource::internalInputBindMask) &unbindInputs,
        decltype(GO gfx_shader_resource::internalOutputBindMask) &unbindOutputs);
    /*!
        Unbinds a group of SRVs from a stage.

        \param stageIndex The stage index.
        \param groupStart The first group index.
        \param groupSize The group size.
     */
    void unbind_srvs(uint32_t stageIndex, size_t groupStart, size_t groupSize);
    /*!
        Unbinds a group of UAVs from the compute-shader stage.

        \param groupStart The first group index.
        \param groupSize The group size.
     */
    void unbind_compute_shader_uavs(size_t groupStart, size_t groupSize);
    /*!
        Binds a SRV to a stage.

        \param stageIndex The stage index.
        \param resource The resource to bind.
        \param index The resource sub-index.
     */
    void bind_shader_resource(uint32_t stageIndex, uint32_t slot, gfx_shader_resource *resource, uint32_t index);
private:
    typedef std::vector<uint32_t> index_vector_t;
private:
    //! The current input assembler stage.
    GO_INTERNAL gfx_input_assembler_stage m_currentPipelineIA;
    //! The target input assembler stage.
    GO_INTERNAL gfx_input_assembler_stage m_desiredPipelineIA;
    //! The current vertex-shader pipeline stage.
    GO_INTERNAL gfx_vertex_shader_stage m_currentPipelineVS;
    //! The desired vertex-shader pipeline stage.
    GO_INTERNAL gfx_vertex_shader_stage m_desiredPipelineVS;
    //! The current pixel-shader pipeline stage.
    GO_INTERNAL gfx_pixel_shader_stage m_currentPipelinePS;
    //! The desired pixel-shader pipeline stage.
    GO_INTERNAL gfx_pixel_shader_stage m_desiredPipelinePS;
    //! The current geometry-shader pipeline stage.
    GO_INTERNAL gfx_pixel_shader_stage m_currentPipelineGS;
    //! The desired geometry-shader pipeline stage.
    GO_INTERNAL gfx_geometry_shader_stage m_desiredPipelineGS;
    //! The current compute-shader pipeline stage.
    GO_INTERNAL gfx_compute_shader_stage m_currentPipelineCS;
    //! The desired compute-shader pipeline stage.
    GO_INTERNAL gfx_compute_shader_stage m_desiredPipelineCS;
    //! The current output-merger pipeline stage.
    GO_INTERNAL gfx_output_merger_stage m_currentPipelineOM;
    //! The desired output-merger pipeline stage.
    GO_INTERNAL gfx_output_merger_stage m_desiredPipelineOM;
    //! The current rasterizer pipeline stage.
    GO_INTERNAL gfx_rasterizer_stage m_currentPipelineRS;
    //! The desired rasterizer pipeline stage.
    GO_INTERNAL gfx_rasterizer_stage m_desiredPipelineRS;
    //! Helper array of stage pointers.
    std::array<GO_INTERNAL gfx_shader_stage *, 4> m_currentCommonStages;
    //! Helper array of stage pointers.
    std::array<GO_INTERNAL gfx_shader_stage *, 4> m_desiredCommonStages;
    //! The render path.
    std::unique_ptr<gfx_render_path> m_renderPath;
    //! The frame counter.
    uint32_t m_frameCounter;
};

GO_END_NAMESPACE