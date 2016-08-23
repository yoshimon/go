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


#include "go/graphics/asset_management/model_database.h"


#include "go/core/string_utility.h"
#include <DirectXMath.h>


// ================================================================================ //
// go::gfx_model_database member functions
// ================================================================================ //

go::gfx_model_database::gfx_model_database(gfx_material_database &materialDB)
    : asset_database("modeldb", "models", "model"), m_materialDB(materialDB)
{
}


void go::gfx_model_database::process_record(rapidxml::xml_node<> *n, gfx_model_definition &def)
{
    using namespace DirectX;
    using DirectX::operator+;

    // File references (GMD + GMS)
    def.filePath = path() + def.name;

    // Bounding Volumes
    auto nodeBV = n->first_node("bounding_volume");
    if(!nodeBV)
    {
        GO_THROW(data_error, "No bounding volume defined.");
    }

    // Animations
    auto nodeAnimations = n->first_node("animations");
    if(nodeAnimations)
    {
        def.animationCollection = nodeAnimations->value();

        // Dynamically set by the animation
        def.aabbMin = XMFLOAT4A(0, 0, 0, 1);
        def.aabbMax = XMFLOAT4A(0, 0, 0, 1);
        def.boundingSphere = XMFLOAT4A(0, 0, 0, 0);
    }
    else
    {
        auto nodeStaticBV = nodeBV->first_node("static");

        auto nodeAABB = nodeStaticBV->first_node("bounding_box");
        auto attAABBMin = nodeAABB->first_attribute("min");
        auto attAABBMax = nodeAABB->first_attribute("max");
        if(!nodeAABB || !attAABBMin || !attAABBMax)
        {
            GO_THROW(data_error, "Invalid AABB definition.");
        }

        // Store the AABB min max corners
        auto &&m0 = string_to_vector3f<XMFLOAT3>(attAABBMin->value());
        def.aabbMin.x = m0.x;
        def.aabbMin.y = m0.y;
        def.aabbMin.z = m0.z;
        def.aabbMin.w = 1.0f;

        auto &&m1 = string_to_vector3f<XMFLOAT3>(attAABBMax->value());
        def.aabbMax.x = m1.x;
        def.aabbMax.y = m1.y;
        def.aabbMax.z = m1.z;
        def.aabbMax.w = 1.0f;

        auto &&vAABBMin = XMLoadFloat4A(&def.aabbMin);
        auto &&vAABBMax = XMLoadFloat4A(&def.aabbMax);
        auto &&vAABBExtents = (vAABBMax - vAABBMin) * 0.5f;
        auto &&vAABBCenter = vAABBMin + vAABBExtents;

        XMStoreFloat4A(&def.aabbCenter, vAABBCenter);
        XMStoreFloat4A(&def.aabbExtents, vAABBExtents);

        // BoundingSphere
        auto &&vSphere = XMVectorSetW(vAABBCenter, XMVectorGetX(XMVector3Length(vAABBExtents)));
        XMStoreFloat4A(&def.boundingSphere, vSphere);
    }

    // Materials
    auto nodeMaterials = n->first_node("materials");
    if(nodeMaterials)
    {
        auto uniform = nodeMaterials->first_attribute("uniform");
        if(uniform)
        {
            auto mat = m_materialDB.find(uniform->value());
            def.materials.push_back(mat);
        }
        else
        {
            auto nodeMaterial = nodeMaterials->first_node("material");
            while(nodeMaterial)
            {
                // Resolve material references here
                auto mat = m_materialDB.find(nodeMaterial->value());
                if(mat)
                {
                    def.materials.push_back(mat);
                }
                else
                {
                    GO_THROW(data_error, "Invalid material reference detected.");
                }

                nodeMaterial = nodeMaterial->next_sibling(nodeMaterial->name());
            }
        }
    }
}
