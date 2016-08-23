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


#include "go/graphics/d3d11/pipeline_state.h"


#include "go/graphics/d3d11/device.h"

// ================================================================================ //
// Static helper functions
// ================================================================================ //

static inline void replace_output_marks(go::gfx_shader_resource *oldResource, go::gfx_shader_resource *newResource,
    uint32_t stageIndex, uint32_t slot)
{
    if(oldResource)
    {
        oldResource->internalOutputBindMask[stageIndex].clear(slot);
    }

    if(newResource)
    {
        newResource->internalOutputBindMask[stageIndex].set(slot);
        newResource->internalPendingOutputBindMask[stageIndex].clear(slot); // Binding done
    }
}


static inline void replace_input_marks(go::gfx_shader_resource *oldResource, go::gfx_shader_resource *newResource,
    uint32_t stageIndex, uint32_t slot)
{
    if(oldResource)
    {
        oldResource->internalInputBindMask[stageIndex].clear(slot);
    }

    if(newResource)
    {
        newResource->internalInputBindMask[stageIndex].set(slot);
        newResource->internalPendingInputBindMask[stageIndex].clear(slot); // Binding done
    }
}


// ================================================================================ //
// go::internal::gfx_shader_stage member functions
// ================================================================================ //

go::internal::gfx_shader_stage::gfx_shader_stage(uint32_t pSRVStageIndex)
    : srvStageIndex(pSRVStageIndex)
{
    reset();
}


void go::internal::gfx_shader_stage::reset()
{
    // Reset markers
    for(size_t i = 0; i < gfx_srv_pipeline_state::kSlotCount; ++i)
    {
        auto srv = std::get<0>(shaderResources[i]);
        if(srv)
        {
            srv->internalInputBindMask[srvStageIndex].clear_all();
            srv->internalPendingInputBindMask[srvStageIndex].clear_all();
        }
    }

    shader = nullptr;
    constantBuffers.reset();
    shaderResources.reset();
    samplerStates.reset();
}


