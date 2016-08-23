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

#include "gat/tools/unity_scene_converter.h"


#include "gat/util/converters.h"


GAT_DEFINE_TOOL(unity_scene_converter, _T("unity_scene_converter"))


//===========================================================================
// gat::unity_scene_converter
//===========================================================================

void gat::unity_scene_converter::run(int argc, const _TCHAR *argv[], const config_file &cfg)
{
    _tprintf(_T("Unity Scene Converter loaded.\n"));

    asset_meta_generators metaGenerators;

    // Load the scene and convert each unique model to a separate file
    converters::convert_unity_scene(argc, argv, cfg, metaGenerators);

    // Write out the meta data
    metaGenerators.generate_and_save();
}