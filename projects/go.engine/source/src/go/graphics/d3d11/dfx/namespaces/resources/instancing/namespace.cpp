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

#include "dfx/namespaces/resources/namespaces/instancing/namespace.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/resources/namespaces/instancing/namespace_types.h"
#include "dfx/namespaces/resources/namespaces/instancing/namespace_declarations.h"

#include "dfx/namespaces/instance_data/namespace_types.h"


using namespace go;


// ================================================================================ //
// dfx::resources::instancing functions
// ================================================================================ //

void dfx::resources::instancing::on_initialize(void *userData)
{
    dfx_hlsl::resources::instancing::g_unskinnedInstanceData = new gfx_structured_buffer(
        4096, // This is an upper bound for the number of instances we can hold
        sizeof(dfx_hlsl::instance_data::unskinned_instance_data),
        gfx_access_policy::read_only,
        gfx_buffer_update_frequency::high
    );
}


void dfx::resources::instancing::on_shutdown()
{
    GO_SAFE_DELETE(dfx_hlsl::resources::instancing::g_unskinnedInstanceData);
}


void dfx::resources::instancing::on_resize()
{
}