void go::internal::gfx_shader_stage::bind(gfx_shader_stage &currentStage)
{
    // Shader
    {
        auto oldShader = currentStage.shader;
        auto newShader = shader;

        if(oldShader != newShader)
        {
            currentStage.shader = newShader;
            shaderBinder(newShader ? newShader->d3d_shader() : nullptr);

            // Input Layout changed? (VS only)
            auto oldIL = oldShader ? oldShader->d3d_input_layout() : nullptr;
            auto newIL = newShader ? newShader->d3d_input_layout() : nullptr;
            if(oldIL != newIL)
            {
                GO_INTERNAL g_d3d11ImmediateContext->IASetInputLayout(newIL);
            }
        }
    }

    // ConstantBuffers
    constantBuffers.bind_changes(currentStage.constantBuffers, this,
        [](size_t groupStart, size_t groupSize, gfx_constants_pipeline_state::data_t data[2], void *userData)
    {
        ID3D11Buffer *tmp[gfx_constants_pipeline_state::kSlotCount];

        for(size_t i = groupStart, end = groupStart + groupSize; i < end; ++i)
        {
            auto tNew = std::get<0>(data[0][i]);
            tmp[i] = tNew ? tNew->d3d_buffer() : nullptr;
        }

        ((gfx_shader_stage *)userData)->constantBufferBinder((uint32_t)groupStart, (uint32_t)groupSize, &tmp[groupStart]);
    }, [](const gfx_constants_pipeline_state::data_tuple_t &a, const gfx_constants_pipeline_state::data_tuple_t &b)
    {
        auto aPtr = std::get<0>(a);
        auto bPtr = std::get<0>(b);
        return (aPtr && bPtr) ? aPtr->d3d_buffer() == bPtr->d3d_buffer() : aPtr == bPtr;
    });

    // ShaderResources
    shaderResources.bind_changes(currentStage.shaderResources, this,
        [](size_t groupStart, size_t groupSize, gfx_srv_pipeline_state::data_t data[2], void *userData)
    {
        ID3D11ShaderResourceView *tmp[gfx_srv_pipeline_state::kSlotCount];

        auto stage = ((gfx_shader_stage *)userData);
        auto srvStageIndex = stage->srvStageIndex;

        for(size_t i = groupStart, end = groupStart + groupSize; i < end; ++i)
        {
            auto tNew = std::get<0>(data[0][i]);
            auto tNewIdx = std::get<1>(data[0][i]);
            tmp[i] = tNew ? tNew->d3d_srv(tNewIdx) : nullptr;
            replace_input_marks(std::get<0>(data[1][i]), tNew, srvStageIndex, (uint32_t)i);
        }

       stage->shaderResourceBinder((uint32_t)groupStart, (uint32_t)groupSize, &tmp[groupStart]);
    }, [](const gfx_srv_pipeline_state::data_tuple_t &a, const gfx_srv_pipeline_state::data_tuple_t &b)
    {
        auto aPtr = std::get<0>(a);
        auto bPtr = std::get<0>(b);
        return (aPtr && bPtr) ? aPtr->d3d_srv(std::get<1>(a)) == bPtr->d3d_srv(std::get<1>(b)) : aPtr == bPtr;
    });

    // SamplerStates
    samplerStates.bind_changes(currentStage.samplerStates, this,
        [](size_t groupStart, size_t groupSize, gfx_sampler_pipeline_state::data_t data[2], void *userData)
    {
        ID3D11SamplerState *tmp[gfx_sampler_pipeline_state::kSlotCount];

        for(size_t i = groupStart, end = groupStart + groupSize; i < end; ++i)
        {
            auto tNew = std::get<0>(data[0][i]);
            tmp[i] = tNew ? tNew->d3d_sampler_state() : nullptr;
        }

        ((gfx_shader_stage *)userData)->samplerStateBinder((uint32_t)groupStart, (uint32_t)groupSize, &tmp[groupStart]);
    }, [](const gfx_sampler_pipeline_state::data_tuple_t &a, const gfx_sampler_pipeline_state::data_tuple_t &b)
    {
        auto aPtr = std::get<0>(a);
        auto bPtr = std::get<0>(b);
        return (aPtr && bPtr) ? aPtr->d3d_sampler_state() == bPtr->d3d_sampler_state() : aPtr == bPtr;
    });
}


// ================================================================================ //
// go::internal::gfx_compute_shader_stage member functions
// ================================================================================ //

go::internal::gfx_vertex_shader_stage::gfx_vertex_shader_stage()
    : gfx_shader_stage(kGPUPipelineStageVS_SRV)
{
    shaderBinder = [](ID3D11DeviceChild *data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->VSSetShader((ID3D11VertexShader *)data, nullptr, 0);
    };

    constantBufferBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11Buffer **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->VSSetConstantBuffers(groupStart, groupSize, data);
    };

    samplerStateBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11SamplerState **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->VSSetSamplers(groupStart, groupSize, data);
    };

    shaderResourceBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11ShaderResourceView **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->VSSetShaderResources(groupStart, groupSize, data);
    };
}


// ================================================================================ //
// go::internal::gfx_pixel_shader_stage member functions
// ================================================================================ //

go::internal::gfx_pixel_shader_stage::gfx_pixel_shader_stage()
    : gfx_shader_stage(kGPUPipelineStagePS_SRV)
{
    shaderBinder = [](ID3D11DeviceChild *data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->PSSetShader((ID3D11PixelShader *)data, nullptr, 0);
    };

    constantBufferBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11Buffer **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->PSSetConstantBuffers(groupStart, groupSize, data);
    };

    samplerStateBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11SamplerState **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->PSSetSamplers(groupStart, groupSize, data);
    };

    shaderResourceBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11ShaderResourceView **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->PSSetShaderResources(groupStart, groupSize, data);
    };
}


// ================================================================================ //
// go::internal::gfx_geometry_shader_stage member functions
// ================================================================================ //

