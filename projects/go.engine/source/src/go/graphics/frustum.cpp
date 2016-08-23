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

#include "go/graphics/frustum.h"


using namespace DirectX;


// ================================================================================ //
// go::gfx_perspective_frustum member functions
// ================================================================================ //

void go::gfx_perspective_frustum::reset(
    DirectX::FXMVECTOR vPos, DirectX::FXMVECTOR vDir, DirectX::FXMVECTOR vUp,
    float nearClip, float farClip, float fovY, float aspectRatio)
{
    auto frustumDepth = farClip - nearClip;
    
    auto vRight = XMVector3Normalize(XMVector3Cross(vUp, vDir));

    auto vNC = vPos + vDir * nearClip;
    auto vFC = vPos + vDir * farClip;

    auto tanFovY = tan(fovY * 0.5f);
    auto halfNearHeight = nearClip * tanFovY;
    auto halfFarHeight = farClip * tanFovY;

    auto nearHeight = halfNearHeight * 2.0f;
    auto farHeight = halfFarHeight * 2.0f;

    auto halfNearWidth = aspectRatio * nearHeight;
    auto halfFarWidth = aspectRatio * farHeight;

    auto vUpHNear = vUp * halfNearHeight;
    auto vUpHFar = vUp * halfFarHeight;

    auto vRightWNear = vRight * halfNearWidth;
    auto vRightWFar = vRight * halfFarWidth;

    // 8 frustum corners
    auto vNTL = vNC + vUpHNear - vRightWNear;
    auto vFTL = vFC + vUpHFar - vRightWFar;

    auto vNTR = vNC + vUpHNear + vRightWNear;
    auto vFTR = vFC + vUpHFar + vRightWFar;

    auto vNBL = vNC - vUpHNear - vRightWNear;
    auto vFBL = vFC - vUpHFar - vRightWFar;

    auto vNBR = vNC - vUpHNear + vRightWNear;
    auto vFBR = vFC - vUpHFar + vRightWFar;

    // Store corners
    XMStoreFloat4A(&corners[0], vNTL);
    XMStoreFloat4A(&corners[1], vNTR);
    XMStoreFloat4A(&corners[2], vNBL);
    XMStoreFloat4A(&corners[3], vNBR);

    XMStoreFloat4A(&corners[4], vFTL);
    XMStoreFloat4A(&corners[5], vFTR);
    XMStoreFloat4A(&corners[6], vFBL);
    XMStoreFloat4A(&corners[7], vFBR);

    // Store frustum center
    auto vCenter = (1.0f / 8.0f) * (vNTL + vNTR + vNBL + vNBR + vFTL + vFTR + vFBL + vFBR);
    XMStoreFloat4A(&center, vCenter);

    // Compute sphere around the corner points
    // http://gsteph.blogspot.de/2010/11/minimum-bounding-sphere-for-frustum.html
    // NOTE: requires a projection with width >= height
    auto halfNearLen = halfNearWidth > halfNearHeight ? halfNearWidth : halfNearHeight;
    auto halfFarLen = halfFarWidth > halfFarHeight ? halfFarWidth : halfFarHeight;

    auto h = frustumDepth;
    auto h2 = h * h;
    auto de2 = halfFarLen * halfFarLen;
    auto ab2 = halfNearLen * halfNearLen;
    auto nom = de2 - ab2 - h2;
    auto denom = -2.0f * h;
    auto cd = nom / denom; // Distance of center from far plane
    auto centerOffset = farClip - std::max(0.0f, cd);
    auto vBoundingSphereCenter = vPos + vDir * centerOffset;
    XMStoreFloat4A(&boundingSphereCenter, vBoundingSphereCenter);
    boundingSphereRadius = XMVectorGetX(XMVector3Length(vFTL - vBoundingSphereCenter));

    // Compute cone around the corner points
    auto vToFarCornerDir = XMVector3Normalize(vFTL - vPos);
    boundingConeAngleCos = XMVectorGetX(XMVector3Dot(vDir, vToFarCornerDir));
    boundingConeAngleSin = sin(acos(boundingConeAngleCos));
    boundingConeAngleTan = boundingConeAngleSin / boundingConeAngleCos;
    
    // Compute the frustum plane normals
    auto vTopLeftEdge = vFTL - vNTL;
    auto vTopNearEdge = vNTR - vNTL;
    auto vTopDownEdge = vFBL - vFTL;
    auto vTopRightEdge = vFTR - vNTR;
    auto vBottomLeftEdge = vFBL - vNBL;

    auto vLeftPlaneN = XMVector3Normalize(XMVector3Cross(vTopDownEdge, vTopLeftEdge));
    auto vRightPlaneN = XMVector3Normalize(XMVector3Cross(vTopRightEdge, vTopDownEdge));
    auto vTopPlaneN = XMVector3Normalize(XMVector3Cross(vTopLeftEdge, vTopNearEdge));
    auto vBottomPlaneN = XMVector3Normalize(XMVector3Cross(vTopNearEdge, vBottomLeftEdge));
    auto vNearPlaneN = -vDir;
    auto vFarPlaneN = -vNearPlaneN;

    auto vLeftPlane = XMVectorInsert<0, 0, 0, 0, 1>(vLeftPlaneN, -XMVector3Dot(vLeftPlaneN, vNTL));
    auto vRightPlane = XMVectorInsert<0, 0, 0, 0, 1>(vRightPlaneN, -XMVector3Dot(vRightPlaneN, vNTR));
    auto vTopPlane = XMVectorInsert<0, 0, 0, 0, 1>(vTopPlaneN, -XMVector3Dot(vTopPlaneN, vNTL));
    auto vBottomPlane = XMVectorInsert<0, 0, 0, 0, 1>(vBottomPlaneN, -XMVector3Dot(vBottomPlaneN, vNBL));
    auto vNearPlane = XMVectorInsert<0, 0, 0, 0, 1>(vNearPlaneN, -XMVector3Dot(vNearPlaneN, vNTL));
    auto vFarPlane = XMVectorInsert<0, 0, 0, 0, 1>(vFarPlaneN, -XMVector3Dot(vFarPlaneN, vFTL));

    XMStoreFloat4A(&planes[0], vLeftPlane);
    XMStoreFloat4A(&planes[1], vRightPlane);
    XMStoreFloat4A(&planes[2], vTopPlane);
    XMStoreFloat4A(&planes[3], vBottomPlane);
    XMStoreFloat4A(&planes[4], vNearPlane);
    XMStoreFloat4A(&planes[5], vFarPlane);

    // Save basis for cone construction
    XMStoreFloat4A(&position, vPos);
    XMStoreFloat4A(&direction, vDir);

    // Save near and far planes
    this->nearClip = nearClip;
    this->farClip = farClip;
}


void go::gfx_perspective_frustum::load_planes(DirectX::XMVECTOR outPlanes[6])
{
    for(size_t i = 0; i < 6; ++i)
    {
        outPlanes[i] = XMLoadFloat4A(&planes[i]);
    }
}


void go::gfx_perspective_frustum::load_corners(DirectX::XMVECTOR outCorners[8])
{
    for(size_t i = 0; i < 8; ++i)
    {
        outCorners[i] = XMLoadFloat4A(&corners[i]);
    }
}


// ================================================================================ //
// go::gfx_orthographic_frustum member functions
// ================================================================================ //

void go::gfx_orthographic_frustum::reset(DirectX::FXMVECTOR vPos, DirectX::FXMVECTOR vDir,
    float width, float height, float depth)
{
    auto cylinderDiameter = sqrt(width * width + height * height);
    auto cylinderRadius = cylinderDiameter * 0.5f;

    auto vCylinderAxis = vDir * depth;
    XMStoreFloat4A(&boundingCylinderAxis, vCylinderAxis);
    boundingCylinderRadius = cylinderRadius;

    // Save basis for cylinder construction
    XMStoreFloat4A(&position, vPos);
}
