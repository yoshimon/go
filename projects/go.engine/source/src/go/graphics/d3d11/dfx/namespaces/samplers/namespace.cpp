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

#include "dfx/namespaces/samplers/namespace.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/samplers/namespace_types.h"
#include "dfx/namespaces/samplers/namespace_declarations.h"


// ================================================================================ //
// dfx::samplers functions
// ================================================================================ //

void dfx::samplers::on_initialize(void *userData)
{
    using namespace dfx_hlsl::samplers;
    auto &samplers = go::the_gfx_device->common_sampler_states();
    g_linearClamp = const_cast<go::gfx_sampler_state *>(&samplers.linearClamp);
    g_linearWrap = const_cast<go::gfx_sampler_state *>(&samplers.linearWrap);
    g_aniso4x = const_cast<go::gfx_sampler_state *>(&samplers.anisotropic4xClamp);
    g_pointBorderWhite = const_cast<go::gfx_sampler_state *>(&samplers.pointBorderWhite);
    g_pointWrap = const_cast<go::gfx_sampler_state *>(&samplers.pointWrap);
    g_pointClamp = const_cast<go::gfx_sampler_state *>(&samplers.pointClamp);
    g_cmpShadow = const_cast<go::gfx_sampler_state *>(&samplers.shadowMapLinear);
    g_cmpShadowPoint = const_cast<go::gfx_sampler_state *>(&samplers.shadowMapPoint);
    g_surface = samplers.surfaceWrapSampler;
}


void dfx::samplers::on_shutdown()
{
}


void dfx::samplers::on_resize()
{
}
