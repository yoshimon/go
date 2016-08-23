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
    \file graphics/asset_management/model_database.h
    \brief Contains a class for managing model definitions.

    \author Gokhan Ozdogan
    \version 1.0
*/

#pragma once


#include "go/core/common.h"
#include "go/core/path_utility.h"
#include "go/asset_management/asset_database.h"
#include "go/graphics/asset_management/material_database.h"
#include "go/core/aligned.h"


GO_BEGIN_NAMESPACE

//! A structure, which contains information about the different assets of a model.
struct GO_SSE_ALIGN gfx_model_definition : public aligned<gfx_model_definition>, public asset_record
{
    typedef std::vector<gfx_material_definition *> material_vector_t;
    /*!
        AABB min-corner.

        The w component is set to 1.
     */
    DirectX::XMFLOAT4A aabbMin;
    /*!
        AABB max-corner.

        The w component is set to 1.
     */
    DirectX::XMFLOAT4A aabbMax;
    //! The AABB center.
    DirectX::XMFLOAT4A aabbCenter;
    //! The AABB extents.
    DirectX::XMFLOAT4A aabbExtents;
    //! The bounding-sphere.
    DirectX::XMFLOAT4A boundingSphere;
    //! The name of the animation collection.
    file_path animationCollection;
    //! The mesh file.
    file_path filePath;
    //! A list of materials for each submesh.
    material_vector_t materials;
};


//! A class, which contains model definitions.
class GO_API gfx_model_database : public asset_database<gfx_model_definition>
{
    GO_DECLARE_NON_COPYABLE(gfx_model_database)
public:
    /*!
        Constructor.

        \param materialDB The material database.
     */
    gfx_model_database(gfx_material_database &materialDB);
protected:
    void process_record(rapidxml::xml_node<> *n, gfx_model_definition &def) override;
private:
    //! The material database.
    gfx_material_database &m_materialDB;
};

GO_END_NAMESPACE
