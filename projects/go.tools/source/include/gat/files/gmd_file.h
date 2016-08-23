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

#pragma once

#include "gat/defines.h"
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

//! A class, which provides an interface for GMD files.
struct gmd_file
{
    //! The unskinned meshes.
    std::vector<go::gfx_mesh> unskinnedMeshes;
    //! The skinned meshes.
    std::vector<go::gfx_mesh> skinnedMeshes;
    //! The collective set of unskinned vertices.
    std::vector<go::gfx_model::unskinned_vertex_t> unskinnedVertices;
    //! The collective set of skinned vertices.
    std::vector<go::gfx_model::skinned_vertex_t> skinnedVertices;
    //! The collective set of mesh indices.
    std::vector<uint32_t> indices;
    //! The collective set of gfx_bones.
    go::gfx_bone_vector gfx_bones;
    /*!
        Saves the file to disk.

        \param filePath The file path.
     */
    void save(const go::file_path &filePath) const;
};

GAT_END_NAMESPACE