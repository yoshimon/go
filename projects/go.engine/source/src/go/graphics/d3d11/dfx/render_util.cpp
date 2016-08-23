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


#include "go/graphics/d3d11/dfx/render_util.h"


#include "go/graphics/asset_management/shader_manager.h"


// ================================================================================ //
// go::dfx_util::common_shader_cache functions
// ================================================================================ //

void go::dfx_util::common_shader_cache::reset(gfx_shader_manager *shaderMgr)
{
	// Common
	common.fsTriangleVS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "shaders\\vertex_to_pixel\\fstriangle_vs.cso");
    common.skyVS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "shaders\\vertex_to_pixel\\sky_vs.cso");
	common.unskinnedGeometryVS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "shaders\\vertex_to_pixel\\unskinned_vs.cso");
	common.unskinnedDepthGeometryVS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "shaders\\vertex_to_pixel\\unskinned_depth_vs.cso");

	// Half-res upsample
	halfresUpsampleEffect.halfresUpsamplePS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\halfres_upsample\\halfres_upsample_ps.cso");

	// Shadow Maps
	shadowMapsEffect.unskinnedGeometryVS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\shadow_maps\\unskinned_vs.cso");

    // MinZ Downsampling
    minZDownsamplingEffect.minZDownsamplingCS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\min_z_downsampling\\min_z_downsampling_cs.cso");

	// Bilateral Blur
	bilateralBlurEffect.blurXCS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\bilateral_blur\\blur_x_cs.cso");
	bilateralBlurEffect.blurYCS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\bilateral_blur\\blur_y_cs.cso");

	// Overlay
	overlayEffect.overlayPS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\overlay\\overlay_ps.cso");

	// Gaussian Blur
    gaussianBlurEffect.blurXCS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\gaussian_blur\\blur_x_cs.cso");
    gaussianBlurEffect.blurYCS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\gaussian_blur\\blur_y_cs.cso");

	// HDR / Bloom
    hdrBloomEffect.luminanceBrightnessCS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\hdr_bloom\\luminance_brightness_cs.cso");
    hdrBloomEffect.averageLuminanceCS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\hdr_bloom\\average_luminance_cs.cso");
    hdrBloomEffect.combineCS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\hdr_bloom\\combine_cs.cso");

	// Tonemapping
    tonemappingEffect.tonemappingPS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\tonemapping\\tonemap_ps.cso");

	// Film Grain
    filmGrainEffect.filmGrainPS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\film_grain\\film_grain_ps.cso");

	// Vignette
    vignetteEffect.vignettePS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\vignette\\vignette_ps.cso");

	// FXAA
	fxaaEffect.fxaaPS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\fxaa\\fxaa_ps.cso");

	// Volumetric Lighting
    volumetricLightingEffect.volumetricLightingCS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\volumetric_lighting\\volumetric_lighting_cs.cso");
    volumetricLightingEffect.applyTransmittancePS = shaderMgr->get(GO_PATH_SYMBOL_SHADER "effects\\volumetric_lighting\\apply_transmittance_ps.cso");

    // Material depth-only shaders
	go::file_path pxShaderPath(GO_PATH_SYMBOL_SHADER, "shaders\\pixel");
    for(uint32_t i = 0; i < (uint32_t)go::gfx_material_type::count; ++i)
    {
        auto &matTypeStr = go::gfx_material_type_strings[i];
        go::file_path filePath(pxShaderPath, matTypeStr);
        filePath.replace_extension("_depth_ps.cso");
        common.materialDepthOnlyPSTable[i] = shaderMgr->get(filePath);
    }
}