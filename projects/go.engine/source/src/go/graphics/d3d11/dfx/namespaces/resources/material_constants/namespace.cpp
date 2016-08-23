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

#include "dfx/namespaces/resources/namespaces/material_constants/namespace.h"
#include "go/graphics/d3d11/dfx/go_types.h"

#include "dfx/namespaces/resources/namespaces/material_constants/namespace_types.h"
#include "dfx/namespaces/resources/namespaces/material_constants/namespace_declarations.h"

#include "dfx/namespaces/materials/namespaces/constants/namespace_types.h"


using namespace go;


// ================================================================================ //
// Helper functions
// ================================================================================ //

void initialize_standard_constants(go::gfx_material_database *materialDB)
{
    if(!materialDB)
    {
        return;
    }

    // CPU constants
    std::vector<dfx_hlsl::materials::constants::standard> standardConstants;
    auto &materialTypeIndices = materialDB->material_type_indices();
    auto &standardMaterialIndices = materialTypeIndices[(size_t)gfx_material_type::standard];
    standardConstants.reserve(materialDB->capacity());

    // Figure out how many standard materials we have and read their constants
	for(size_t materialIndex = 0, materialTypeIndex = 0; materialIndex < materialDB->capacity(); ++materialIndex)
    {
		if(materialTypeIndex < standardMaterialIndices.size() && standardMaterialIndices[materialTypeIndex] == materialIndex)
		{
			standardConstants.push_back(materialDB->find(materialIndex)->constants.standard);
			++materialTypeIndex;
		}
		else
		{
			// Push dummy record
			standardConstants.emplace_back();
		}
    }

    // GPU constants
    GO_SAFE_DELETE(dfx_hlsl::resources::material_constants::g_standard);
    dfx_hlsl::resources::material_constants::g_standard = new gfx_structured_buffer(
        standardConstants.size(),
        sizeof(dfx_hlsl::materials::constants::standard),
        gfx_access_policy::read_only,
        gfx_buffer_update_frequency::never,
        gfx_structured_buffer_data_type::user,
        standardConstants.data()
    );
}


// ================================================================================ //
// dfx::resources::material_constants functions
// ================================================================================ //

void dfx::resources::material_constants::on_initialize(void *userData)
{
    initialize_standard_constants((go::gfx_material_database *)userData);

    // TODO: add an update mechanicsm using a compute shader that updates the DB when it changes
}


void dfx::resources::material_constants::on_shutdown()
{
    GO_SAFE_DELETE(dfx_hlsl::resources::material_constants::g_standard);
}


void dfx::resources::material_constants::on_resize()
{
}
