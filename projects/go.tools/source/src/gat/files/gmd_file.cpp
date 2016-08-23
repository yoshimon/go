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

#include "gat/files/gmd_file.h"


#include "gat/util/exception.h"
#include "go/golib.h"
#include <filesystem>


using namespace DirectX;
namespace fs = std::tr2::sys;


//===========================================================================
// gat::gmd_file
//===========================================================================

void gat::gmd_file::save(const go::file_path &filePath) const
{
    // Force the directory path
    fs::create_directories(filePath.directory_path().c_str());

    std::ofstream f;
    f.open(filePath, std::ios::binary);
    if(!f.is_open())
    {
        throw tchar_exception(_T("Could not create GMD output file."));
    }

    try
    {
        go::gmd_file::header_t header;
        header.magic = go::gmd_file::kMagic;
        header.version = 1;
        header.numUnskinnedMeshes = (uint32_t)unskinnedMeshes.size();
        header.numSkinnedMeshes = (uint32_t)skinnedMeshes.size();
        header.numUnskinnedVertices = (uint32_t)unskinnedVertices.size();
        header.numSkinnedVertices = (uint32_t)skinnedVertices.size();
        header.numIndices = (uint32_t)indices.size();
        header.numBones = 0; // (uint32_t)bones.size();
        f.write((char *)&header, sizeof(header));
        f.write((char *)unskinnedMeshes.data(), unskinnedMeshes.size() * sizeof(go::gmd_file::submesh_t));
        f.write((char *)skinnedMeshes.data(), skinnedMeshes.size() * sizeof(go::gmd_file::submesh_t));

		// Convert to 16-bit indices if possible
		if(header.numUnskinnedVertices + header.numSkinnedVertices <= USHRT_MAX)
		{
			// Conversion
			std::vector<go::gmd_file::index16_t> indices16;
			indices16.reserve(indices.size());
			std::copy(std::begin(indices), std::end(indices), std::back_inserter(indices16));
			
			f.write((char *)indices16.data(), indices16.size() * sizeof(go::gmd_file::index16_t));
		}
		else
		{
			f.write((char *)indices.data(), indices.size() * sizeof(go::gmd_file::index32_t));
		}

        f.write((char *)unskinnedVertices.data(), unskinnedVertices.size() * sizeof(go::gmd_file::unskinned_vertex_t));
        f.write((char *)skinnedVertices.data(), skinnedVertices.size() * sizeof(go::gmd_file::skinned_vertex_t));
        // f.write((char *)bones.data(), gfx_bones.size() * sizeof(go::world_bone));
    }
    catch(...)
    {
#ifdef UNICODE
        _wremove(filePath.c_str());
#else
        remove(filePath.c_str());
#endif
        throw tchar_exception(_T("Could not save GMD file."));
    }
}