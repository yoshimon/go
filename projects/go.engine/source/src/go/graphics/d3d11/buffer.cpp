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


#include "go/graphics/d3d11/buffer.h"


#include "go/core/log.h"
#include "go/graphics/d3d11/display.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

static inline void *d3d_map_throw(ID3D11Buffer *buffer, D3D11_MAP mode)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    GO_THROW_HRESULT
    (
        GO_INTERNAL g_d3d11ImmediateContext->Map(buffer, 0, mode, 0, &mappedResource),
        "could not map buffer"
    );
    return mappedResource.pData;
}


static D3D11_BUFFER_DESC d3d_buffer_desc(uint32_t byteWidth)
{
    D3D11_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = byteWidth;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    return cbDesc;
}


static void override_update_frequency(D3D11_BUFFER_DESC &bd, go::gfx_buffer_update_frequency updateFrequency,
    void *initialData)
{
    if(updateFrequency == go::gfx_buffer_update_frequency ::never)
    {
        // Overwrite flags
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.CPUAccessFlags = 0;

        // We should have some initial data
        GO_ASSERT(initialData);
    }
}


static void d3d_create_buffer(D3D11_BUFFER_DESC &bd, go::gfx_buffer_update_frequency updateFrequency,
    void *initialData, ID3D11Buffer **buffer)
{
    override_update_frequency(bd, updateFrequency, initialData);

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = initialData;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    // Create the buffer.
    GO_THROW_HRESULT
    (
        GO_INTERNAL g_d3d11Device->CreateBuffer(&bd, initialData ? &initData : nullptr, buffer),
        "could not create D3D buffer object"
    );
}


static void d3d_create_staging_buffer(D3D11_BUFFER_DESC &bd, go::gfx_buffer_update_frequency updateFrequency, ID3D11Buffer **buffer)
{
    switch(updateFrequency)
    {
    case go::gfx_buffer_update_frequency::high:
    case go::gfx_buffer_update_frequency::unknown:
        bd.Usage = D3D11_USAGE_DYNAMIC;
        break;
    case go::gfx_buffer_update_frequency::low:
        // Create the staging buffer
        bd.Usage = D3D11_USAGE_STAGING;
        GO_THROW_HRESULT
        (
            GO_INTERNAL g_d3d11Device->CreateBuffer(&bd, nullptr, buffer),
            "could not create D3D staging-buffer"
        );
        break;
    }
}


// ================================================================================ //
// go::gfx_buffer member functions
// ================================================================================ //

void go::gfx_buffer::release()
{
    m_d3dBuffer.Reset();
}


void go::gfx_buffer::update(const void *data, uint32_t dataSize)
{
    auto dst = begin_update();
    memcpy(dst, data, dataSize);
    end_update();
}


// ================================================================================ //
// go::gfx_constant_buffer member functions
// ================================================================================ //

go::gfx_constant_buffer::gfx_constant_buffer(uint32_t size, gfx_buffer_update_frequency updateFrequency, void *initialData)
{
    reset(size, updateFrequency, initialData);
}


void *go::gfx_constant_buffer::begin_update()
{
    return d3d_map_throw(m_d3dBuffer.Get(), D3D11_MAP_WRITE_DISCARD);
}


void go::gfx_constant_buffer::end_update()
{
    GO_INTERNAL g_d3d11ImmediateContext->Unmap(m_d3dBuffer.Get(), 0);
}


