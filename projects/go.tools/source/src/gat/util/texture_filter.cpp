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

#include "gat/util/texture_filter.h"


#include "gat/util/exception.h"
#include "gat/util/cmd_help.h"
#include "gat/util/d3d11_helper.h"

#include "go/golib.h"
#include "DDSTextureLoader.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"


using namespace DirectX;


//===========================================================================
// gat::texture_filter
//===========================================================================

void gat::texture_filter::specular_roughness_anti_aliasing(const go::file_path &roughnessTexFilePath,
    const go::file_path &normalTexFilePath, const go::file_path &roughnessOutTexFilePath)
{
    _tprintf(_T("Filtering roughness texture '%s'...\n"), roughnessTexFilePath.c_str());

    auto inFilePathR = to_wstring(roughnessTexFilePath.c_str());
    auto inFilePathN = to_wstring(normalTexFilePath.c_str());
    auto outFilePath = to_wstring(roughnessOutTexFilePath.c_str());

    // Startup D3D
    Microsoft::WRL::ComPtr<ID3D11Device> d3dDev;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;
    create_d3d11_device(d3dDev, d3dContext);

    // Load textures
    Microsoft::WRL::ComPtr<ID3D11Resource> normalRes;
    Microsoft::WRL::ComPtr<ID3D11Resource> roughnessRes;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalSRV;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughnessSRV;

    THROW_COM_ERROR(CreateDDSTextureFromFile(d3dDev.Get(), inFilePathR.c_str(), &roughnessRes, &roughnessSRV));
    THROW_COM_ERROR(CreateDDSTextureFromFile(d3dDev.Get(), inFilePathN.c_str(), &normalRes, &normalSRV));

    // Sizes must match
    Microsoft::WRL::ComPtr<ID3D11Texture2D> normalTex;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> roughnessTex;
    THROW_COM_ERROR(normalRes.As<ID3D11Texture2D>(&normalTex));
    THROW_COM_ERROR(roughnessRes.As<ID3D11Texture2D>(&roughnessTex));

    D3D11_TEXTURE2D_DESC normalDesc;
    D3D11_TEXTURE2D_DESC roughnessDesc;
    normalTex->GetDesc(&normalDesc);
    roughnessTex->GetDesc(&roughnessDesc);

    if(normalDesc.Width != roughnessDesc.Width || normalDesc.Height != roughnessDesc.Height)
    {
        throw tchar_exception(_T("Normal and Roughness map have to match in size."));
    }

    // Load the shader
    auto csFile = go::binary_file::read(_T(".\\gat_pbr_roughness_mapper.cso"));
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> cs;
    THROW_COM_ERROR(d3dDev->CreateComputeShader(csFile.data(), csFile.size(), nullptr, &cs));

    // Bind textures
    ID3D11ShaderResourceView *srvs[] = { roughnessSRV.Get(), normalSRV.Get() };
    d3dContext->CSSetShaderResources(0, ARRAYSIZE(srvs), srvs);

    // Bind output
    D3D11_TEXTURE2D_DESC outDesc;
    outDesc = roughnessDesc;
    outDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> outTex;
    THROW_COM_ERROR(d3dDev->CreateTexture2D(&outDesc, nullptr, &outTex));

    std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> outUAVs;
    outUAVs.resize(outDesc.MipLevels);

    CD3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
    uavDesc.Format = outDesc.Format;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    for(UINT i = 0; i < outDesc.MipLevels; ++i)
    {
        uavDesc.Texture2D.MipSlice = i;
        THROW_COM_ERROR(d3dDev->CreateUnorderedAccessView(outTex.Get(), &uavDesc, &outUAVs[i]));
    }

    // Create constant buffer
    struct constants_t
    {
        UINT mipLevel;
        UINT _padding[3];
    };

    CD3D11_BUFFER_DESC cbDesc(sizeof(constants_t), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
    Microsoft::WRL::ComPtr<ID3D11Buffer> cb;
    THROW_COM_ERROR(d3dDev->CreateBuffer(&cbDesc, nullptr, &cb));
    d3dContext->CSSetConstantBuffers(0, 1, cb.GetAddressOf());

    // Execute compute shader
    d3dContext->CSSetShader(cs.Get(), nullptr, 0);

    D3D11_MAPPED_SUBRESOURCE mappedRes;
    for(UINT i = 0; i < roughnessDesc.MipLevels; ++i)
    {
        // Update mip index
        THROW_COM_ERROR(d3dContext->Map(cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes));
        memcpy(mappedRes.pData, &i, sizeof(i));
        d3dContext->Unmap(cb.Get(), 0);

        // Bind UAV
        d3dContext->CSSetUnorderedAccessViews(0, 1, outUAVs[i].GetAddressOf(), nullptr);

        UINT x = roughnessDesc.Width >> i;
        UINT y = roughnessDesc.Height >> i;
        d3dContext->Dispatch(x, y, 1);
    }

    DirectX::ScratchImage img;
    CaptureTexture(d3dDev.Get(), d3dContext.Get(), outTex.Get(), img);

    SaveToDDSFile(img.GetImages(), img.GetImageCount(), img.GetMetadata(), DDS_FLAGS_NONE, outFilePath.c_str());
}