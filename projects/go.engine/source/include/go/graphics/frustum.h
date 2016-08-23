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
    \file graphics/frustum.h
    \brief Contains classes that represent view frusta.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/aligned.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

//! A structure, which represents a perspective view-frustum.
struct GO_API GO_SSE_ALIGN gfx_perspective_frustum : public aligned<gfx_perspective_frustum>
{
    /*!
        Resets the frustum.

        \param pos The position of the virtual camera.
        \param dir The normalized direction in which the virtual camera is looking.
        \param upVector The "up"-vector.
        \param nearClip The near-clipping plane.
        \param farClip The far-clipping plane.
        \param fovY The vertical FOV.
        \param aspectRatio The aspect-ratio (width / height).
     */
    void reset(
        DirectX::FXMVECTOR pos,
        DirectX::FXMVECTOR dir,
        DirectX::FXMVECTOR upVector,
        float nearClip,
        float farClip,
        float fovY,
        float aspectRatio);
    /*!
        Loads the frustum planes into a set of registers.

        \param outPlanes The frustum planes.
     */
    void load_planes(DirectX::XMVECTOR outPlanes[6]);
    /*!
        Loads the frustum corners into a set of registers.

        \param outCorners The frustum corners.
     */
    void load_corners(DirectX::XMVECTOR outCorners[8]);
    //! The bounding sphere center.
    DirectX::XMFLOAT4A boundingSphereCenter;
    //! The radius of the bounding sphere.
    float boundingSphereRadius;
    //! The cosine of the cone angle.
    float boundingConeAngleCos;
    //! The sine of the cone angle.
    float boundingConeAngleSin;
    //! The tan of the cone angle.
    float boundingConeAngleTan;
    //! The position of the virtual camera.
    DirectX::XMFLOAT4A position;
    //! The normalized direction of the virtual camera.
    DirectX::XMFLOAT4A direction;
    //! The frustum planes.
    DirectX::XMFLOAT4A planes[6];
    //! The near clipping plane.
    float nearClip;
    //! The far clipping plane.
    float farClip;
    /*!
        The frustum corners.

        The diagonal can be deduced from the first and last index.
     */
    DirectX::XMFLOAT4A corners[8];
    //! The center of the frustum.
    DirectX::XMFLOAT4A center;
};


//! A structure, which represents an orthographic view-frustum.
struct GO_API GO_SSE_ALIGN gfx_orthographic_frustum : public aligned<gfx_orthographic_frustum>
{
    /*!
        Resets the frustum.

        \param pos The position of the virtual camera.
        \param dir The normalized direction in which the virtual camera is looking.
        \param width The width of the frustum.
        \param height The height of the frustum.
        \param depth The depth of the frustum.
     */
    void reset(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR dir, float width, float height, float depth);
    //! The position of the virtual camera.
    DirectX::XMFLOAT4A position;
    //! The bounding cylinder axis.
    DirectX::XMFLOAT4A boundingCylinderAxis;
    //! The radius of the bounding cylinder.
    float boundingCylinderRadius;
};

GO_END_NAMESPACE