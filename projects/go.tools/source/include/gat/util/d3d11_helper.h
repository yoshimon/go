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

#pragma once

#include "gat/defines.h"
#include "gat/files/config_file.h"
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

/*!
    Creates a D3D11 device.

    \param outDevice The output device.
    \param outContext The output context.
 */
void create_d3d11_device(Microsoft::WRL::ComPtr<ID3D11Device> &outDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> &outContext);
/*!
    Converts a 2D texture to a cubemap.

    \param d3dDevice The D3D device.
    \param d3dContext The D3D context.
 */
void convert_tex2d_to_cubemap(const config_file &cfg,
    const std::wstring &sourceFilePath, bool generateMips,
    ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext,
    Microsoft::WRL::ComPtr<ID3D11Texture2D> &outDestCubeTex,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> &outDestCubeSRV,
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> &outDestCubeUAV);
/*!
    Saves an SRV to a DDS file.

    \param d3dSRV The SRV to save.
    \param filePath The file path.
    \param overrideFormat The target format. Use DXGI_FORMAT_UNKNOWN to keep the source format.
 */
void save_dds_srv(ID3D11ShaderResourceView *d3dSRV, const std::wstring &filePath, DXGI_FORMAT overrideFormat = DXGI_FORMAT_UNKNOWN);

GAT_END_NAMESPACE