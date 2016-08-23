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

#include "gat/tools/tool_base.h"


typedef std::unordered_map<gat::tstring, gat::tool_base *> converter_map_t;
converter_map_t g_assetConverters;

//===========================================================================
// gat::tool_base
//===========================================================================

auto gat::tool_base::from_name(const _TCHAR *name) -> tool_base *
{
    auto it = g_assetConverters.find(name);
    return it == g_assetConverters.end() ? nullptr : it->second;
}


gat::tool_base::tool_base(const _TCHAR *name)
{
    g_assetConverters.emplace(name, this);
}