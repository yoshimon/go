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


#include "go/graphics/d3d11/shader.h"


#include "go/core/log.h"
#include "go/graphics/d3d11/display.h"
#include "go/graphics/d3d11/device.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

Microsoft::WRL::ComPtr<ID3DBlob> compile_shader(const std::string &profile,
                                                const std::string &shaderSource,
                                                const std::string &entryPoint)
{
    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif 
    GO_THROW_HRESULT
    (
        D3DCompile(shaderSource.c_str(), shaderSource.size(), nullptr, nullptr, nullptr,
        entryPoint.c_str(), profile.c_str(), flags, 0, &blob, nullptr),
        "failed to compile GPU shader"
    );

    return blob;
}


static DXGI_FORMAT d3d_register_component_info_to_dxgi_format(D3D_REGISTER_COMPONENT_TYPE componentType, uint32_t componentCount)
{
    switch(componentType)
    {
    case D3D_REGISTER_COMPONENT_FLOAT32:
        switch(componentCount)
        {
        case 1: return DXGI_FORMAT_R32_FLOAT;
        case 2: return DXGI_FORMAT_R32G32_FLOAT;
        case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
        case 4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
        break;
    case D3D_REGISTER_COMPONENT_SINT32:
        switch(componentCount)
        {
        case 1: return DXGI_FORMAT_R32_SINT;
        case 2: return DXGI_FORMAT_R32G32_SINT;
        case 3: return DXGI_FORMAT_R32G32B32_SINT;
        case 4: return DXGI_FORMAT_R32G32B32A32_SINT;
        }
        break;
    case D3D_REGISTER_COMPONENT_UINT32:
        switch(componentCount)
        {
        case 1: return DXGI_FORMAT_R32_UINT;
        case 2: return DXGI_FORMAT_R32G32_UINT;
        case 3: return DXGI_FORMAT_R32G32B32_UINT;
        case 4: return DXGI_FORMAT_R32G32B32A32_UINT;
        }
        break;
    }
    GO_MSGASSERT(false, "unknown D3D register component descriptor");
    return DXGI_FORMAT_UNKNOWN;
}


static int32_t input_component_count(const D3D11_SIGNATURE_PARAMETER_DESC &spd)
{
    auto componentCount = 0;
    for(auto i = 1; i < 0xF; i = i << 1)
    {
        if(spd.Mask & i)
        {
            ++componentCount;
        }
    }

    return componentCount;
}


// ================================================================================ //
// go::gfx_shader member functions
// ================================================================================ //

go::gfx_shader::gfx_shader(gfx_shader_type type, const uint8_t *byteCode, uint32_t numBytes)
{
    initialize(type, byteCode, numBytes);
}


go::gfx_shader::gfx_shader(gfx_shader_type type, const byte_vector &byteCode)
    : gfx_shader(type, byteCode.data(), (uint32_t)byteCode.size())
{
}


void go::gfx_shader::initialize(gfx_shader_type type, const uint8_t *byteCode, uint32_t numBytes)
{
    static const std::string s_profileSuffix = "_5_0";

    HRESULT hr;
    ID3D11DeviceChild *shader;
    switch(type)
    {
    case gfx_shader_type::vertex:
        m_typeString = "vs";
        hr = GO_INTERNAL g_d3d11Device->CreateVertexShader(byteCode, numBytes, nullptr, (ID3D11VertexShader **)&shader);
        break;
    case gfx_shader_type::pixel:
        m_typeString = "ps";
        hr = GO_INTERNAL g_d3d11Device->CreatePixelShader(byteCode, numBytes, nullptr, (ID3D11PixelShader **)&shader);
        break;
    case gfx_shader_type::compute:
        m_typeString = "cs";
        hr = GO_INTERNAL g_d3d11Device->CreateComputeShader(byteCode, numBytes, nullptr, (ID3D11ComputeShader **)&shader);
        break;
    case gfx_shader_type::geometry:
        m_typeString = "gs";
        hr = GO_INTERNAL g_d3d11Device->CreateGeometryShader(byteCode, numBytes, nullptr, (ID3D11GeometryShader **)&shader);
        break;
    default: GO_THROW_HRESULT(E_FAIL, "Unknown shader type.");
    }
    GO_THROW_HRESULT(hr, "Failed to create D3D shader.");
    m_d3dShader.Attach(shader);

    m_type = type;
    m_profile = m_typeString + s_profileSuffix;

    GO_THROW_HRESULT
    (
        D3DReflect(byteCode, (size_t)numBytes, IID_ID3D11ShaderReflection, (void **)&m_reflection),
        "failed to reflect D3D shader"
    );

    // Setup reflection
    D3D11_SHADER_DESC sd;
    m_reflection->GetDesc(&sd);

    // Input Layout
    if(type == gfx_shader_type::vertex)
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
        inputElements.reserve(sd.InputParameters);

        for(auto i = 0U; i < sd.InputParameters; ++i)
        {
            D3D11_SIGNATURE_PARAMETER_DESC spd;
            GO_THROW_HRESULT
            (
                m_reflection->GetInputParameterDesc(i, &spd),
                "failed to query shader input parameter descriptor"
            );

            // Skip system values
            if(strcmp(spd.SemanticName, "SV_InstanceID") == 0)
            {
                continue;
            }

            D3D11_INPUT_ELEMENT_DESC ied;
            ied.SemanticName = spd.SemanticName;
            ied.SemanticIndex = spd.SemanticIndex;
            ied.InputSlot = 0;
            ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            ied.InstanceDataStepRate = 0;
            ied.AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
            ied.Format = d3d_register_component_info_to_dxgi_format(spd.ComponentType, input_component_count(spd));
            inputElements.push_back(std::move(ied));
        }

        GO_THROW_HRESULT
        (
            GO_INTERNAL g_d3d11Device->CreateInputLayout(inputElements.data(), (uint32_t)inputElements.size(), byteCode, numBytes, &m_inputLayout),
            "failed to create D3D input layout from shader"
        );
    }
}


const std::string &go::gfx_shader::profile_string() const noexcept
{
    return m_profile;
}


const std::string &go::gfx_shader::type_string() const noexcept
{
    return m_typeString;
}