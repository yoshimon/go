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
    \file graphics/d3d11/dfx/effects/hdr_bloom.h
    \brief Contains type declarations for the HDR-Bloom effect.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


namespace dfx
{
    namespace effects
    {
        class gaussian_blur;
    }
}


GO_BEGIN_NAMESPACE

class gfx_texture;

namespace dfx_effects
{
    //! A structure, which contains the HDR (Bloom) parameters.
    struct hdr_bloom_parameters
    {
        /*!
            The maximum number of blur passes.
            
            This is also the number of mip-levels in the brightness texture.
         */
        static const uint32_t kNumBlurPasses = 4;
        struct
        {
            //! The bloom treshold.
            float bloomThreshold;
            //! Exposure adaption scale.
            float exposureAdaptionDelta;
            //! An upper bound for the measured luminance value.
            float maxLuminance;
            //! Middle-gray value for global tonemapping.
            float middleGray;
            //! The HDR color texture.
            gfx_texture *hdrColorTexture;
            //! The gaussian blur effect to use.
            dfx::effects::gaussian_blur *gaussianBlurEffect;
            //! The luminance-result of the last time the effect was executed.
            gfx_texture *lastLuminanceTexture;
        } in;
        struct
        {
            //! The output buffer.
            gfx_texture *hdrBloomTexture;
            //! The luminance texture.
            gfx_texture *luminanceTexture;
            /*!
                The brightness texture.

                This texture has to be mip-mapped on creation.
             */
            gfx_texture *brightnessTexture;
            /*!
                Temporary output buffer to write the blurred results into.
                
                Must be at least the size of the brightness texture.
             */
            gfx_texture *blurredBrightnessScratchTexture;
            /*!
                The blurred bright-pass scratch texture.

                This texture has to be mip-mapped on creation.
             */
            gfx_texture *blurredBrightnessTexture;
        } out;
    };
}

GO_END_NAMESPACE
