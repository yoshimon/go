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

#include "go/world/light_manager.h"

#include "go/world/scene.h"


using namespace DirectX;
using DirectX::operator+;


// ================================================================================ //
// go::light_manager member functions
// ================================================================================ //

go::light_manager::light_manager(scene *owner)
    : m_owner(owner)
{
    auto &&maxEntityCount = owner->entity_manager()->capacity();

    m_entityToSphericalLightIndex.resize(maxEntityCount, (entity_id)-1);
    m_sphericalLights.reserve(maxEntityCount);

    m_entityToDiskLightIndex.resize(maxEntityCount, (entity_id)-1);
    m_diskLights.reserve(maxEntityCount);
}


void go::light_manager::update_visible_lights()
{
    cull_shadow_casting_spherical_lights();
    cull_shadow_casting_disk_lights();
}


void go::light_manager::attach_spherical_light(entity_id entityID, const spherical_light_desc &desc)
{
    if(has_spherical_light(entityID))
    {
        return;
    }

    m_entityToSphericalLightIndex[entityID] = m_sphericalLights.size();
    m_sphericalLights.push_back(spherical_light_data{ entityID, desc });
}


void go::light_manager::detach_spherical_light(entity_id entityID)
{
    auto &&lightIndex = m_entityToSphericalLightIndex[entityID];
    m_entityToSphericalLightIndex[entityID] = (entity_id)-1;
    go::swap_erase(m_sphericalLights, lightIndex);
}


const go::spherical_light_data &go::light_manager::find_spherical_light(entity_id entityID) const noexcept
{
    return m_sphericalLights[m_entityToSphericalLightIndex[entityID]];
}


bool go::light_manager::has_spherical_light(entity_id entityID) const noexcept
{
    return m_entityToSphericalLightIndex[entityID] != (entity_id)-1;
}


void go::light_manager::attach_disk_light(entity_id entityID, const disk_light_desc &desc)
{
    if(has_disk_light(entityID))
    {
        return;
    }

    m_entityToDiskLightIndex[entityID] = m_diskLights.size();
    m_diskLights.push_back(disk_light_data{ entityID, desc });
}


void go::light_manager::detach_disk_light(entity_id entityID)
{
    auto &&lightIndex = m_entityToDiskLightIndex[entityID];
    m_entityToDiskLightIndex[entityID] = (entity_id)-1;
    go::swap_erase(m_diskLights, lightIndex);
}


const go::disk_light_data &go::light_manager::find_disk_light(entity_id entityID) const noexcept
{
    return m_diskLights[m_entityToDiskLightIndex[entityID]];
}


bool go::light_manager::has_disk_light(entity_id entityID) const noexcept
{
    return m_entityToDiskLightIndex[entityID] != (entity_id)-1;
}


void go::light_manager::update_directional_light_frustum()
{
    auto &&renderContext = m_owner->m_renderContext;
    auto &&entityMgr = m_owner->entity_manager();
    auto &&cameraID = m_owner->active_camera();
    auto &&cameraFrustum = renderContext.renderPoints.main.transform.frustum;

    auto &&transform = entityMgr->interpolated_transforms()[cameraID];

    // Load camera info
    auto &&vCamPos = XMLoadFloat4A(&transform.position);
    auto &&vCamRotation = XMLoadFloat4A(&transform.rotation);
    auto &&vCamDir = XMVector3Rotate(XMLoadFloat4A(&transform.rotation), vCamRotation);

    // Figure out the light basis vectors
    auto &&vSunDir = XMLoadFloat3(&renderContext.sky.sunDirection);
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
    auto &&lightSplit = renderContext.renderPoints.directionalLight[0];

    auto &&lightTransforms = lightSplit.transform;
    XMStoreFloat4x4(&lightTransforms.projectionMatrix, mLightProj);
    XMStoreFloat4x4(&lightTransforms.viewMatrix, mLightView);
    XMStoreFloat4x4(&lightTransforms.viewProjectionMatrix, mLightViewProj);

    // Update the light frustum
    auto &&frustum = lightSplit.transform.frustum;
    frustum.reset(vLightPos, vSunDir, bounds, bounds, lightFarClip);
}


void go::light_manager::cull_spherical_proxy_lights()
{

}


