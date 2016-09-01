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

#include "go/world/scene.h"

#include "go/graphics/api/device.h"
#include "go/graphics/api/renderer.h"
#include "go/world/scene_manager.h"


using namespace DirectX;
using DirectX::operator+;


// ================================================================================ //
// go::scene member functions
// ================================================================================ //

go::scene::scene(scene_manager *owner, size_t entityCacheSize)
    : m_owner(owner),
    m_entityManager(this, entityCacheSize), // NOTE: the order is important here!
    m_lightManager(this),
    m_activeCamera((entity_id)-1)
{
    memset(&m_cameraProperties, 0, sizeof(m_cameraProperties));
}


void go::scene::render()
{
    GO_ASSERT(m_activeCamera != (entity_id)-1);

    // Compute rendering transforms by interpolating between the current
    // and the last known TRS transforms using the accumulator value
    m_entityManager.interpolate_transforms();

    // Always update the main camera first
    update_camera_frustum();

    // Determine the visible lights using the interpolated transforms
    m_lightManager.update_visible_lights();

    for(auto &&m : m_componentManagers)
    {
        m->handle_begin_render();
    }

    for(auto &&m : m_componentManagers)
    {
        m->handle_render();
    }

    for(auto &&m : m_componentManagers)
    {
        m->handle_end_render();
    }

    // Submit the work to the GPU
    flush_render_context();
}


void go::scene::flush_render_context()
{
    // Send everything to the renderer and reset
    m_renderContext.finalize();
    the_gfx_renderer->render_path()->render(m_renderContext, m_owner->asset_context());
    m_renderContext.reset_frame();
}


void go::scene::update()
{    
    // Swap the transform buffers
    // The current transforms become the old ones
    m_entityManager.update();

    // Now we can update all managers, add the new current transforms
    // which will allow the render function, that is executed later,
    // to interpolate between the two.
    for(auto &&m : m_componentManagers)
    {
        m->handle_begin_update();
    }

    // TODO: update physics
    // TODO: update sound

    for(auto &&m : m_componentManagers)
    {
        m->handle_update();
    }

    for(auto &&m : m_componentManagers)
    {
        m->handle_end_update();
    }
}


void go::scene::change_environment(const scene_environment &value)
{
    m_environment = value;
}


void go::scene::change_active_camera(entity_id entityID) noexcept
{
    m_activeCamera = entityID;
}


void go::scene::change_camera_properties(const gfx_camera_properties &value)
{
    m_cameraProperties = value;
}


void go::scene::update_camera_frustum()
{
    auto &&cameraID = m_activeCamera;
    auto &&cameraTransform = m_renderContext.renderPoints.main.transform;
    auto &&cameraProperties = m_renderContext.renderPoints.main.cameraProperties;

    auto &&transform = m_entityManager.interpolated_transforms()[cameraID];

    // Local camera basis
    auto &&vPos = XMLoadFloat4A(&transform.position);
    auto &&vRotationQuat = XMLoadFloat4A(&transform.rotation);
    auto &&vOrientation = XMVector3Rotate(g_XMIdentityR2, vRotationQuat);
    auto &&isLookingStraightUp = XMScalarNearEqual(abs(XMVectorGetX(XMVector3Dot(g_XMIdentityR1, vOrientation))), 1.0f, FLT_EPSILON);
    auto &&vSide = isLookingStraightUp ? g_XMIdentityR2 : XMVector3Normalize(XMVector3Cross(g_XMIdentityR1, vOrientation));
    auto &&vUp = XMVector3Normalize(XMVector3Cross(vOrientation, vSide));

    // View
    auto &&mViewMatrix = XMMatrixLookToLH(vPos, vOrientation, vUp);
    XMStoreFloat4x4(&cameraTransform.viewMatrix, mViewMatrix);

    // Projection
    auto &&fmt = the_gfx_device->back_buffer()->format();
    auto &&aspectRatio = fmt.width / (float)fmt.height;
    auto &&mProjectionMatrix = XMMatrixPerspectiveFovLH(cameraProperties.fovY, aspectRatio, cameraProperties.farClip, cameraProperties.nearClip); // NOTE: complementary Z!
    XMStoreFloat4x4(&cameraTransform.projectionMatrix, mProjectionMatrix);

    // Composite transforms
    auto &&mViewProjMatrix = mViewMatrix * mProjectionMatrix;
    XMStoreFloat4x4(&cameraTransform.viewProjectionMatrix, mViewProjMatrix);

    // Compute the frustum
    auto &&frustum = cameraTransform.frustum;
    frustum.reset(vPos, vOrientation, vUp, cameraProperties.nearClip, cameraProperties.farClip, cameraProperties.fovY, aspectRatio);
}