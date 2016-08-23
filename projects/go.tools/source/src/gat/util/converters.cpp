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

#include "gat/util/converters.h"


#include "gat/util/exception.h"
#include "gat/util/cmd_help.h"
#include "gat/util/unity_scene_converter.h"
#include "gat/util/unity_material_set.h"
#include "gat/util/unity_material_set_converter.h"
#include "gat/util/unity_model_converter.h"
#include "gat/util/process_util.h"
#include "gat/util/texture_filter.h"
#include "gat/util/d3d11_helper.h"

#include "DDSTextureLoader.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"



namespace fs = std::experimental::filesystem;


//===========================================================================
// gat::converters
//===========================================================================

void gat::converters::convert_unity_scene(int argc, const _TCHAR *argv[], const config_file &cfg, asset_meta_generators &metaGenerators)
{
    using namespace gat;

    scene_converter_parameters params;

    params.in.sceneFilePath = get_parameter_value(argc, argv, _T("-iscn"), _T(""));

    params.out.sceneFilePath = get_parameter_value(argc, argv, _T("-oscn"), _T(""));
    params.out.assetDirPath = get_parameter_value(argc, argv, _T("-oassets"), _T(""));

    convert_unity_scene(cfg, params, metaGenerators);
}


void gat::converters::convert_unity_scene(const config_file &cfg, const scene_converter_parameters &parameters,
    asset_meta_generators &metaGenerators)
{
    // Build file paths
    auto materialSetFilePath = parameters.in.sceneFilePath.directory_path() + "materials.egs";
    auto outMaterialDirPath = parameters.out.assetDirPath;
    auto outTextureDirPath = parameters.out.assetDirPath;

    // Load the model file
    unity_scene scene(parameters.in.sceneFilePath);
    unity_scene_converter conv(scene);
    conv.save
    (
        cfg,
        parameters.out.sceneFilePath,
        materialSetFilePath,
        parameters.out.assetDirPath,
        metaGenerators
    );
}


void gat::converters::convert_texture(int argc, const _TCHAR *argv[], const gat::config_file &cfg,
    asset_meta_generators &metaGenerators)
{
    using namespace gat;

    texture_converter_parameters params;

    params.in.texFilePath = get_parameter_value(argc, argv, _T("-itex"), _T(""));
    params.in.skipIfExisting = is_parameter_set(argc, argv, _T("-skipIfExisting"));

    params.out.format = get_parameter_value(argc, argv, _T("-dstformat"), _T(""));
    params.out.texFilePath = get_parameter_value(argc, argv, _T("-otex"), _T(""));

    convert_texture(cfg, params, metaGenerators);
}


void gat::converters::convert_texture(const config_file &cfg, texture_converter_parameters &parameters,
    asset_meta_generators &metaGenerators)
{
    tstring sRGBFlag = _T("-srgb "); // TODO: only Albedo should be sRGB but it produces incorrect results...
    auto &outFormat = parameters.out.format;

    // Run texconv
    auto outTexDirPath = parameters.out.texFilePath.directory_path();
    tstring inTexFilePathStr(parameters.in.texFilePath);
    tstring outTexDirPathStr(outTexDirPath);

    // Rebuild the output DDS path that texconv used
    go::file_path outTexFilePath(outTexDirPath, parameters.in.texFilePath.file_name());
    outTexFilePath.replace_extension(_T(".dds"));
    if(!fs::exists(outTexFilePath.c_str()) || !parameters.in.skipIfExisting)
    {
        fs::create_directories(outTexDirPathStr);

        auto cmdLine = sRGBFlag + _T("-nologo -dx10 -pow2 -f ") + outFormat + _T(" -o \"") + outTexDirPathStr + _T("\" \"") + inTexFilePathStr + _T("\"");
        go::file_path texConvFilePath(cfg.applicationDirPath + cfg.query(config_prop_texconv));

        _tprintf(">> TEXCONV: ");

        if(process_util::create_process(texConvFilePath.c_str(), cmdLine.c_str(), false) != 0)
        {
            _tprintf(_T("Could not convert texture '%s'.\n"), texConvFilePath.c_str());
            throw tchar_exception(_T("Failed to convert a texture."));
        }

        // Rename the file to represent the requested filename
        // NOTE: rename it to something else to force a rename to lowercase
        auto tempName = std::string(outTexFilePath) + "_";
        fs::rename(outTexFilePath.c_str(), tempName.c_str());
        fs::rename(tempName.c_str(), parameters.out.texFilePath.c_str());

        // Create the meta data
        saved_data savedData;
        savedData.emplace_back(asset_type::texture, parameters.out.texFilePath);
        metaGenerators.push_back(savedData);
    }
}


