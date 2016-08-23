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


#include "go/core/common.h"
#include "go/graphics/asset_management/asset_context.h"


GO_BEGIN_NAMESPACE

namespace dfx_util
{
    //! Enumeration of possible render modes.
    enum class geometry_mask
    {
        //! Render all geometry.
        all,
        //! Render opaque geometry only.
        opaque_only,
        //! Render transparent geometry only.
        transparent_only
    };

    //! A structure, that holds the common shaders.
    struct common_shader_cache
    {
        struct
        {
            //! Full-screen triangle shader.
            gfx_shader *fsTriangleVS;
            //! Sky shader.
            gfx_shader *skyVS;
            //! Unskinned geometry shader.
            gfx_shader *unskinnedGeometryVS;
			//! Unskinned depth-only geometry shader.
			gfx_shader *unskinnedDepthGeometryVS;
            //! Depth-only material shaders.
            gfx_shader *materialDepthOnlyPSTable[(uint32_t)go::gfx_material_type::count];
        } common;
		struct
		{
			//! Shadow map shader.
			gfx_shader *unskinnedGeometryVS;
		} shadowMapsEffect;
        struct
        {
            //! MinZ downsampling shader.
            gfx_shader *minZDownsamplingCS;
        } minZDownsamplingEffect;
        struct
        {
            //! Gaussian blur shader.
            gfx_shader *blurXCS;
            //! Gaussian blur shader.
            gfx_shader *blurYCS;
        } gaussianBlurEffect;
		struct
		{
			//! Bilateral blur shader.
			gfx_shader *blurXCS;
			//! Bilateral blur shader.
			gfx_shader *blurYCS;
		} bilateralBlurEffect;
		struct
		{
			//! Overlay shader.
			gfx_shader *overlayPS;
		} overlayEffect;
        struct
        {
            //! Luminance brightness shader.
            gfx_shader *luminanceBrightnessCS;
            //! Average luminance shader.
            gfx_shader *averageLuminanceCS;
            //! Combine shader.
            gfx_shader *combineCS;
        } hdrBloomEffect;
        struct
        {
            //! Luminance brightness shader.
            gfx_shader *tonemappingPS;
        } tonemappingEffect;
        struct
        {
            //! Film grain shader.
            gfx_shader *filmGrainPS;
        } filmGrainEffect;
        struct
        {
            //! Vignette shader.
            gfx_shader *vignettePS;
        } vignetteEffect;
		struct
		{
			//! FXAA shader.
			gfx_shader *fxaaPS;
		} fxaaEffect;
		struct
		{
			//! Volumetric lighting shader.
            gfx_shader *volumetricLightingCS;
            //! Applies the transmittance to the incident radiance.
            gfx_shader *applyTransmittancePS;
		} volumetricLightingEffect;
		struct
		{
			//! Half-resolution upsampling shader.
			gfx_shader *halfresUpsamplePS;
		} halfresUpsampleEffect;
        //! Resets the shader cache.
        virtual void reset(gfx_shader_manager *manager);
    };

    //! A structure, that holds information about common effect parameters.
    struct effect_parameters
    {
        //! The asset context.
        gfx_asset_context *assetContext;
    };
}

GO_END_NAMESPACE