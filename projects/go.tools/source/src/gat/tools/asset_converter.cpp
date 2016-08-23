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

#include "gat/tools/asset_converter.h"


#include "gat/files/asset_meta_file.h"
#include "gat/util/exception.h"
#include "gat/util/cmd_help.h"
#include "gat/util/converters.h"


GAT_DEFINE_TOOL(asset_converter, _T("asset_converter"))


//===========================================================================
// gat::asset_converter
//===========================================================================

void gat::asset_converter::run(int argc, const _TCHAR *argv[], const config_file &cfg)
{
    _tprintf(_T("Asset Converter loaded.\n"));

    asset_meta_generators metaGenerators;

    // Determine the type of asset to convert
    auto assetType = get_parameter_value(argc, argv, _T("-type"), _T(""));
    if(!tstrcmp(assetType, asset_type_string[(int32_t)asset_type::texture]))
    {
        // Converts a single texture (e.g. BMP) to a Go compatible format
        converters::convert_texture(argc, argv, cfg, metaGenerators);
    }
    else if(!tstrcmp(assetType, asset_type_string[(int32_t)asset_type::material]))
    {
        // Unity material set
        converters::convert_unity_material_set(argc, argv, cfg, metaGenerators);
    }
    else
    {
        throw tchar_exception(_T("No valid asset type specified."));
    }

    // Write out the meta data
    metaGenerators.generate_and_save();
}