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


#include "go/graphics/d3d11/device.h"


#include "go/core/path_utility.h"
#include "go/graphics/d3d11/display.h"
#include "go/graphics/d3d11/shader.h"
#include "go/filesystem/system.h"
#include "go/platform/windows.h"
#include "go/core/log.h"
#include "go/variables.h"


// ================================================================================ //
// go::gfx_device member functions
// ================================================================================ //

go::gfx_device::gfx_device()
    : observer(this, &go::gfx_device::on_observer_gpu_display)
{
    initialize_back_buffer();
    initialize_depth_buffer();
    initialize_system_states();
    observe_gpu_display();
}


void go::gfx_device::initialize_back_buffer()
{
    m_backBuffer.reset();
    m_isBackBufferAvailable = true;
}


void go::gfx_device::initialize_depth_buffer()
{
    gfx_texture_format depthFmt(m_backBuffer.format());
    depthFmt.colorFormat = gfx_color_format::d32_float;
    m_depthBuffer.reset(depthFmt);
}


void go::gfx_device::initialize_system_states()
{
    // Blend states
    {
        m_systemBlendStates.alpha.reset(gfx_blend_mode::alpha);
        m_systemBlendStates.additive.reset(gfx_blend_mode::additive);
        m_systemBlendStates.multiply.reset(gfx_blend_mode::multiply);
    }

    // Sampler states
    {
        float white[] = { 1.f, 1.f, 1.f, 1.f };

        m_systemSamplerStates.pointClamp.reset(
            gfx_sampler_state_type::min_mag_mip_point,
            gfx_texture_address_mode::clamp,
            gfx_texture_address_mode::clamp,
            gfx_texture_address_mode::clamp);
        m_systemSamplerStates.pointWrap.reset(gfx_sampler_state_type::min_mag_mip_point);
        m_systemSamplerStates.pointBorderWhite.reset(
            gfx_sampler_state_type::min_mag_mip_point,
            gfx_texture_address_mode::border,
            gfx_texture_address_mode::border,
            gfx_texture_address_mode::border,
            comparison_function::never,
            white);
        m_systemSamplerStates.linearWrap.reset(gfx_sampler_state_type::min_mag_mip_linear);
        m_systemSamplerStates.linearClamp.reset(
            gfx_sampler_state_type::min_mag_mip_linear,
            gfx_texture_address_mode::clamp,
            gfx_texture_address_mode::clamp,
            gfx_texture_address_mode::clamp);
        m_systemSamplerStates.anisotropic4xWrap.reset(
			gfx_sampler_state_type::anisotropic4x,
			gfx_texture_address_mode::wrap,
			gfx_texture_address_mode::wrap,
			gfx_texture_address_mode::wrap);
		m_systemSamplerStates.anisotropic4xClamp.reset(
			gfx_sampler_state_type::anisotropic4x,
			gfx_texture_address_mode::clamp,
			gfx_texture_address_mode::clamp,
			gfx_texture_address_mode::clamp);
		m_systemSamplerStates.shadowMapLinear.reset(
            gfx_sampler_state_type::compare_min_mag_linear_mip_point,
            gfx_texture_address_mode::clamp,
            gfx_texture_address_mode::clamp,
            gfx_texture_address_mode::clamp,
            comparison_function::less);
        m_systemSamplerStates.shadowMapPoint.reset(
            gfx_sampler_state_type::compare_min_mag_mip_point,
            gfx_texture_address_mode::clamp,
            gfx_texture_address_mode::clamp,
            gfx_texture_address_mode::clamp,
            comparison_function::less_equal);

        if(GO_CVAR(texture_filter).valuei() == 1)
        {
            m_systemSamplerStates.surfaceWrapSampler = &m_systemSamplerStates.anisotropic4xWrap;
        }
        else
        {
            m_systemSamplerStates.surfaceWrapSampler = &m_systemSamplerStates.linearWrap;
        }
    }

    // Depth states
    {
        gfx_depth_stencil_state_parameters o;
		o.stencil.enableTest = false;

        // Test & Write
        o.depth.enableTest = true;
        o.depth.enableWrite = true;

        o.depth.comparisonFunction = comparison_function::less;
        m_systemDepthStencilStates.lessTestWrite.reset(o);

        o.depth.comparisonFunction = comparison_function::less_equal;
        m_systemDepthStencilStates.lessEqualTestWrite.reset(o);

        o.depth.comparisonFunction = comparison_function::greater;
        m_systemDepthStencilStates.greaterTestWrite.reset(o);

        // Test
        o.depth.enableWrite = false;

        o.depth.comparisonFunction = comparison_function::less;
        m_systemDepthStencilStates.lessTestOnly.reset(o);

        o.depth.comparisonFunction = comparison_function::less_equal;
        m_systemDepthStencilStates.lessEqualTestOnly.reset(o);

        o.depth.comparisonFunction = comparison_function::greater;
        m_systemDepthStencilStates.greaterTestOnly.reset(o);

        o.depth.comparisonFunction = comparison_function::greater_equal;
        m_systemDepthStencilStates.greaterEqualTestOnly.reset(o);
    }

    // Rasterizer
	{
		gfx_rasterizer_parameters o;

		o.cullMode = gfx_rasterizer_cull_mode::none;
		o.depthBias = 0;
		o.depthBiasClamp = 0;
		o.slopeScaledDepthBias = 0;
		o.depthClipEnable = true;
		o.frontCounterClockwise = false;
		m_systemRasterizerStates.twoSided.reset(o);

        o.cullMode = gfx_rasterizer_cull_mode::front;
        m_systemRasterizerStates.backFaces.reset(o);

        o.cullMode = gfx_rasterizer_cull_mode::back;
        m_systemRasterizerStates.frontFaces.reset(o);
        
        auto d32_depth_bias = [](float d) { return (int)(d / (1 / std::pow(2, 23))); };
        
        o.cullMode = gfx_rasterizer_cull_mode::back;
		o.depthBias = d32_depth_bias(0.0001f);
		o.slopeScaledDepthBias = 3.0f;
        m_systemRasterizerStates.shadow.reset(o);
        
        o.cullMode = gfx_rasterizer_cull_mode::none;
		m_systemRasterizerStates.shadowTwoSided.reset(o);

        o.depthClipEnable = false;
		o.depthBias = d32_depth_bias(0.00005f);
		o.slopeScaledDepthBias = 2.0f;
		o.cullMode = gfx_rasterizer_cull_mode::back;
        m_systemRasterizerStates.shadowDirectional.reset(o);
        
        o.cullMode = gfx_rasterizer_cull_mode::none;
        m_systemRasterizerStates.shadowDirectionalTwoSided.reset(o);
    }
}


void go::gfx_device::observe_gpu_display()
{
    the_display->add_observer(static_cast<gfx_display::observer_type *>(this));
}


bool go::gfx_device::on_observer_gpu_display(gfx_display *, gfx_display::event_type &event)
{
    switch(event.msg)
    {
    case gfx_display_event_data::event::resizing:
        m_isBackBufferAvailable = false;
        m_backBuffer.release();
        m_depthBuffer.release();
        break;
    case gfx_display_event_data::event::resized:
		initialize_back_buffer();
        initialize_depth_buffer();
        break;
    }

    return true;
}