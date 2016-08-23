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
    \file graphics/d3d11/shader.h
    \brief Contains a class for creating graphics shaders.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! Enumeration of possible shader types.
enum class gfx_shader_type
{
    //! A pixel shader.
    pixel,
    //! A vertex shader.
    vertex,
    //! A compute shader.
    compute,
    //! A geometry shader.
    geometry
};


//! A class, which implements common shader functionality.
class GO_API gfx_shader
{
    GO_DECLARE_NON_COPYABLE(gfx_shader)
public:
    /*!
        Constructs the object and initializes it.

        \param type The shader type.
        \param byteCode The compiled shader code.
        \param numBytes Number of bytes pointed to by <i>byteCode</i>.
     */
    gfx_shader(gfx_shader_type type, const uint8_t *byteCode, uint32_t numBytes);
    /*!
        Constructs the object and initializes it.

        \param type The shader type.
        \param byteCode The compiled shader code.
     */
    gfx_shader(gfx_shader_type type, const byte_vector &byteCode);
public:
    /*!
        Returns the profile description of the shader.

        \return The shader profile string.
     */
    const std::string &profile_string() const noexcept;
    /*!
        Returns the shader type.
        
        \return The shader type string.
     */
    const std::string &type_string() const noexcept;
    /*!
        Returns the shader type.

        \return The shader type.
     */
    gfx_shader_type type() const noexcept { return m_type; }
    /*!
        Returns the internal D3D shader.

        \return The internal D3D shader.
     */
    ID3D11DeviceChild *d3d_shader() const noexcept { return m_d3dShader.Get(); }
    /*!
        Returns the internal D3D input-layout.

        \return The internal D3D input layout.
     */
    ID3D11InputLayout *d3d_input_layout() const noexcept { return m_inputLayout.Get(); }
protected:
    //! Hidden constructor.
    gfx_shader() = default;
protected:
    /*!
        Initializes the object.

        \param type The shader type.
        \param byteCode The compiled shader code.
        \param numBytes Number of bytes pointed to by <i>byteCode</i>.
     */
    void initialize(gfx_shader_type type, const uint8_t *byteCode, uint32_t numBytes);
private:
    //! The Direct3D vertex shader object.
    Microsoft::WRL::ComPtr<ID3D11DeviceChild> m_d3dShader;
    //! The reflection instance.
    Microsoft::WRL::ComPtr<ID3D11ShaderReflection> m_reflection;
    /*!
        The input layout.
        Vertex-shaders only.
     */
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
    //! The shader type.
    gfx_shader_type m_type;
    //! The profile string.
    std::string m_profile;
    //! The type string.
    std::string m_typeString;
};

GO_END_NAMESPACE