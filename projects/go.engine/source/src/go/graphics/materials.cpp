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


#include "go/graphics/materials.h"


// ================================================================================ //
// Helper functions
// ================================================================================ //

go::gfx_texture_format default_1x1_texture_fmt(go::gfx_color_format colorFmt)
{
    return go::gfx_texture_format::as_2d(1, 1, colorFmt);
}


// ================================================================================ //
// Helper macros
// ================================================================================ //

#define DEFINE_MATERIAL_TEXTURE(TextureData, ColorFormat) { \
        &s_textureData_##TextureData , default_1x1_texture_fmt(go::gfx_color_format::ColorFormat) }

#define DEFINE_TEXTURE_DATA(Type, Name) Type s_textureData_##Name

// ================================================================================ //
// Static texture data
// ================================================================================ //

DEFINE_TEXTURE_DATA(uint32_t, R32_White) = { 0xFFFFFFFF };
DEFINE_TEXTURE_DATA(uint32_t, R32_Black) = { 0x0 };
DEFINE_TEXTURE_DATA(uint32_t, R32_RoughMetal) = { 0x000000FF }; // R = Metal, A = Smoothness
DEFINE_TEXTURE_DATA(uint16_t, R16_NeutralNormal) = { 0x8080 };


// ================================================================================ //
// Default material textures
// ================================================================================ //

// Global material texture fallback table (update on material add / delete)
go::gfx_texture_manager::default_texture_info_t s_defaultTextureInfos[]
{
    // Standard
    DEFINE_MATERIAL_TEXTURE(R32_White, b8g8r8a8_unorm_sRGB), // Albedo
    DEFINE_MATERIAL_TEXTURE(R32_RoughMetal, r8g8b8a8_unorm), // MetallicSmoothness
    DEFINE_MATERIAL_TEXTURE(R16_NeutralNormal, r8g8_unorm), // Normal
    DEFINE_MATERIAL_TEXTURE(R32_White, r8_unorm), // Occlusion
    DEFINE_MATERIAL_TEXTURE(R32_White, r8g8b8a8_unorm), // Emission
    // Unlit
    DEFINE_MATERIAL_TEXTURE(R32_White, b8g8r8a8_unorm_sRGB), // Color
};


// ================================================================================ //
// Global functions
// ================================================================================ //

go::gfx_texture_manager::default_textures_info_t go::gfx_default_material_textures_info()
{
    return{ s_defaultTextureInfos, _countof(s_defaultTextureInfos) };
}