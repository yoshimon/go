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
    \file graphics/d3d11/resource.h
    \brief Contains a class which represents a shader resource that can be bound to the GPU pipeline.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/bitmask.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! A class, which represents a resource, that can be bound to GPU shaders.
class GO_API gfx_shader_resource
{
public:
    //! Destructor.
    virtual ~gfx_shader_resource() = default;
public:
    /*!
        Returns the internal D3D SRV.

        \param index The resource index.

        \return The internal D3D SRV.
     */
    ID3D11ShaderResourceView *d3d_srv(int32_t index = 0) const { return m_d3dSRVArray[index].Get(); }
    /*!
        Returns the internal D3D SRV.

        \param index The resource index.

        \return The internal D3D SRV.
     */
    ID3D11ShaderResourceView *d3d_srv_layer(int32_t index = 0) const { return m_d3dSRVLayerArray[index].Get(); }
    /*!
        Returns the internal D3D UAV.

        \param index The resource index.

        \return The internal D3D UAV.
     */
    ID3D11UnorderedAccessView *d3d_uav(int32_t index = 0) const { return m_d3dUAVArray[index].Get(); }
    //! Releases the object.
    virtual void release();
    //! Output bind mask.
    std::array<bitmask<D3D11_PS_CS_UAV_REGISTER_COUNT>, 3> internalOutputBindMask;
    //! Output bind mask.
    std::array<bitmask<D3D11_PS_CS_UAV_REGISTER_COUNT>, 3> internalPendingOutputBindMask;
    //! Bind masks for each shader stage.
    std::array<bitmask<D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT>, 4> internalInputBindMask;
    //! Bind masks for each shader stage.
    std::array<bitmask<D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT>, 4> internalPendingInputBindMask;
protected:
    //! The internal D3D SRV.
    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_d3dSRVArray;
    //! The internal D3D SRV.
    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_d3dSRVLayerArray;
    //! The internal D3D SRV.
    std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> m_d3dUAVArray;
};

GO_END_NAMESPACE