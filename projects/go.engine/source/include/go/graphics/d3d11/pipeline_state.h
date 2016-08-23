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
    \file graphics/d3d11/pipeline_state.h
    \brief Contains a class which represents the state of a pipeline stage.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/bitmask.h"
#include "go/graphics/d3d11/shader.h"
#include "go/graphics/d3d11/buffer.h"
#include "go/graphics/d3d11/resource.h"
#include "go/graphics/d3d11/texture.h"
#include "go/graphics/d3d11/sampler_state.h"


GO_BEGIN_INTERNAL_NAMESPACE

// Indices into stage-bitmask arrays
// Input
//! The vertex shader SRV stage.
const uint32_t kGPUPipelineStageVS_SRV = 0;
//! The pixel shader SRV stage.
const uint32_t kGPUPipelineStagePS_SRV = 1;
//! The compute shader SRV stage.
const uint32_t kGPUPipelineStageCS_SRV = 2;
//! The geometry shader SRV stage.
const uint32_t kGPUPipelineStageGS_SRV = 3;

// Output
//! The output-merger shader RTV stage.
const uint32_t kGPUPipelineStageOM_RTV = 0;
//! The output-merger shader UAV stage.
const uint32_t kGPUPipelineStageOM_UAV = 1;
//! The computer-shader UAV stage.
const uint32_t kGPUPipelineStageCS_UAV = 2;


/*!
    A class, which represents the state of a pipeline stage.

    \tparam SlotCount The total number of slots in this state.
    \tparam SlotArgs The bind-arguments to forward.
 */
template<size_t SlotCount, typename... SlotArgs>
class gfx_pipeline_state
{
public:
    typedef std::tuple<SlotArgs...> data_tuple_t;
    typedef typename std::tuple_element<0, data_tuple_t>::type first_data_t;
    typedef data_tuple_t *data_t;
    typedef void(*bind_function_t)(size_t, size_t, data_t [2], void *);
    typedef bool(*equality_cmp_function_t)(const data_tuple_t &, const data_tuple_t &);
    //! The number of slots in the state.
    static const size_t kSlotCount = SlotCount;
public:
    //! Constructor.
    gfx_pipeline_state();
public:
    /*!
        Element-access operator.

        \param index The index of the slot to access.

        \return The slot value.
     */
    data_tuple_t &operator[](size_t index);
public:
    /*!
        Binds a slot.

        \param slot The slot to bind.
        \param args The data to bind.

        \return The old element at the slot.
     */
	auto bind_slot(uint32_t slot, SlotArgs... args) ->first_data_t;
    /*!
        Unbinds a set of slots using an unbind-mask.

        This function will not mark the selected slots as modified.

        \param resource The resource to unbind.
        \param unbindMask The unbind-mask.
     */
    void unbind_by_mask(const first_data_t resource, const bitmask<SlotCount> &unbindMask);
    /*!
        Binds all changes and resets the modification bits.

        \param other The other state to compare the changes to.
        \param bindFunc A function, which is invoked on a per-group basis to bind the state change.
        \param Element comparison function.
     */
    void bind_changes(gfx_pipeline_state<SlotCount, SlotArgs...> &other, void *userData, bind_function_t bindFunc, equality_cmp_function_t equalityCmpFunc);
    /*!
        Returns a pointer to the data slots.

        \return A pointer to the data slots.
     */
    data_tuple_t *slots() noexcept;
    /*!
        Returns the modified entry mask.

        \return The modified entry mask.
     */
    bitmask<SlotCount> &modified_mask() noexcept;
    //! Resets the state.
    void reset();
private:
    //! A bitmask which indicates possible modified entries in the slot vector.
    bitmask<SlotCount> m_modifiedEntries;
    //! Per-slot values.
    std::array<data_tuple_t, SlotCount> m_slotValues;
};


// Object-group states
typedef gfx_pipeline_state<D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT, gfx_shader_resource *, uint32_t> gfx_srv_pipeline_state;
typedef gfx_pipeline_state<D3D11_PS_CS_UAV_REGISTER_COUNT, gfx_shader_resource *, uint32_t, uint32_t> gfx_uav_pipeline_state;
typedef gfx_pipeline_state<D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, gfx_sampler_state *> gfx_sampler_pipeline_state;
typedef gfx_pipeline_state<D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, gfx_constant_buffer *> gfx_constants_pipeline_state;
typedef gfx_pipeline_state<D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, gfx_vertex_buffer *> gfx_vertexbuffer_pipeline_state;


typedef void(*gfx_shader_binder)(ID3D11DeviceChild *);
typedef void(*gfx_constant_buffer_binder)(uint32_t, uint32_t, ID3D11Buffer **);
typedef void(*gfx_srv_binder)(uint32_t, uint32_t, ID3D11ShaderResourceView **);
typedef void(*gfx_uav_binder)(uint32_t, uint32_t, ID3D11UnorderedAccessView **, uint32_t *, uint32_t *);
typedef void(*gfx_sampler_state_binder)(uint32_t, uint32_t, ID3D11SamplerState **);