void go::light_manager::cull_shadow_casting_spherical_lights()
{
    auto &&renderContext = m_owner->m_renderContext;
    auto &&entityMgr = m_owner->entity_manager();
    auto &&transforms = entityMgr->interpolated_transforms();
    auto &&activeCamera = m_owner->active_camera();
    
    // Camera position
    auto vCamPos = XMLoadFloat4A(&transforms[activeCamera].position);

    // Camera frustum...
    auto &camFrustum = renderContext.renderPoints.main.transform.frustum;

    // as a sphere
    auto vCamFrustumBoundingSphere = XMLoadFloat4A(&camFrustum.boundingSphereCenter);
    auto camFrustumBoundingSphereRadius = camFrustum.boundingSphereRadius;

    // and as a cone
    auto vCamFrustumConeApex = XMLoadFloat4A(&camFrustum.position);
    auto vCamFrustumConeDir = XMLoadFloat4A(&camFrustum.direction);
    auto camFrustumConeCos = camFrustum.boundingConeAngleCos;
    auto camFrustumConeSin = camFrustum.boundingConeAngleSin;
    XMVECTOR vCamFrustumPlanes[6];
    camFrustum.load_planes(vCamFrustumPlanes);

    /*
    // Cull all lights based on proxy geometry
    cull_spherical_proxy_lights(renderContext.lights.spherical.shapes, renderContext.lights.visible.spherical, renderContext.lights.shadowCasting.spherical);

    // Setup the transforms
    // NOTE: don't change this! The shaders expect this order
    XMVECTOR vDir[] =
    {
        { -1,  0,  0, 0 },
        { 1,  0,  0, 0 },
        { 0, -1,  0, 0 },
        { 0,  1,  0, 0 },
        { 0,  0, -1, 0 },
        { 0,  0,  1, 0 }
    };
    XMVECTOR vUpDir[] =
    {
        { 0, 1,  0, 0 },
        { 0, 1,  0, 0 },
        { 1, 0,  0, 0 },
        { 1, 0,  0, 0 },
        { 0, 1,  0, 0 },
        { 0, 1,  0, 0 }
    };

    // Omni-directional light constants
    const auto kFovY = XM_PIDIV2;
    const auto kAspectRatio = 1.f;
    const auto kNearClip = 0.2f;
    const auto kMinFarClip = 0.001f;

    size_t sclIndex = 0;
    for(auto scl : shadowCastingLights)
    {
        auto &&renderPoint = renderPoints[sclIndex];
        auto &&shape = lightShapes[scl];
        auto &&parameters = lightParams[scl];

        auto &faceTransforms = renderPoint.faceTransforms;
        auto &isFaceActive = renderPoint.isFaceActive;

        auto vPos = XMVectorSet(shape.center[0], shape.center[1], shape.center[2], 1.f);
        auto farClip = std::max(kMinFarClip, shape.radius);

        // Calculate subset of the 6 frustums that is actually visible
        renderPoint.id = scl;
        renderPoint.isActive = false;
        renderPoint.kind = go::gfx_render_point_kind::spherical_light;
        for(auto i = 0; i < 6; ++i)
        {
            // Update point light frustum
            auto view = DirectX::XMMatrixLookToLH(vPos, vDir[i], vUpDir[i]);
            auto proj = DirectX::XMMatrixPerspectiveFovLH(kFovY, kAspectRatio, kNearClip, farClip);
            auto viewProj = view * proj;
            XMStoreFloat4x4(&faceTransforms[i].viewMatrix, view);
            XMStoreFloat4x4(&faceTransforms[i].projectionMatrix, proj);
            XMStoreFloat4x4(&faceTransforms[i].viewProjectionMatrix, viewProj);

            auto &frustum = faceTransforms[i].frustum;
            frustum.reset(vPos, vDir[i], vUpDir[i], kNearClip, farClip, kFovY, kAspectRatio);

            // TODO: per-face frustum culling
            // go::intersection_tests::frustum_frustum
            isFaceActive[i] = true;
            renderPoint.isActive |= isFaceActive[i];
        }

        // TODO: calculate importance based on projected area

        // Point this light to the correct shadow caster slot
        parameters.shadowCasterIndex = (float)sclIndex;

        ++sclIndex;
    }
    */
}


void go::light_manager::cull_shadow_casting_disk_lights()
{

}