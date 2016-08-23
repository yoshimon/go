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
    \file filesystem/views/native/gmd_file.h
    \brief Contains a class for opening a Go Model Definition (GMD) file view.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/path_utility.h"
#include "go/platform/windows.h"
#include "go/graphics/model.h"


GO_BEGIN_NAMESPACE

//! A class, which represents a view of a GMD file.
class GO_API gmd_file
{
    GO_DECLARE_NON_COPYABLE(gmd_file)
public:
    //! GMD header file magic number.
    static const uint32_t kMagic = 0x00000001;
    //! An enumeration of possible vertex types.
    enum class vertex_type_t : int32_t
    {
        //! An unskinned vertex for static geometry.
        unskinned = 0,
        //! A skinned vertex for animated geometry.
        skinned
    };
    //! A structure, which represents the file header.
    struct header_t
    {
        //! Magic number.
        uint32_t magic;
        //! The file version.
        uint32_t version;
        //! The number of unskinned meshes in the model.
        uint32_t numUnskinnedMeshes;
        //! The number of skinned meshes in the model.
        uint32_t numSkinnedMeshes;
        //! The number of unskinned vertices in the model.
        uint32_t numUnskinnedVertices;
        //! The number of skinned vertices in the model.
        uint32_t numSkinnedVertices;
        //! The number of indices in the model.
        uint32_t numIndices;
        //! The number of gfx_bones.
        uint32_t numBones;
    };
	typedef uint32_t index32_t;
	typedef uint16_t index16_t;
    //! A structure, which represents a submesh in the model.
    struct submesh_t
    {
        //! The index offset.
        uint32_t startIndex;
        //! The number of indices.
        uint32_t numIndices;
        //! The vertex offset.
        uint32_t startVertex;
        //! The number of vertices.
        uint32_t numVertices;
    };
    typedef gfx_model::unskinned_vertex_t unskinned_vertex_t;
    typedef gfx_model::skinned_vertex_t skinned_vertex_t;
public:
    /*!
        Loads a model from file in memory.

        \param data Pointer to the file data.
        \param dataSize Size of the data pointed to by the data pointer.
        \param out The model to output to.
     */
    static bool load(const uint8_t *data, size_t dataSize, gfx_model &out);
    //! \overload
    static bool load(const file_path &fileName, gfx_model &out);
};

GO_END_NAMESPACE
