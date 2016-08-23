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
#include "gat/files/gmd_file.h"
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

/*!
    A class, which represents a Unity scene.
 */
class unity_model
{
public:
    typedef std::vector<go::gfx_model::unskinned_vertex_t> unskinned_vertices_t;
    typedef std::vector<go::gfx_model::skinned_vertex_t> skinned_vertices_t;
    typedef std::vector<uint32_t> indices_t;
    typedef std::vector<go::gfx_mesh> meshes_t;
    typedef std::vector<std::string> material_names_t;
public:
    /*!
        Constructor.
     */
    unity_model() = default;
    /*!
        \overload
     */
    unity_model(const go::file_path &filePath);
public:
    /*!
        Loads the scene from a file.
     */
    void load(const go::file_path &filePath);
    /*!
        Returns the unskinned vertex list.

        \return The unskinned vertex list.
     */
    const unskinned_vertices_t &unskinned_vertices() const noexcept;
    /*!
        Returns the skinned vertex list.

        \return The skinned vertex list.
     */
    const skinned_vertices_t &skinned_vertices() const noexcept;
    /*!
        Returns the index vector.

        \return The index vector.
     */
    const indices_t &indices() const noexcept;
    /*!
        Returns the submesh vector.

        \return The submesh vector.
     */
    const meshes_t &unskinned_meshes() const noexcept;
    /*!
        Returns the submesh vector.

        \return The submesh vector.
     */
    const meshes_t &skinned_meshes() const noexcept;
    /*!
        Returns the material vector.

        \return The material vector.
     */
    const material_names_t &material_names() const noexcept;
private:
    /*!
        The unskinned vertices.
     */
    unskinned_vertices_t m_unskinnedVertices;
    /*!
        The skinned vertices.
     */
    skinned_vertices_t m_skinnedVertices;
    /*!
        The indices.
     */
    indices_t m_indices;
    /*!
        The submeshes.
     */
    meshes_t m_unskinnedMeshes;
    /*!
        The skinned submeshes.
     */
    meshes_t m_skinnedMeshes;
    /*!
        The material vector.
     */
    material_names_t m_materialNames;
};

GAT_END_NAMESPACE