go::internal::gfx_geometry_shader_stage::gfx_geometry_shader_stage()
    : gfx_shader_stage(kGPUPipelineStageGS_SRV)
{
    shaderBinder = [](ID3D11DeviceChild *data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->GSSetShader((ID3D11GeometryShader *)data, nullptr, 0);
    };

    constantBufferBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11Buffer **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->GSSetConstantBuffers(groupStart, groupSize, data);
    };

    samplerStateBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11SamplerState **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->GSSetSamplers(groupStart, groupSize, data);
    };

    shaderResourceBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11ShaderResourceView **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->GSSetShaderResources(groupStart, groupSize, data);
    };
}


// ================================================================================ //
// go::internal::gfx_compute_shader_stage member functions
// ================================================================================ //

go::internal::gfx_compute_shader_stage::gfx_compute_shader_stage()
    : gfx_shader_stage(kGPUPipelineStageCS_SRV)
{
    shaderBinder = [](ID3D11DeviceChild *data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->CSSetShader((ID3D11ComputeShader *)data, nullptr, 0);
    };

    constantBufferBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11Buffer **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->CSSetConstantBuffers(groupStart, groupSize, data);
    };

    samplerStateBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11SamplerState **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->CSSetSamplers(groupStart, groupSize, data);
    };

    shaderResourceBinder = [](uint32_t groupStart, uint32_t groupSize, ID3D11ShaderResourceView **data)
    {
        GO_INTERNAL g_d3d11ImmediateContext->CSSetShaderResources(groupStart, groupSize, data);
    };
}


void go::internal::gfx_compute_shader_stage::reset()
{
    gfx_shader_stage::reset();

    // Reset markers
    for(size_t i = 0; i < gfx_uav_pipeline_state::kSlotCount; ++i)
    {
        auto uav = std::get<0>(uavs[i]);
        if(uav)
        {
            uav->internalOutputBindMask[GO_INTERNAL kGPUPipelineStageCS_UAV].clear_all();
            uav->internalPendingOutputBindMask[GO_INTERNAL kGPUPipelineStageCS_UAV].clear_all();
        }
    }

    uavs.reset();
}


void go::internal::gfx_compute_shader_stage::bind_uavs(gfx_shader_stage &currentStage)
{
    // UAVs
    uavs.bind_changes(((gfx_compute_shader_stage &)currentStage).uavs, this,
        [](size_t groupStart, size_t groupSize, gfx_uav_pipeline_state::data_t data[2], void *userData)
    {
        ID3D11UnorderedAccessView *tmp[gfx_uav_pipeline_state::kSlotCount];
        uint32_t tmpInitialCounts[gfx_uav_pipeline_state::kSlotCount];

        for(size_t i = groupStart, end = groupStart + groupSize; i < end; ++i)
        {
            auto tNew = std::get<0>(data[0][i]);
            auto tNewIdx = std::get<1>(data[0][i]);
            tmp[i] = tNew ? tNew->d3d_uav(tNewIdx) : nullptr;
            tmpInitialCounts[i] = std::get<2>(data[0][i]);
            replace_output_marks(std::get<0>(data[1][i]), tNew, kGPUPipelineStageCS_UAV, (uint32_t)i);
        }

        GO_INTERNAL g_d3d11ImmediateContext->CSSetUnorderedAccessViews((uint32_t)groupStart, (uint32_t)groupSize, &tmp[groupStart], &tmpInitialCounts[groupStart]);
    }, [](const gfx_uav_pipeline_state::data_tuple_t &a, const gfx_uav_pipeline_state::data_tuple_t &b)
    {
        auto aPtr = std::get<0>(a);
        auto bPtr = std::get<0>(b);
        return (aPtr && bPtr) ? aPtr->d3d_uav(std::get<1>(a)) == bPtr->d3d_uav(std::get<1>(b)) : aPtr == bPtr;
    });
}


// ================================================================================ //
// go::internal::gfx_input_assembler_stage member functions
// ================================================================================ //

go::internal::gfx_input_assembler_stage::gfx_input_assembler_stage()
{
    reset();
}


void go::internal::gfx_input_assembler_stage::reset()
{
    indexBuffer = nullptr;
    startIndex = 0;
    indexSize = sizeof(uint32_t);
    primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    vertexBuffers.reset();
}