void gat::converters::convert_unity_material_set(int argc, const _TCHAR *argv[],
    const config_file &cfg, asset_meta_generators &metaGenerators)
{
    unity_material_converter_parameters params;

    params.in.materialSetFilePath = get_parameter_value(argc, argv, _T("-imtl"), _T(""));
    params.in.assetDirPath = get_parameter_value(argc, argv, _T("-iasset"), _T(""));

    params.out.assetDirPath = get_parameter_value(argc, argv, _T("-oasset"), _T(""));

    convert_unity_material_set(cfg, params, metaGenerators);
}


void gat::converters::convert_unity_material_set(const config_file &cfg,
    const unity_material_converter_parameters &parameters, asset_meta_generators &metaGenerators)
{
    unity_material_set matSet(parameters.in.materialSetFilePath);
    unity_material_set_converter matSetConv(matSet);
    matSetConv.save(cfg, parameters.in.assetDirPath, parameters.out.assetDirPath, metaGenerators);
}


void gat::converters::convert_unity_model(const config_file &cfg,
    const unity_model_converter_parameters &parameters, asset_meta_generators &metaGenerators)
{
    unity_model mdl(parameters.in.modelFilePath);
    unity_model_converter mdlConv(mdl);
    mdlConv.save(cfg, parameters.out.modelFilePath, metaGenerators);
}


void gat::converters::convolve_specular_radiance_map(int argc, const _TCHAR *argv[], const config_file &cfg,
    asset_meta_generators &metaGenerators)
{
    specular_ibl_convolver_parameters params;

    params.in.cubemapFilePath = get_parameter_value(argc, argv, _T("-imap"), _T(""));
    params.in.iterations = std::stol(get_parameter_value(argc, argv, _T("-iterations"), _T("16")));

    params.out.cubemapFilePath = get_parameter_value(argc, argv, _T("-omap"), _T(""));

    convolve_specular_radiance_map(cfg, params, metaGenerators);
}


