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
    m_activeCamera((entity_id)-1),
    m_isRendering(false)
{
    memset(&m_cameraProperties, 0, sizeof(m_cameraProperties));
}


void go::scene::render()
{
    if(m_isRendering || m_activeCamera == (entity_id)-1)
    {
        return;
    }

    m_isRendering = true;

    // Always update the main camera first
    update_camera_frustum();

    // Figure out which lights should be casting shadows
    determine_shadow_casting_lights();

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

    m_isRendering = false;
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
    for(auto &&m : m_componentManagers)
    {
        m->handle_begin_update();
    }

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

    auto &&transform = m_entityManager.transforms()[cameraID];

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


void go::scene::determine_shadow_casting_lights()
{
    auto &&cameraID = m_activeCamera;
    auto &&cameraFrustum = m_renderContext.renderPoints.main.transform.frustum;

    auto &&transform = m_entityManager.transforms()[cameraID];

    // Load camera info
    auto &&vCamPos = XMLoadFloat4A(&transform.position);
    auto &&vCamRotation = XMLoadFloat4A(&transform.rotation);
    auto &&vCamDir = XMVector3Rotate(XMLoadFloat4A(&transform.rotation), vCamRotation);

    // Figure out the light basis vectors
    auto &&vSunDir = XMLoadFloat3(&m_renderContext.sky.sunDirection);
    auto &&vToSunDir = -vSunDir;
    auto &&isSunDirUpParallel = XMScalarNearEqual(abs(XMVectorGetX(XMVector3Dot(g_XMIdentityR1, vSunDir))), 1.0f, FLT_EPSILON);
    auto &&vSunSide = isSunDirUpParallel ? g_XMIdentityR2 : XMVector3Normalize(XMVector3Cross(g_XMIdentityR1, vSunDir));
    auto &&vSunUp = XMVector3Normalize(XMVector3Cross(vSunDir, vSunSide));

    // Load the camera frustum bounding sphere
    auto &&vFrustumBSCenter = XMLoadFloat4A(&cameraFrustum.boundingSphereCenter);
    auto &&frustumBSRadius = std::ceil(cameraFrustum.boundingSphereRadius * 16.0f) / 16.0f; // NOTE: round up to prevent precision problems from recalc
    auto &&frustumBSDiameter = frustumBSRadius * 2.0f;

    // Push the light back
    const auto lightNearClip = 1.0f;
    const auto extraBackupDist = 1000.0f; // TODO: for culling
    const auto backupDist = extraBackupDist + lightNearClip + frustumBSRadius;
    auto &&vLightPos = vFrustumBSCenter + vToSunDir * backupDist;

    // Build the light view matrix
    auto &&mLightView = DirectX::XMMatrixLookAtLH(vLightPos, vFrustumBSCenter, vSunUp);

    // Build the projection matrix
    auto &&bounds = frustumBSDiameter;
    auto &&lightFarClip = backupDist + frustumBSRadius;
    auto &&mLightProj = XMMatrixOrthographicLH(bounds, bounds, lightNearClip, lightFarClip);

    // Combine them
    auto &&mLightViewProj = mLightView * mLightProj;

    // Snap texels
    auto &&halfShadowMapSize = DFX_DIRECTIONAL_LIGHT_SHADOW_MAP_SIZE * 0.5f;
    auto &&originLS = XMVector3TransformCoord(g_XMIdentityR3, mLightViewProj) / halfShadowMapSize;
    auto &&roundedOriginLS = XMVectorRound(originLS);
    auto &&rounding = (roundedOriginLS - originLS) / halfShadowMapSize;

    auto &&mRoundMatrix = DirectX::XMMatrixTranslationFromVector(rounding);

    mLightViewProj *= mRoundMatrix;

    // Save results
    auto &&lightSplit = m_renderContext.renderPoints.directionalLight[0];

    auto &&lightTransforms = lightSplit.transform;
    XMStoreFloat4x4(&lightTransforms.projectionMatrix, mLightProj);
    XMStoreFloat4x4(&lightTransforms.viewMatrix, mLightView);
    XMStoreFloat4x4(&lightTransforms.viewProjectionMatrix, mLightViewProj);

    // Update the light frustum
    auto &&frustum = lightSplit.transform.frustum;
    frustum.reset(vLightPos, vSunDir, bounds, bounds, lightFarClip);
}