void go::gfx_constant_buffer::reset(uint32_t size, gfx_buffer_update_frequency updateFrequency, void *initialData)
{
    GO_ASSERT(size % 16 == 0);
    CD3D11_BUFFER_DESC bd(size, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
    d3d_create_buffer(bd, updateFrequency, initialData, &m_d3dBuffer);
    
    m_size = size;
}


// ================================================================================ //
// go::gfx_stageable_buffer member functions
// ================================================================================ //

void go::gfx_stageable_buffer::release()
{
    gfx_buffer::release();
    m_stagingBuffer.Reset();
}


go::gfx_stageable_buffer::gfx_stageable_buffer(
    gfx_stageable_buffer_type type,
    uint32_t elementCount, uint32_t elementSize, 
    gfx_buffer_update_frequency updateFrequency,
    void *initialData)
    : m_type(type)
{
    reset(elementCount, elementSize, updateFrequency, initialData);
}


go::gfx_stageable_buffer::gfx_stageable_buffer(gfx_stageable_buffer_type type)
    : m_type(type)
{
}


void *go::gfx_stageable_buffer::begin_update()
{
    return begin_update(gfx_stageable_buffer_update_mode::discard);
}


void *go::gfx_stageable_buffer::begin_update(gfx_stageable_buffer_update_mode updateMode)
{
    if(is_in_set(m_updateFrequency, gfx_buffer_update_frequency::high, gfx_buffer_update_frequency::unknown))
    {
        return d3d_map_throw(m_d3dBuffer.Get(), updateMode == gfx_stageable_buffer_update_mode::discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE);
    }
    else
    {
        return d3d_map_throw(m_stagingBuffer.Get(), D3D11_MAP_WRITE_DISCARD);
    }
}


void go::gfx_stageable_buffer::end_update()
{
    if(is_in_set(m_updateFrequency, gfx_buffer_update_frequency::high, gfx_buffer_update_frequency::unknown))
    {
        GO_INTERNAL g_d3d11ImmediateContext->Unmap(m_d3dBuffer.Get(), 0);
    }
    else
    {
        GO_INTERNAL g_d3d11ImmediateContext->Unmap(m_stagingBuffer.Get(), 0);
        GO_INTERNAL g_d3d11ImmediateContext->CopyResource(m_d3dBuffer.Get(), m_stagingBuffer.Get());
    }
}


void go::gfx_stageable_buffer::reset(
    uint32_t elementCount, uint32_t elementSize,
    gfx_buffer_update_frequency updateFrequency,
    void *initialData)
{
    CD3D11_BUFFER_DESC bd(elementCount * elementSize, m_type == gfx_stageable_buffer_type::vertex ?  D3D11_BIND_VERTEX_BUFFER : D3D11_BIND_INDEX_BUFFER);
    d3d_create_staging_buffer(bd, updateFrequency, &m_stagingBuffer);
    d3d_create_buffer(bd, updateFrequency, initialData, &m_d3dBuffer);

    m_updateFrequency = updateFrequency;
    m_size = elementCount * elementSize;
    m_elementSize = elementSize;
}


// ================================================================================ //
// go::gfx_vertex_buffer member functions
// ================================================================================ //

go::gfx_vertex_buffer::gfx_vertex_buffer(
    uint32_t elementCount, uint32_t elementSize,
    gfx_buffer_update_frequency updateFrequency,
    void *initialData)
    : gfx_stageable_buffer(gfx_stageable_buffer_type::vertex, elementCount, elementSize, updateFrequency, initialData)
{
}


go::gfx_vertex_buffer::gfx_vertex_buffer()
    : gfx_stageable_buffer(gfx_stageable_buffer_type::vertex)
{
}


// ================================================================================ //
// go::gfx_index_buffer member functions
// ================================================================================ //

go::gfx_index_buffer::gfx_index_buffer(
    uint32_t elementCount, uint32_t elementSize,
    gfx_buffer_update_frequency updateFrequency, void *initialData)
    : gfx_stageable_buffer(gfx_stageable_buffer_type::index, elementCount, elementSize, updateFrequency, initialData)
{
}


go::gfx_index_buffer::gfx_index_buffer()
    : gfx_stageable_buffer(gfx_stageable_buffer_type::index)
{
}


// ================================================================================ //
// go::gfx_structured_buffer member functions
// ================================================================================ //

go::gfx_structured_buffer::gfx_structured_buffer(
    int32_t elementCapacity, uint32_t elementSize,
    gfx_access_policy accessPolicy,
    gfx_buffer_update_frequency updateFrequency,
    gfx_structured_buffer_data_type dataType,
    void *initialData)
{
    reset(elementCapacity, elementSize, accessPolicy, updateFrequency, dataType, initialData);
}


void go::gfx_structured_buffer::release()
{
    gfx_buffer::release();
    m_d3dSRVArray.clear();
    m_d3dUAVArray.clear();
    m_d3dStagingBuffer.Reset();
}


void *go::gfx_structured_buffer::begin_update()
{
    if(is_in_set(m_updateFrequency, gfx_buffer_update_frequency::high, gfx_buffer_update_frequency::unknown))
    {
        return d3d_map_throw(m_d3dBuffer.Get(), D3D11_MAP_WRITE_DISCARD);
    }
    else
    {
        return d3d_map_throw(m_d3dStagingBuffer.Get(), D3D11_MAP_WRITE_DISCARD);
    }
}


void go::gfx_structured_buffer::end_update()
{
    if(is_in_set(m_updateFrequency, gfx_buffer_update_frequency::high, gfx_buffer_update_frequency::unknown))
    {
        GO_INTERNAL g_d3d11ImmediateContext->Unmap(m_d3dBuffer.Get(), 0);
    }
    else
    {
        GO_INTERNAL g_d3d11ImmediateContext->Unmap(m_d3dStagingBuffer.Get(), 0);
        GO_INTERNAL g_d3d11ImmediateContext->CopyResource(m_d3dBuffer.Get(), m_d3dStagingBuffer.Get());
    }
}


void go::gfx_structured_buffer::reset(
    int32_t elementCapacity, uint32_t elementSize,
    gfx_access_policy accessPolicy,
    gfx_buffer_update_frequency updateFrequency,
    gfx_structured_buffer_data_type dataType,
    void *initialData)
{
    GO_ASSERT(elementCapacity > 0);
    if(dataType == gfx_structured_buffer_data_type::raw32)
    {
        GO_ASSERT(elementSize == 4);
    }

    auto bindFlags = accessPolicy == gfx_access_policy::read_write ? D3D11_BIND_UNORDERED_ACCESS : 0;
    CD3D11_BUFFER_DESC bd
    (
        elementCapacity * elementSize,
        D3D11_BIND_SHADER_RESOURCE | bindFlags,
        D3D11_USAGE_DEFAULT, // Corrected later (see below)
        accessPolicy == gfx_access_policy::read_only ? D3D11_CPU_ACCESS_WRITE : 0,
        dataType == gfx_structured_buffer_data_type::user ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : 0,
        dataType == gfx_structured_buffer_data_type::user ? elementSize : 0
    );
    bd.MiscFlags |= dataType == gfx_structured_buffer_data_type::raw32 ? D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS : 0;
    d3d_create_staging_buffer(bd, updateFrequency, &m_d3dStagingBuffer);
    if(!m_d3dStagingBuffer && accessPolicy == gfx_access_policy::read_write)
    {
        // Special case for structured buffers
        bd.Usage = D3D11_USAGE_DEFAULT;
    }
    d3d_create_buffer(bd, updateFrequency, initialData, &m_d3dBuffer);

    // Create SRV
    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    memset(&srvd, 0, sizeof(srvd));
    // StructuredBuffer (Buffer<> not allowed)
    switch(dataType)
    {
    case gfx_structured_buffer_data_type::user: srvd.Format = DXGI_FORMAT_UNKNOWN; break;
    case gfx_structured_buffer_data_type::raw32: srvd.Format = DXGI_FORMAT_R32_TYPELESS; break;
    case gfx_structured_buffer_data_type::native: srvd.Format = DXGI_FORMAT_R32_UINT; break;
    }

    if(dataType == gfx_structured_buffer_data_type::raw32)
    {
        srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
        srvd.BufferEx.NumElements = elementCapacity;
        srvd.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
    }
    else
    {
        srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvd.Buffer.ElementWidth = elementSize;
        srvd.Buffer.NumElements = elementCapacity;
    }
    
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    GO_THROW_HRESULT
    (
        GO_INTERNAL g_d3d11Device->CreateShaderResourceView(m_d3dBuffer.Get(), &srvd, &srv),
        "could not create SRV"
    );
    m_d3dSRVArray.clear();
    m_d3dSRVArray.push_back(srv);

    // Create UAV
    if(bindFlags & D3D11_BIND_UNORDERED_ACCESS)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
        memset(&uavd, 0, sizeof(uavd));
        uavd.Buffer.Flags = dataType == gfx_structured_buffer_data_type::raw32 ? D3D11_BUFFER_UAV_FLAG_RAW : 0;
        uavd.Buffer.NumElements = elementCapacity;
        uavd.Format = srvd.Format;
        uavd.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

        Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> d3dUAV;
        GO_THROW_HRESULT
        (
            GO_INTERNAL g_d3d11Device->CreateUnorderedAccessView(m_d3dBuffer.Get(), &uavd, &d3dUAV),
            "could not create UAV"
        );

        m_d3dUAVArray.clear();
        m_d3dUAVArray.push_back(d3dUAV);
    }

    m_elementCapacity = elementCapacity;
    m_updateFrequency = updateFrequency;
}