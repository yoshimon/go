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

#include "gat/util/d3d11_helper.h"


#include "gat/util/exception.h"

#include "DirectXTex/DDSTextureLoader/DDSTextureLoader.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"


//===========================================================================
// Global functions
//===========================================================================

void gat::create_d3d11_device(Microsoft::WRL::ComPtr<ID3D11Device> &outDevice,
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> &outContext)
{
    _tprintf(_T("Creating D3D11 device.\n"));

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };
    D3D_FEATURE_LEVEL selectedFeatureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device> result;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    uint32_t createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if (defined(DEBUG) | defined(_DEBUG))
    // Enable debug output
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    if(FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
        &outDevice, &selectedFeatureLevel, &outContext)))
    {
        throw tchar_exception(_T("Could not initialize the default D3D11 hardware device."));
    }
}


void gat::convert_tex2d_to_cubemap(const config_file &cfg,
    const std::wstring &sourceFilePath, bool generateMips,
    ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext,
    Microsoft::WRL::ComPtr<ID3D11Texture2D> &outDestCubeTex,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> &outDestCubeSRV,
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> &outDestCubeUAV)
{
    using namespace DirectX;

    // Load textures
    Microsoft::WRL::ComPtr<ID3D11Resource> sourceCubeRes;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sourceCubeSRV;
    THROW_COM_ERROR(CreateDDSTextureFromFile(d3dDevice, sourceFilePath.c_str(), &sourceCubeRes, &sourceCubeSRV));

    // Convert the texture2D format to a cubemap
    _tprintf(_T("Converting 2D texture to cubemap...\n"));

    Microsoft::WRL::ComPtr<ID3D11Texture2D> sourceCubeTex;
    THROW_COM_ERROR(sourceCubeRes.As<ID3D11Texture2D>(&sourceCubeTex));

    D3D11_TEXTURE2D_DESC sourceCubeDesc;
    sourceCubeTex->GetDesc(&sourceCubeDesc);

    auto faceSize = sourceCubeDesc.Width / 6;

    // Make sure the cubemap dimensions are valid
    if(faceSize != sourceCubeDesc.Height)
    {
        throw tchar_exception(_T("The width of the texture must be the height times 6."));
    }

    if(!go::is_power_of_two(sourceCubeDesc.Height))
    {
        throw tchar_exception(_T("The height of the texture must be a power of two."));
    }

    if(faceSize < 32)
    {
        throw tchar_exception(_T("The face size must be at least 32 pixels."));
    }

    // Create the output textures
    D3D11_TEXTURE2D_DESC outDescCubeDesc;
    memset(&outDescCubeDesc, 0, sizeof(outDescCubeDesc));
    outDescCubeDesc.Width = faceSize;
    outDescCubeDesc.Height = faceSize;
    outDescCubeDesc.MipLevels = (UINT)std::log2(faceSize) + 1;
    outDescCubeDesc.ArraySize = 6;
    outDescCubeDesc.SampleDesc.Count = 1;
    outDescCubeDesc.Usage = D3D11_USAGE_DEFAULT;
    outDescCubeDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    outDescCubeDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    outDescCubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
    THROW_COM_ERROR(d3dDevice->CreateTexture2D(&outDescCubeDesc, nullptr, &outDestCubeTex));

    // Create the SRV
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    memset(&srvDesc, 0, sizeof(srvDesc));
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.Format = outDescCubeDesc.Format;
    srvDesc.TextureCube.MipLevels = outDescCubeDesc.MipLevels;
    srvDesc.TextureCube.MostDetailedMip = 0;
    THROW_COM_ERROR(d3dDevice->CreateShaderResourceView(outDestCubeTex.Get(), &srvDesc, &outDestCubeSRV));

    // Create the UAVs
    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
    memset(&uavDesc, 0, sizeof(uavDesc));
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
    uavDesc.Format = outDescCubeDesc.Format;
    uavDesc.Texture2DArray.ArraySize = 6;
    uavDesc.Texture2DArray.FirstArraySlice = 0;
    uavDesc.Texture2DArray.MipSlice = 0;
    THROW_COM_ERROR(d3dDevice->CreateUnorderedAccessView(outDestCubeTex.Get(), &uavDesc, &outDestCubeUAV));

    // Bind the shader
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> shader;
    {
        go::file_path csFilePath(cfg.applicationDirPath + _T("shaders\\gat_pbr_tex2d_to_cube.cso"));
        auto csFile = go::binary_file::read(csFilePath);
        THROW_COM_ERROR(d3dDevice->CreateComputeShader(csFile.data(), csFile.size(), nullptr, &shader));
        d3dContext->CSSetShader(shader.Get(), nullptr, 0);
    }

    // Bind the resources
    d3dContext->CSSetShaderResources(0, 1, sourceCubeSRV.GetAddressOf());
    d3dContext->CSSetUnorderedAccessViews(0, 1, outDestCubeUAV.GetAddressOf(), nullptr);

    {
        const auto kNumThreadsX = 32;
        const auto kNumThreadsY = 8;
        d3dContext->Dispatch(sourceCubeDesc.Width / kNumThreadsX, sourceCubeDesc.Height / kNumThreadsY, 1);
    }

    if(generateMips)
    {
        d3dContext->GenerateMips(outDestCubeSRV.Get());
    }
}


void gat::save_dds_srv(ID3D11ShaderResourceView *d3dSRV, const std::wstring &filePath, DXGI_FORMAT overrideFormat)
{
    using namespace DirectX;

    Microsoft::WRL::ComPtr<ID3D11Resource> d3dRes;
    d3dSRV->GetResource(&d3dRes);
    Microsoft::WRL::ComPtr<ID3D11Device> d3dDev;
    d3dSRV->GetDevice(&d3dDev);
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;
    d3dDev->GetImmediateContext(&d3dContext);

    DirectX::ScratchImage img;
    CaptureTexture(d3dDev.Get(), d3dContext.Get(), d3dRes.Get(), img);

    // TODO: compress the image

    // Save the DDS
    SaveToDDSFile(img.GetImages(), img.GetImageCount(), img.GetMetadata(), DDS_FLAGS_NONE, filePath.c_str());
}