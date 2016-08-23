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
    \file graphics/d3d11/dfx/effects/volumetric_lighting.h
    \brief Contains type declarations for the volumetric lighting effect.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"

namespace dfx
{
    namespace effects
    {
        class bilateral_blur;
        class halfres_upsample;
    }
}

GO_BEGIN_NAMESPACE

class gfx_texture;

namespace dfx_effects
{
    //! A structure, which contains the volumetric lighting parameters.
    struct volumetric_lighting_parameters
    {
        struct
        {
            //! The radius of the effect.
            float intensity;
            //! The sample count.
            uint32_t sampleCount;
            //! The view-space depth texture.
            gfx_texture *viewDepthTexture;
            //! The directional-light shadow map.
            gfx_texture *directionalLightShadowMap;
            //! The bilateral blur effect to use.
            dfx::effects::bilateral_blur *bilateralBlurEffect;
            //! The upsampling effect to use.
            dfx::effects::halfres_upsample *halfresUpsampleEffect;
        } in;
        struct
        {
            //! The output texture.
            gfx_render_target *volumetricLightingOutputTexture;
            /*!
                Temporary output buffer to write the blurred results into.

                Must be at least the size of the output texture texture.
             */
            gfx_texture *blurredOutputScratchTexture;
            //! The combined target texture.
            gfx_render_target *combinedOutputTexture;
        } out;
    };
}

GO_END_NAMESPACE
