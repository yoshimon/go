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
    \file core/performance_counter.h
    \brief Contains classes for keeping track of different performance counters.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

//! A structure, that holds important timer events.
struct performance_timers
{
    //! The time in milliseconds to issue the shadow maps commands.
    float shadowMaps;
    //! The time in milliseconds to issue the prepass commands.
    float prepass;
    //! The time in milliseconds to issue the shading commands.
    float shading;
    //! The time in milliseconds to issue the depth downsampling commands.
    float depthDownsampling;
    //! The time in milliseconds to issue the HBAO+ commands.
    float hbao;
    //! The time in milliseconds to issue the ambient lighting commands.
    float ambientLighting;
    //! The time in milliseconds to issue the volumetric lighting commands.
    float volumetricLighting;
    //! The time in milliseconds to issue the HDR (Bloom) commands.
    float hdrBloom;
    //! The time in milliseconds to issue the tonemapping commands.
    float tonemapping;
    //! The time in milliseconds to issue the FXAA commands.
    float fxaa;
    //! The time in milliseconds to issue the film grain commands.
    float filmGrain;
    //! The time in milliseconds to issue the vignette commands.
    float vignette;
};

//! A structure, which keeps track of different performance numbers.
struct performance_counter
{
    //! The elapsed time since the last frame was processed in seconds.
    float elapsedTime;
    //! Interpolation factor between the current and the next frame.
    float blendFactor;
    //! The number of frames per second that are being processed.
    int32_t fps;
    /*!
        The global game time.

        The elapsed time in milliseconds since the engine was started.
     */
    uint32_t engineFrameTimeMS;
    //! A structure, which keeps track of performance number that are specific to the renderer.
    struct renderer
    {
        //! The number of rendered triangles.
        int32_t renderedObjects;
    };
    //! The CPU timers.
    performance_timers cpu;
    //! The GPU timers.
    performance_timers gpu;
};


/*!
    The global performance counter instance.

    Only selected, engine internal, services should update (write to) this.
 */
extern GO_API performance_counter performance;
//! The global game time in seconds.
extern GO_API float the_game_time;

GO_END_NAMESPACE
