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
    \file graphics/d3d11/device.h
    \brief Contains a class for world resource allocation.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/design/observer.h"

#include "go/graphics/d3d11/blend_state.h"
#include "go/graphics/d3d11/display.h"
#include "go/graphics/d3d11/device.h"
#include "go/graphics/d3d11/shader.h"
#include "go/graphics/d3d11/sampler_state.h"
#include "go/graphics/d3d11/depth_stencil_state.h"
#include "go/graphics/d3d11/rasterizer_state.h"

#include "go/graphics/asset_management/material_manager.h"
#include "go/graphics/asset_management/model_manager.h"
#include "go/graphics/asset_management/shader_manager.h"
#include "go/graphics/asset_management/texture_manager.h"


GO_BEGIN_NAMESPACE

//! A class, which manages Direct3D11 resources. 
class gfx_device : public observer<gfx_device, gfx_display, gfx_display::event_type &>
{
    GO_DECLARE_NON_COPYABLE(gfx_device)
public:
    //! A structure, which serves as a container for system blend states.
    struct common_blend_states_t
    {
        //! Alpha-blending.
        gfx_blend_state alpha;
        //! Additive blending.
        gfx_blend_state additive;
        //! Multiplicative blending.
        gfx_blend_state multiply;
    };
    //! A structure, which serves as a container for system sampler states.
    struct common_sampler_states_t
    {
        //! Min-Mag-Mip point sampler using wrap-addressing mode.
        gfx_sampler_state pointWrap; 
        //! Min-Mag-Mip point sampler using clamp-addressing mode.
        gfx_sampler_state pointClamp;
        //! Min-Mag-Mip point sampler using border-addressing mode.
        gfx_sampler_state pointBorderWhite;
        //! Min-Mag-Mip linear sampler using wrap addressing mode.
        gfx_sampler_state linearWrap;
        //! Min-Mag-Mip linear sampler using clamp addressing mode.
        gfx_sampler_state linearClamp;
        //! Anisotropic (4x) sampler using wrap addressing mode.
        gfx_sampler_state anisotropic4xWrap;
		//! Anisotropic (4x) sampler using clamp addressing mode.
		gfx_sampler_state anisotropic4xClamp;
		//! Shadow map linear sampler.
        gfx_sampler_state shadowMapLinear;
        //! Shadow map point sampler.
        gfx_sampler_state shadowMapPoint;
        //! Diffuse wrap-sampler.
        gfx_sampler_state *surfaceWrapSampler;
    };
    //! A structure, which serves as a container for system depth stencil states.
    struct common_depth_stencil_states_t
    {
        //! Less-depth-test with write enabled.
        gfx_depth_stencil_state lessTestWrite;
        //! Less-depth-test with write disabled.
        gfx_depth_stencil_state lessTestOnly;
        //! Less-equal-depth test with write enabled.
        gfx_depth_stencil_state lessEqualTestWrite;
        //! Less-equal-depth test with write disabled.
        gfx_depth_stencil_state lessEqualTestOnly;
        //! Greater-depth test with write disabled.
        gfx_depth_stencil_state greaterTestOnly;
        //! Greater-equal depth test with write disabled.
        gfx_depth_stencil_state greaterEqualTestOnly;
        //! Greater-depth test write write enabeld.
        gfx_depth_stencil_state greaterTestWrite;
    };
    //! A structure, which serves as a container for system rasterizer states.
    struct common_rasterizer_states_t
    {
        //! Default rasterizer state for two-sided rendering.
        gfx_rasterizer_state twoSided;
        //! Directional shadow rasterizer state.
        gfx_rasterizer_state shadowDirectional;
        //! Same as shadowDirectional for two-sided materials.
        gfx_rasterizer_state shadowDirectionalTwoSided;
        //! Shadow rasterizer state.
        gfx_rasterizer_state shadow;
        //! Same as shadow for two-sided materials.
        gfx_rasterizer_state shadowTwoSided;
        //! Back-faces only.
        gfx_rasterizer_state backFaces;
        //! Front-faces only.
        gfx_rasterizer_state frontFaces;
    };
public:
    //! Constructor.
    gfx_device();
public:
    /*!
        Returns the back-buffer render-target.
        This render-target is guaranteed to be native-sampled (i.e. the same size as the viewport).

        \return The back-buffer.
     */
    gfx_back_buffer *back_buffer() noexcept { return m_isBackBufferAvailable ? &m_backBuffer : nullptr; }
    /*!
        Returns the depth-buffer.
        This render-target is guaranteed to be native-sampled (i.e. the same size as the viewport).

        \return The depth-buffer.
     */
    gfx_depth_stencil_buffer *depth_buffer() noexcept{ return m_isBackBufferAvailable ? &m_depthBuffer : nullptr; }
    /*!
        Returns a collection of system blend states.
        
        \return A reference to the collection of system blend states.
     */
    const common_blend_states_t &common_blend_states() const noexcept { return m_systemBlendStates; }
    /*!
        Returns a collection of system sampler states.
        
        \return A reference to the collection of system sampler states.
     */
    const common_sampler_states_t &common_sampler_states() const noexcept { return m_systemSamplerStates; }
    /*!
        Returns a collection of system depth-stencil states.

        \return A reference to the collection of system depth-stencil states.
     */
    const common_depth_stencil_states_t &common_depth_stencil_states() const noexcept{ return m_systemDepthStencilStates; }
    /*!
        Returns a collection of system rasterizer states.

        \return A reference to the collection of system rasterizer states.
     */
    const common_rasterizer_states_t &common_rasterizer_states() const noexcept{ return m_systemRasterizerStates; }
    //! Updates the device and its internal states.
    void update();
private:
    /*!
        Callback function which is invoked when a display event occurs.
        The back-buffer should be resized accordingly.

        \param sender The sender.
        \param event The event.

        \return Always true.
     */
    bool on_observer_gpu_display(gfx_display *sender, gfx_display::event_type &event);
    //! Initializes the back-buffer.
    void initialize_back_buffer();
    //! Initializes the depth-buffer.
    void initialize_depth_buffer();
    //! Initializes the system states.
    void initialize_system_states();
    //! Registers the device with the display driver.
    void observe_gpu_display();
private:
    //! Dirty-flag.
    bool m_isBackBufferAvailable;
    //! The back-buffer.
    gfx_back_buffer m_backBuffer;
    //! The depth-buffer.
    gfx_depth_stencil_buffer m_depthBuffer;
    //! A collection of system blend states.
    common_blend_states_t m_systemBlendStates;
    //! A collection of system sampler states.
    common_sampler_states_t m_systemSamplerStates;
    //! A collection of system depth-stencil states.
    common_depth_stencil_states_t m_systemDepthStencilStates;
    //! A collection of a system rasterizer states.
    common_rasterizer_states_t m_systemRasterizerStates;
};

GO_END_NAMESPACE