void gat::converters::convolve_specular_radiance_map(const config_file &cfg,
    const specular_ibl_convolver_parameters &parameters, asset_meta_generators &metaGenerators)
{
    using namespace DirectX;

    // Load the input cubemap
    _tprintf(_T("Filtering cubemap texture '%s'...\n"), parameters.in.cubemapFilePath.c_str());

    auto inFilePath = to_wstring(parameters.in.cubemapFilePath.c_str());
    auto outFilePath = to_wstring(parameters.out.cubemapFilePath.c_str());

    // Startup D3D
    Microsoft::WRL::ComPtr<ID3D11Device> d3dDev;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;
    create_d3d11_device(d3dDev, d3dContext);

    // Convert the 2D texture to a cubemap
    Microsoft::WRL::ComPtr<ID3D11Texture2D> inCubemapTex;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> inCubemapSRV;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> inCubemapUAV;
    convert_tex2d_to_cubemap(cfg, inFilePath, false, d3dDev.Get(), d3dContext.Get(), inCubemapTex, inCubemapSRV, inCubemapUAV);

    // Convolve that cubemap with the specular D term
    _tprintf(_T("Convolving cubemap...\n"));
    
    // Load the shader for that
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> shader;
    {
        go::file_path csFilePath(cfg.applicationDirPath + _T("shaders\\gat_pbr_ibl_specular_convolve.cso"));
        auto csFile = go::binary_file::read(csFilePath);
        THROW_COM_ERROR(d3dDev->CreateComputeShader(csFile.data(), csFile.size(), nullptr, &shader));
        d3dContext->CSSetShader(shader.Get(), nullptr, 0);
    }

    // Bind output
    D3D11_TEXTURE2D_DESC outDesc;
    inCubemapTex->GetDesc(&outDesc);
    outDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    outDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    // Create output buffers for ping-ponging
    Microsoft::WRL::ComPtr<ID3D11Texture2D> outTex;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> prevColNormTex;
    THROW_COM_ERROR(d3dDev->CreateTexture2D(&outDesc, nullptr, &outTex));
    THROW_COM_ERROR(d3dDev->CreateTexture2D(&outDesc, nullptr, &prevColNormTex));

    std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> outTexUAVs;
    std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> outColNormTexUAVs;
    outTexUAVs.resize(outDesc.MipLevels);
    outColNormTexUAVs.resize(outDesc.MipLevels);

    CD3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
    uavDesc.Format = outDesc.Format;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
    uavDesc.Texture2DArray.FirstArraySlice = 0;
    uavDesc.Texture2DArray.ArraySize = 6;
    for(UINT mip = 0; mip < outDesc.MipLevels; ++mip)
    {
        uavDesc.Texture2DArray.MipSlice = mip;
        THROW_COM_ERROR(d3dDev->CreateUnorderedAccessView(outTex.Get(), &uavDesc, &outTexUAVs[mip]));
        THROW_COM_ERROR(d3dDev->CreateUnorderedAccessView(prevColNormTex.Get(), &uavDesc, &outColNormTexUAVs[mip]));
    }

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> prevColNormSRV;
    THROW_COM_ERROR(d3dDev->CreateShaderResourceView(prevColNormTex.Get(), nullptr, &prevColNormSRV));
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> outTexSRV;
    THROW_COM_ERROR(d3dDev->CreateShaderResourceView(outTex.Get(), nullptr, &outTexSRV));

    // Create constant buffer
    struct constants_t
    {
        UINT g_mipLevel;
        UINT g_numMips;
        UINT g_mipSize;
        UINT g_iteration;

        UINT g_numIterations;
        UINT _padding[3];
    };

    CD3D11_BUFFER_DESC cbDesc(sizeof(constants_t), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
    Microsoft::WRL::ComPtr<ID3D11Buffer> cb;
    THROW_COM_ERROR(d3dDev->CreateBuffer(&cbDesc, nullptr, &cb));
    d3dContext->CSSetConstantBuffers(0, 1, cb.GetAddressOf());

    // Create EnvMap sampler
    CD3D11_SAMPLER_DESC samplerDesc(D3D11_DEFAULT);
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
    THROW_COM_ERROR(d3dDev->CreateSamplerState(&samplerDesc, &samplerState));
    d3dContext->CSSetSamplers(0, 1, samplerState.GetAddressOf());

    // To avoid hazards
    void *nullBuffer = nullptr;

    // Figure out how many iterations we are actually doing
    const auto kNumIterations = std::min(32U, std::max(1U, parameters.in.iterations));

    d3dContext->CSSetUnorderedAccessViews(0, 1, (ID3D11UnorderedAccessView **)&nullBuffer, nullptr);
    d3dContext->CSSetShaderResources(0, 1, inCubemapSRV.GetAddressOf());

    for(UINT mip = 0; mip < outDesc.MipLevels; ++mip)
    {
        UINT mipSize = outDesc.Width >> mip;
        _tprintf(_T("Convolving MIP (%d/%d) [%d px]...\n"), mip + 1, outDesc.MipLevels, mipSize);

        for(auto i = 0U; i < kNumIterations; ++i)
        {
            if(kNumIterations > 1)
            {
                _tprintf(_T("Convolving iteration (%d/%d)...\n"), i + 1, kNumIterations);
            }

            // Update constants
            D3D11_MAPPED_SUBRESOURCE mappedRes;
            THROW_COM_ERROR(d3dContext->Map(cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes));
            auto p = (constants_t *)mappedRes.pData;
            p->g_mipLevel = mip;
            p->g_numMips = outDesc.MipLevels - 1;
            p->g_mipSize = mipSize;
            p->g_iteration = i;
            p->g_numIterations = kNumIterations;
            d3dContext->Unmap(cb.Get(), 0);

            // Figure out which buffer to use for read/write
            auto currentWrite = i % 2 == 0 ? outTexUAVs[mip].GetAddressOf() : outColNormTexUAVs[mip].GetAddressOf();
            auto currentRead = i % 2 == 0 ? prevColNormSRV.Get() : outTexSRV.Get();

            // Unbind to avoid hazards
            d3dContext->CSSetShaderResources(1, 1, (ID3D11ShaderResourceView **)&nullBuffer);
            d3dContext->CSSetUnorderedAccessViews(0, 1, (ID3D11UnorderedAccessView **)&nullBuffer, nullptr);

            // Bind the new read/write buffers
            d3dContext->CSSetShaderResources(1, 1, &currentRead);
            d3dContext->CSSetUnorderedAccessViews(0, 1, currentWrite, nullptr);

            // Run
            d3dContext->Dispatch(mipSize, mipSize, 6);

            // Prevents the driver from hanging
            d3dContext->Flush();
        }
    }

    auto targetSRV = kNumIterations % 2 == 1 ? outTexSRV.Get() : prevColNormSRV.Get();
    save_dds_srv(targetSRV, outFilePath, DXGI_FORMAT_BC6H_UF16);

    // Write out meta data
    saved_data savedData;
    savedData.emplace_back(asset_type::texture, parameters.out.cubemapFilePath);
    metaGenerators.push_back(savedData);
}