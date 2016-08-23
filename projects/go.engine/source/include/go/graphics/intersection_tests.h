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
    \file graphics/intersection_tests.h
    \brief Contains classes for performing intersection tests.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"
#include <DirectXCollision.h>


GO_BEGIN_NAMESPACE

//! Enumeration of possible intersection results.
enum class intersection_test_result
{
    //! The intersection is disjoint.
    disjoint,
    //! The first volume contains the second volume.
    contains,
    //! The two volumes intersect.
    intersects
};


//! A helper structure, which contains functions for quick intersection tests.
struct intersection_tests
{
    typedef intersection_test_result result_t;
    /*!
        Returns whether a sphere intersects or contains another sphere.

        \param center0 The center of the first sphere.
        \param radius0 The radius of the first sphere.
        \param center1 The center of the second sphere.
        \param radius1 The radius of the second sphere.
        */
    static inline bool sphere_sphere(
        DirectX::FXMVECTOR center0, float radius0,
        DirectX::HXMVECTOR center1, float radius1)
    {
        using namespace DirectX;
        auto sumRadii = radius0 + radius1;
        return XMVectorGetX(XMVector3LengthSq(center0 - center1)) < (sumRadii * sumRadii);
    }
    /*!
        Returns the intersection test between a cone and a sphere.

        \param coneApex The apex of the cone.
        \param coneDir The cone direction.
        \param coneCos The cosine of the cone angle.
        \param coneSin The sine of the cone angle.
        \param sphereCenter The center of the sphere.
        \param sphereRadius The radius of the sphere.
        */
    static inline result_t cone_sphere(
        DirectX::FXMVECTOR coneApex, DirectX::FXMVECTOR coneDir, float coneCos, float coneSin,
        DirectX::FXMVECTOR sphereCenter, float sphereRadius)
    {
        using namespace DirectX;
        auto v = sphereCenter - coneApex;
        auto a = XMVectorGetX(XMVector3Dot(v, coneDir));
        auto x = coneCos * std::sqrt(XMVectorGetX(XMVector3Dot(v, v)) - a * a) - a * coneSin;
        if(std::abs(x) > sphereRadius)
        {
            return x < 0.f ? result_t::contains : result_t::disjoint;
        }
        else
        {
            return result_t::intersects;
        }
    }
    /*!
        Returns the intersection test between a cylinder and a sphere.

        \param cylinderBase The cylinder base.
        \param cylinderAxis The cylinder axis.
        \param cylinderRadius The cylinder radius.
        \param sphereCenter The center of the sphere.
        \param sphereRadius The sphere radius.
     */
    static inline bool cylinder_sphere(
        DirectX::FXMVECTOR cylinderBase, DirectX::FXMVECTOR cylinderAxis, float cylinderRadius,
        DirectX::FXMVECTOR sphereCenter, float sphereRadius)
    {
        using namespace DirectX;
        // XMVector3LinePointDistance
        auto vAxisLenSq = XMVector3LengthSq(cylinderAxis); // TODO: precompute
        auto vToPoint = sphereCenter - cylinderBase;
        auto vProjScale = XMVector3Dot(vToPoint, cylinderAxis);
        vProjScale = XMVectorDivide(vProjScale, vAxisLenSq);

        auto vDistanceVector = XMVectorMultiply(cylinderAxis, vProjScale);
        vDistanceVector = vToPoint - vDistanceVector;

        auto distance = XMVectorGetX(XMVector3LengthEst(vDistanceVector));
        return distance < (cylinderRadius + sphereRadius);
    }
    /*!
        Returns the intersection test between a cone and another cone.
     
        \param cone0Apex The apex of the first cone.
        \param cone0Cos The angle of the first cone.
        \param cone1Cos The apex of the second cone.
        \param cone1Cos The angle of the second cone.
     */
    static inline bool cone_cone(
        DirectX::FXMVECTOR cone0Apex, float cone0Cos,
        DirectX::FXMVECTOR cone1Apex, float cone1Cos)
    {
        GO_ASSERT(false);
        return false;
    }
    /*!
        Returns the intersection between a frustum and a sphere.

        \param vPlanes The frustum planes.
        \param vCenter The center of the sphere.
        \param radius The radius of the sphere.

        \return The intersection test result.
     */
    static inline result_t frustum_sphere(
        DirectX::FXMVECTOR vPlanes[6],
        DirectX::FXMVECTOR vCenter, float radius)
    {
        using namespace DirectX;

        // Load the sphere.
        auto vRadius = XMVectorReplicate(radius);

        XMVECTOR Outside, Inside;

        // Test against each plane.
        DirectX::Internal::FastIntersectSpherePlane(vCenter, vRadius, vPlanes[0], Outside, Inside);

        auto AnyOutside = Outside;
        auto AllInside = Inside;

        DirectX::Internal::FastIntersectSpherePlane(vCenter, vRadius, vPlanes[1], Outside, Inside);
        AnyOutside = XMVectorOrInt(AnyOutside, Outside);
        AllInside = XMVectorAndInt(AllInside, Inside);

        DirectX::Internal::FastIntersectSpherePlane(vCenter, vRadius, vPlanes[2], Outside, Inside);
        AnyOutside = XMVectorOrInt(AnyOutside, Outside);
        AllInside = XMVectorAndInt(AllInside, Inside);

        DirectX::Internal::FastIntersectSpherePlane(vCenter, vRadius, vPlanes[3], Outside, Inside);
        AnyOutside = XMVectorOrInt(AnyOutside, Outside);
        AllInside = XMVectorAndInt(AllInside, Inside);

        DirectX::Internal::FastIntersectSpherePlane(vCenter, vRadius, vPlanes[4], Outside, Inside);
        AnyOutside = XMVectorOrInt(AnyOutside, Outside);
        AllInside = XMVectorAndInt(AllInside, Inside);

        DirectX::Internal::FastIntersectSpherePlane(vCenter, vRadius, vPlanes[5], Outside, Inside);
        AnyOutside = XMVectorOrInt(AnyOutside, Outside);
        AllInside = XMVectorAndInt(AllInside, Inside);

        // If the sphere is outside any plane it is outside.
        if(XMVector4EqualInt(AnyOutside, XMVectorTrueInt()))
        {
            return intersection_test_result::disjoint;
        }

        // If the sphere is inside all planes it is inside.
        if(XMVector4EqualInt(AllInside, XMVectorTrueInt()))
        {
            return intersection_test_result::contains;
        }

        // The sphere is not inside all planes or outside a plane, it may intersect.
        return intersection_test_result::intersects;
    }
    /*!
        Returns the intersection between a frustum and an OBB.

        \param vPlanes The frustum planes.
        \param vCenter The center of the sphere.
        \param vExtents The extents of the OBB.
        \param vAxis0 The local box basis.
        \param vAxis1 The local box basis.
        \param vAxis2 The local box basis.

        \return The intersection test result.
     */
    static inline intersection_test_result frustum_obb(
        DirectX::FXMVECTOR vPlanes[6],
        DirectX::FXMVECTOR vCenter, DirectX::FXMVECTOR vExtents,
        DirectX::FXMVECTOR *vAxis)
    {
        using namespace DirectX;

        XMVECTOR Outside, Inside;

        // Test against each plane.
        DirectX::Internal::FastIntersectOrientedBoxPlane(vCenter, vExtents, vAxis[0], vAxis[1], vAxis[2], vPlanes[0], Outside, Inside);

        auto AnyOutside = Outside;
        auto AllInside = Inside;

        DirectX::Internal::FastIntersectOrientedBoxPlane(vCenter, vExtents, vAxis[0], vAxis[1], vAxis[2], vPlanes[1], Outside, Inside);
        AnyOutside = XMVectorOrInt(AnyOutside, Outside);
        AllInside = XMVectorAndInt(AllInside, Inside);

        DirectX::Internal::FastIntersectOrientedBoxPlane(vCenter, vExtents, vAxis[0], vAxis[1], vAxis[2], vPlanes[2], Outside, Inside);
        AnyOutside = XMVectorOrInt(AnyOutside, Outside);
        AllInside = XMVectorAndInt(AllInside, Inside);

        DirectX::Internal::FastIntersectOrientedBoxPlane(vCenter, vExtents, vAxis[0], vAxis[1], vAxis[2], vPlanes[3], Outside, Inside);
        AnyOutside = XMVectorOrInt(AnyOutside, Outside);
        AllInside = XMVectorAndInt(AllInside, Inside);

        DirectX::Internal::FastIntersectOrientedBoxPlane(vCenter, vExtents, vAxis[0], vAxis[1], vAxis[2], vPlanes[4], Outside, Inside);
        AnyOutside = XMVectorOrInt(AnyOutside, Outside);
        AllInside = XMVectorAndInt(AllInside, Inside);

        DirectX::Internal::FastIntersectOrientedBoxPlane(vCenter, vExtents, vAxis[0], vAxis[1], vAxis[2], vPlanes[5], Outside, Inside);
        AnyOutside = XMVectorOrInt(AnyOutside, Outside);
        AllInside = XMVectorAndInt(AllInside, Inside);

        // If the box is outside any plane it is outside.
        if(XMVector4EqualInt(AnyOutside, XMVectorTrueInt()))
        {
            return intersection_test_result::disjoint;
        }

        // If the box is inside all planes it is inside.
        if(XMVector4EqualInt(AllInside, XMVectorTrueInt()))
        {
            return intersection_test_result::contains;
        }

        // The box is not inside all planes or outside a plane, it may intersect.
        return intersection_test_result::intersects;
    }
};

GO_END_NAMESPACE