void go::internal::gfx_input_assembler_stage::bind(gfx_input_assembler_stage &currentStage)
{
    // PrimitiveTopology
    if(currentStage.primitiveTopology != primitiveTopology)
    {
        currentStage.primitiveTopology = primitiveTopology;
        GO_INTERNAL g_d3d11ImmediateContext->IASetPrimitiveTopology(primitiveTopology);
    }

    // IndexBuffer
    if(currentStage.indexBuffer != indexBuffer || currentStage.startIndex != startIndex || currentStage.indexSize != indexSize)
    {
        currentStage.indexBuffer = indexBuffer;
        currentStage.startIndex = startIndex;
        currentStage.indexSize = indexSize;
        GO_INTERNAL g_d3d11ImmediateContext->IASetIndexBuffer(indexBuffer, indexSize == sizeof(uint32_t) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, startIndex);
    }

    // VertexBuffers
    vertexBuffers.bind_changes(currentStage.vertexBuffers, this,
        [](size_t groupStart, size_t groupSize, gfx_vertexbuffer_pipeline_state::data_t data[2], void *userData)
    {
        ID3D11Buffer *tmp[gfx_vertexbuffer_pipeline_state::kSlotCount];
        uint32_t tmpStrides[gfx_vertexbuffer_pipeline_state::kSlotCount];
        uint32_t tmpOffsets[gfx_vertexbuffer_pipeline_state::kSlotCount];

        for(size_t i = groupStart, end = groupStart + groupSize; i < end; ++i)
        {
            auto tNew = std::get<0>(data[0][i]);
            if(tNew)
            {
                tmp[i] = tNew->d3d_buffer();
                tmpStrides[i] = tNew->element_size();
                tmpOffsets[i] = 0;
            }
            else
            {
                tmp[i] = nullptr;
            }
        }

        GO_INTERNAL g_d3d11ImmediateContext->IASetVertexBuffers((uint32_t)groupStart, (uint32_t)groupSize, &tmp[groupStart], &tmpStrides[groupStart], &tmpOffsets[groupStart]);
    }, [](const gfx_vertexbuffer_pipeline_state::data_tuple_t &a, const gfx_vertexbuffer_pipeline_state::data_tuple_t &b)
    {
        auto aPtr = std::get<0>(a);
        auto bPtr = std::get<0>(b);
        return (aPtr && bPtr) ? aPtr->d3d_buffer() == bPtr->d3d_buffer() : aPtr == bPtr;
    });
}


// ================================================================================ //
// go::internal::gfx_input_assembler_stage member functions
// ================================================================================ //

go::internal::gfx_rasterizer_stage::gfx_rasterizer_stage()
{
    reset();
}


void go::internal::gfx_rasterizer_stage::reset()
{
    rasterizerState = nullptr;
    numViewports = 0;
    viewports.fill(CD3D11_VIEWPORT(0.0f, 0.0f, 1.0f, 1.0f));
}


void go::internal::gfx_rasterizer_stage::bind(gfx_rasterizer_stage &currentStage)
{
    // Viewport
    if((currentStage.numViewports != numViewports) || (currentStage.viewports != viewports))
    {
        currentStage.numViewports = numViewports;
        currentStage.viewports = viewports;
        GO_INTERNAL g_d3d11ImmediateContext->RSSetViewports(numViewports, currentStage.viewports.data());
    }

    // RasterizerState
    if(currentStage.rasterizerState != rasterizerState)
    {
        currentStage.rasterizerState = rasterizerState;
        GO_INTERNAL g_d3d11ImmediateContext->RSSetState(rasterizerState);
    }
}


// ================================================================================ //
// go::internal::gfx_input_assembler_stage member functions
// ================================================================================ //

go::internal::gfx_output_merger_stage::gfx_output_merger_stage()
    : numRTVs(0), numUAVs(0) // Prevent uninitialized unbind in reset
{
    reset();
}