//! A structure, which represents a common shader stage.
struct gfx_shader_stage
{
    /*!
        Constructor.

        \param srvStageIndex The SRV-stage index.
     */
    gfx_shader_stage(uint32_t srvStageIndex);
    //! Destructor.
    virtual ~gfx_shader_stage() {}
    //! Resets the stage.
    virtual void reset();
    /*!
        Binds the desired stage changes.

        \param currentStage The current stage.
     */
    virtual void bind(gfx_shader_stage &currentStage);
    //! The SRV stage index.
    uint32_t srvStageIndex;
    //! The shader pointer.
    const gfx_shader *shader;
    //! Callback function to bind a shader.
    gfx_shader_binder shaderBinder;
    //! The constant-buffer states.
    gfx_constants_pipeline_state constantBuffers;
    //! Callback function to bind a constant-buffer group.
    gfx_constant_buffer_binder constantBufferBinder;
    //! The shader-resource states.
    gfx_srv_pipeline_state shaderResources;
    //! Callback function to bind a shader-resource group.
    gfx_srv_binder shaderResourceBinder;
    //! The sampler states.
    gfx_sampler_pipeline_state samplerStates;
    //! The sampler state binder.
    gfx_sampler_state_binder samplerStateBinder;
};


//! A structure, which represents the vertex-shader stage.
struct gfx_vertex_shader_stage : public gfx_shader_stage
{
    //! Constructor.
    gfx_vertex_shader_stage();
};


//! A structure, which represents the pixel-shader stage.
struct gfx_pixel_shader_stage : public gfx_shader_stage
{
    //! Constructor.
    gfx_pixel_shader_stage();
};


//! A structure, which represents the geometry-shader stage.
struct gfx_geometry_shader_stage : public gfx_shader_stage
{
    //! Constructor.
    gfx_geometry_shader_stage();
};


//! A structure, which represents the compute-shader stage.
struct gfx_compute_shader_stage : public gfx_shader_stage
{
    //! Constructor.
    gfx_compute_shader_stage();
    void reset() override;
    //! Binds a set of UAVs to the pipeline.
    void bind_uavs(gfx_shader_stage &currentStage);
    //! The unordered-access-view states.
    gfx_uav_pipeline_state uavs;
};


//! A structure, which represents the input-assembler stage.
struct gfx_input_assembler_stage
{
    //! Constructor.
    gfx_input_assembler_stage();
    //! Resets the stage to a default state.
    void reset();
    /*!
        Binds the desired stage changes.

        \param currentStage The current stage.
     */
    void bind(gfx_input_assembler_stage &currentStage);
    //! The vertex-buffer states.
    gfx_vertexbuffer_pipeline_state vertexBuffers;
    //! The index-buffer pointer.
    ID3D11Buffer *indexBuffer;
    //! The index-buffer offset.
    uint32_t startIndex;
    //! The size of an index.
    uint32_t indexSize;
    //! The primitive topology.
    D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
};


//! A structure, which represents the rasterizer stage.
struct gfx_rasterizer_stage
{
    //! Constructor.
    gfx_rasterizer_stage();
    //! Resets the stage values.
    void reset();
    /*!
        Binds the desired stage changes.

        \param currentStage The current stage.
     */
    void bind(gfx_rasterizer_stage &currentStage);
    //! Rasterizer-state.
    std::array<D3D11_VIEWPORT, 6> viewports;
    //! The number of active viewports.
    uint32_t numViewports;
    //! The rasterizer state.
    ID3D11RasterizerState *rasterizerState;
};


//! A structure, which represents the output-merger stage.
struct gfx_output_merger_stage
{
    //! Constructor.
    gfx_output_merger_stage();
    //! Resets the stage values.
    void reset();
    /*!
        Binds the desired stage changes.

        \param currentStage The current stage.
     */
    void bind(gfx_output_merger_stage &currentStage);
    //! RTV-bindings.
    std::array<gfx_render_target *, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> rtvs;
    //! D3D-RTV cache.
    std::array<ID3D11RenderTargetView *, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> d3dRTVs;
    //! The number of bound render-targets.
    uint32_t numRTVs;
    //! UAV-bindings.
    std::array<gfx_shader_resource *, D3D11_PS_CS_UAV_REGISTER_COUNT> uavs;
    //! D3D-UAV cache.
    std::array<ID3D11UnorderedAccessView *, D3D11_PS_CS_UAV_REGISTER_COUNT> d3dUAVs;
    //! UAV initial counts.
    std::array<uint32_t, D3D11_PS_CS_UAV_REGISTER_COUNT> uavInitialCount;
    //! Number of UAVs.
    uint32_t numUAVs;
    //! The depth-stencil buffer.
    gfx_depth_stencil_buffer *depthStencilBuffer;
    //! The DSV-index.
    uint32_t depthStencilIndex;
    //! The blend-state.
    ID3D11BlendState *blendState;
    //! The blend factors.
    std::array<float, 4> blendFactor;
    //! The depth-stencil state.
    ID3D11DepthStencilState *depthStencilState;
    //! The stencil-reference value.
    uint32_t stencilRef;
    //! Dirty-flag.
    uint32_t forceRTVRebind;
};


#include "pipeline_state_impl.h"

GO_END_INTERNAL_NAMESPACE