void go::internal::gfx_output_merger_stage::reset()
{
    // Reset markers
    for(size_t i = 0; i < gfx_output_merger_stage::numRTVs; ++i)
    {
        auto rtv = rtvs[i];
        if(rtv)
        {
            rtv->internalOutputBindMask[GO_INTERNAL kGPUPipelineStageOM_RTV].clear_all();
            rtv->internalPendingOutputBindMask[GO_INTERNAL kGPUPipelineStageOM_RTV].clear_all();
        }
    }

    // Reset markers
    for(size_t i = 0; i < gfx_output_merger_stage::numUAVs; ++i)
    {
        auto uav = uavs[i];
        if(uav)
        {
            uav->internalOutputBindMask[GO_INTERNAL kGPUPipelineStageOM_UAV].clear_all();
            uav->internalPendingOutputBindMask[GO_INTERNAL kGPUPipelineStageOM_UAV].clear_all();
        }
    }

    depthStencilBuffer = nullptr;
    depthStencilIndex = 0;
    blendState = nullptr;
    depthStencilState = nullptr;
    stencilRef = 0;
    numRTVs = 0;
    numUAVs = 0;
    forceRTVRebind = false;
    
    rtvs.fill(nullptr);
    d3dRTVs.fill(nullptr);

    uavs.fill(nullptr);
    d3dRTVs.fill(nullptr);
    uavInitialCount.fill(0);

    blendFactor.fill(0.0f);
}


void go::internal::gfx_output_merger_stage::bind(gfx_output_merger_stage &currentStage)
{
    // BlendState
    if(currentStage.blendState != blendState ||
        currentStage.blendFactor != blendFactor)
    {
        currentStage.blendState = blendState;
        currentStage.blendFactor = blendFactor;
        GO_INTERNAL g_d3d11ImmediateContext->OMSetBlendState(blendState, blendFactor.data(), 0xFFFFFFFF);
    }


    // DepthStencilState
    if(currentStage.depthStencilState != depthStencilState || currentStage.stencilRef != stencilRef)
    {
        currentStage.depthStencilState = depthStencilState;
        currentStage.stencilRef = stencilRef;
        GO_INTERNAL g_d3d11ImmediateContext->OMSetDepthStencilState(depthStencilState, stencilRef);
    }

    // RTV + DSV + UAV
    if(forceRTVRebind ||
        currentStage.depthStencilBuffer != depthStencilBuffer||
        currentStage.depthStencilIndex != depthStencilIndex ||
        currentStage.numRTVs != numRTVs ||
        currentStage.numUAVs != numUAVs ||
        currentStage.d3dRTVs != d3dRTVs ||
        currentStage.d3dUAVs != d3dUAVs)
    {
        for(size_t i = 0, end = std::max(numRTVs, currentStage.numRTVs); i < end; ++i)
        {
            replace_output_marks(currentStage.rtvs[i], rtvs[i], GO_INTERNAL kGPUPipelineStageOM_RTV, (uint32_t)i);
        }

        for(size_t i = 0, end = std::max(numUAVs, currentStage.numUAVs); i < end; ++i)
        {
            replace_output_marks(currentStage.uavs[i], uavs[i], GO_INTERNAL kGPUPipelineStageOM_UAV, (uint32_t)i);
        }

        forceRTVRebind = false;

        currentStage.depthStencilBuffer = depthStencilBuffer;
        currentStage.depthStencilIndex = depthStencilIndex;
        currentStage.numRTVs = numRTVs;
        currentStage.numUAVs = numUAVs;

        currentStage.rtvs = rtvs;
        currentStage.d3dRTVs = d3dRTVs;

        currentStage.uavs = uavs;
        currentStage.d3dUAVs = d3dUAVs;
        currentStage.uavInitialCount = uavInitialCount;

        if(numUAVs > 0)
        {
            GO_INTERNAL g_d3d11ImmediateContext->OMSetRenderTargetsAndUnorderedAccessViews(
                numRTVs, d3dRTVs.data(),
                depthStencilBuffer ? depthStencilBuffer->d3d_dsv(depthStencilIndex) : nullptr,
                numRTVs, numUAVs,
                d3dUAVs.data(), uavInitialCount.data());
        }
        else
        {
            GO_INTERNAL g_d3d11ImmediateContext->OMSetRenderTargets(
                numRTVs, d3dRTVs.data(),
                depthStencilBuffer ? depthStencilBuffer->d3d_dsv(depthStencilIndex) : nullptr);
        }
    }
}