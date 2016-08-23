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

#include "go/graphics/scene.h"

#include "go/graphics/api/device.h"
#include "go/graphics/api/renderer.h"
#include "go/graphics/intersection_tests.h"
#include "go/graphics/scene_manager.h"
#include "tbb/parallel_for.h"


using namespace DirectX;
using DirectX::operator+;


// ================================================================================ //
// Helper functions
// ================================================================================ //

/*!
    Returns the index of the farthest light. 

    \tparam T The light buffer type.

    \param lights The light buffer.
    \param lightDistanceCache The light distance cache.

    \return The index of the farthest light.
 */
template<class T>
size_t find_farthest_light(const T &lights, float *lightDistanceCache)
{
    size_t maxDistIndex = -1;
    auto maxDist = 0.f;

    GO_FOR_EACH_INDEX(current, lights)
    {
        auto &l = *current.iterator;
        if(lightDistanceCache[l] > maxDist)
        {
            maxDist = lightDistanceCache[l];
            maxDistIndex = current.index;
        }
    }

    return maxDistIndex;
}


/*!
    Returns the index of the farthest light.

    \tparam T The light buffer type.

    \param lights The light buffer.
    \param lightDistanceCache The light distance cache.
    \param newLightIndex The new light index.

    \return The index of the farthest light.
 */
template<class T>
void replace_farthest_light(T &lights, float *lightDistanceCache, size_t newLightIndex)
{
    auto farthestLightIndex = find_farthest_light(lights, lightDistanceCache);
    if(farthestLightIndex != -1)
    {
        lights[farthestLightIndex] = newLightIndex;
    }
}


/*!
    Updates a set of spherical light sources with their new positions based on the current entity state.

    \param entityData The entity data.
    \param lights The lights.
    \param lightShapes The light shapes.
 */
void update_spherical_lights(
    const go::gfx_scene::entity_data_t &entityData,
    const go::gfx_entity_id_vector &lights,
    go::gfx_spherical_lights &sphericalLights)
{
    auto &positions = entityData.positions;
    auto &scales = entityData.scales;

	auto &lightShapes = sphericalLights.shapes;

    go::gfx_entity_id lightIndex = 0;
    for(auto entityIndex : lights)
    {
        // Copy light position & radius
        auto &shape = lightShapes[lightIndex];
		go::copy_float3(shape.center, positions[entityIndex]);
		shape.radius = scales[entityIndex];
		++lightIndex;
    }
}


/*!
	Updates a set of spherical light sources with their new positions based on the current entity state.

	\param entityData The entity data.
	\param lights The lights.
	\param lightShapes The light shapes.
 */
void update_disk_lights(
	const go::gfx_scene::entity_data_t &entityData,
	const go::gfx_entity_id_vector &lights,
	go::gfx_scene::disk_light_data_vector_t &diskLightData,
	go::gfx_disk_lights &diskLights)
{
	auto &positions = entityData.positions;
	auto &orientations = entityData.orientations;
	auto &scales = entityData.scales;

	auto &lightShapes = diskLights.shapes;
	auto &lightParameters = diskLights.parameters;

	go::gfx_entity_id lightIndex = 0;
	for(auto entityIndex : lights)
	{
		// Copy light position & radius
		auto &shape = lightShapes[lightIndex];
		auto &params = lightParameters[lightIndex];
		go::copy_float3(shape.center, positions[entityIndex]);
		shape.radius = scales[entityIndex];
		go::copy_float3(params.directionWS, orientations[entityIndex]);

		auto &data = diskLightData[lightIndex];

		// Virtual spot light position
		auto vLightPosWS = XMLoadFloat3((XMFLOAT3 *)&shape.center);
		auto vLightDirWS = XMLoadFloat3((XMFLOAT3 *)&params.directionWS);
		auto vVirtualPosWS = vLightPosWS - vLightDirWS * (params.radiusWS * data.rcpTanHalfOuterAngle);
		XMStoreFloat3((XMFLOAT3 *)params.virtualPositionWS, vVirtualPosWS);

		// Update attenuation data
		params.angleScale = data.angleScale;
		params.angleBias = data.angleBias;

		++lightIndex;
	}
}


/*!
	Performs coarse culling of proxy-sphere-shaped shadow casting lights.

	...
 */
template<class LightShapes>
void cull_sphere_proxy_shadow_casting_lights(
	size_t maxNumShadowCastingLights,
    FXMVECTOR vCamFrustumBoundingSphere, float camFrustumBoundingSphereRadius,
    GXMVECTOR vCamFrustumConeApex, HXMVECTOR vCamFrustumConeDir, float camFrustumConeCos, float camFrustumConeSin,
    XMVECTOR *vCamFrustumPlanes,
    XMVECTOR vCamPos,
    const go::gfx_scene::float4_vector_t &entityPositions,
	const go::gfx_scene::shadow_caster_flag_vector_t &shadowCasterFlags,
    const go::gfx_entity_id_vector &entityLights,
	const LightShapes &lightShapes,
	go::gfx_entity_id_vector &visibleLights,
	go::index_vector &shadowCastingLights)
{
    // Light distance cache
    auto lightDistCache = (float *)alloca(sizeof(float) * entityLights.size());
    
    // Loop variable
    size_t lightIndex = 0;
    
    // Go through each entity that was marked as a light
    for(auto entityIndex : entityLights)
    {
        // Fetch light position & radius
        auto &shape = lightShapes[lightIndex];
        auto &lightPos = entityPositions[entityIndex];
        auto vLightPos = XMLoadFloat4A(&lightPos);

        // Make sure the light is at least partially inside the view-frustum
        if(go::intersection_tests::sphere_sphere(vCamFrustumBoundingSphere, camFrustumBoundingSphereRadius, vLightPos, shape.radius) &&
           go::intersection_tests::cone_sphere(vCamFrustumConeApex, vCamFrustumConeDir, camFrustumConeCos, camFrustumConeSin, vLightPos, shape.radius) != go::intersection_tests::result_t::disjoint &&
		   go::intersection_tests::frustum_sphere(vCamFrustumPlanes, vLightPos, shape.radius) != go::intersection_tests::result_t::disjoint)
        {
            // Determine distance to viewer
			auto lightDistanceSq = XMVectorGetX(XMVector3LengthSq(vLightPos - vCamPos));
            auto distance = std::max(0.f, lightDistanceSq - shape.radius);

            // Cache the distance for this light
            lightDistCache[lightIndex] = distance;

            // Add light if we have room
            if(visibleLights.size() < GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE)
            {
                visibleLights.push_back(lightIndex);
            }
            else
            {
                replace_farthest_light(visibleLights, lightDistCache, lightIndex);
            }

            // Add new shadow-casting light if we have room
            if(shadowCasterFlags[lightIndex])
            {
                if(shadowCastingLights.size() < maxNumShadowCastingLights)
                {
                    shadowCastingLights.push_back(lightIndex);
                }
                else
                {
                    replace_farthest_light(shadowCastingLights, lightDistCache, lightIndex);
                }
            }
        }

        ++lightIndex;
    }
}


/*!
	Performs coarse culling of shadow-casting point lights.
	...
 */
template<class LightParams>
void update_shadow_casting_point_lights(
	FXMVECTOR vCamFrustumBoundingSphere, float camFrustumBoundingSphereRadius,
	GXMVECTOR vCamFrustumConeApex, HXMVECTOR vCamFrustumConeDir, float camFrustumConeCos, float camFrustumConeSin,
	XMVECTOR *vCamFrustumPlanes,
	XMVECTOR vCamPos,
	const go::gfx_scene::float4_vector_t &entityPositions,
	const go::gfx_scene::shadow_caster_flag_vector_t &shadowCasterFlags,
	const go::gfx_entity_id_vector &entityLights,
	go::gfx_cube_render_point *renderPoints,
	const go::gfx_spherical_lights::shape_array_t &lightShapes,
	LightParams &lightParams,
	go::gfx_entity_id_vector &visibleLights,
	go::index_vector &shadowCastingLights)
{
	// Cull the lights
	cull_sphere_proxy_shadow_casting_lights(
		DFX_MAX_NUM_SHADOW_CASTING_POINT_LIGHTS,
        vCamFrustumBoundingSphere, camFrustumBoundingSphereRadius,
        vCamFrustumConeApex, vCamFrustumConeDir, camFrustumConeCos, camFrustumConeSin,
        vCamFrustumPlanes,
        vCamPos,
        entityPositions,
        shadowCasterFlags,
        entityLights,
        lightShapes,
        visibleLights,
        shadowCastingLights);

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
}


/*!
	Performs coarse culling of shadow-casting spot lights.

 */
template<class LightParams>
void update_shadow_casting_spot_lights(
	FXMVECTOR vCamFrustumBoundingSphere, float camFrustumBoundingSphereRadius,
	GXMVECTOR vCamFrustumConeApex, HXMVECTOR vCamFrustumConeDir, float camFrustumConeCos, float camFrustumConeSin,
	XMVECTOR *vCamFrustumPlanes,
	XMVECTOR vCamPos,
	const go::gfx_scene::float4_vector_t &entityPositions,
	const go::gfx_scene::float4_vector_t &entityOrientations,
	const go::gfx_scene::shadow_caster_flag_vector_t &shadowCasterFlags,
	const go::gfx_entity_id_vector &entityLights,
	go::gfx_perspective_render_point *renderPoints,
	const go::gfx_disk_lights::shape_array_t &lightShapes,
	LightParams &lightParams,
	const go::gfx_scene::disk_light_data_vector_t &lightData,
	go::index_vector &visibleLights,
	go::index_vector &shadowCastingLights)
{
	// Cull the lights
	cull_sphere_proxy_shadow_casting_lights(
		DFX_MAX_NUM_SHADOW_CASTING_SPOT_LIGHTS,
		vCamFrustumBoundingSphere, camFrustumBoundingSphereRadius,
		vCamFrustumConeApex, vCamFrustumConeDir, camFrustumConeCos, camFrustumConeSin,
		vCamFrustumPlanes,
		vCamPos,
		entityPositions,
		shadowCasterFlags,
		entityLights,
		lightShapes,
		visibleLights,
		shadowCastingLights);

	// Setup the transforms
	auto kNearClip = 0.2f;
	auto kMinFarClip = kNearClip + 0.1f;
	auto kAspectRatio = 1.0f;

	size_t sclIndex = 0;
	for(auto scl : shadowCastingLights)
	{
		auto &&renderPoint = renderPoints[sclIndex];
		auto &&shape = lightShapes[scl];
		auto &&parameters = lightParams[scl];
		auto &&data = lightData[scl];
		auto entityIndex = entityLights[scl];
		auto &&transform = renderPoint.transform;

		auto vPos = XMVectorSet(shape.center[0], shape.center[1], shape.center[2], 1.f);
		auto vDir = XMLoadFloat4A(&entityOrientations[entityIndex]);
		auto vSide = XMVector3Normalize(XMVector3Cross(g_XMIdentityR1, vDir));
		if(XMVectorGetX(XMVector3Length(vSide)) <= FLT_EPSILON)
		{
			vSide = g_XMIdentityR2;
		}
		auto vUpDir = XMVector3Normalize(XMVector3Cross(vDir, vSide));
		auto fovY = data.halfOuterAngle * 2.0f;
		auto farClip = std::max(kMinFarClip, shape.radius);

		// Update point light frustum
		auto view = DirectX::XMMatrixLookToLH(vPos, vDir, vUpDir);
		auto proj = DirectX::XMMatrixPerspectiveFovLH(fovY, kAspectRatio, kNearClip, farClip);
		auto viewProj = view * proj;
		XMStoreFloat4x4(&transform.viewMatrix, view);
		XMStoreFloat4x4(&transform.projectionMatrix, proj);
		XMStoreFloat4x4(&transform.viewProjectionMatrix, viewProj);

		auto &frustum = transform.frustum;
		frustum.reset(vPos, vDir, vUpDir, kNearClip, farClip, fovY, kAspectRatio);

		// TODO: frustum-frustum culling
        renderPoint.id = scl;
        renderPoint.kind = go::gfx_render_point_kind::disk_light;
        renderPoint.isActive = true;

		// TODO: calculate importance based on projected area

		// Point this light to the correct shadow caster slot
		parameters.shadowCasterIndex = (float)sclIndex;
		
		++sclIndex;
	}
}


template<typename LightPropertyVector, typename LightParameters, typename LightShape>
void attach_light(
    go::gfx_entity_id id,
    go::gfx_entity_id_vector &entityLights,
    go::gfx_scene::shadow_caster_flag_vector_t &shadowCasterFlags,
    LightPropertyVector &lightPropertyVector,
    bool castsShadows,
    const LightParameters &lightParams,
    const LightShape &lightShape)
{
#if defined(DEBUG) || defined(_DEBUG)
    // Make sure this is a unique light
    auto it = std::find(entityLights.begin(), entityLights.end(), id);
    if(it != entityLights.end())
    {
        GO_LOGFILE_WARNING("A light of this type was already attached to entity %d.\n", id);
    }
#endif

    entityLights.push_back(id);
    shadowCasterFlags.push_back(castsShadows);
    lightPropertyVector.parameters.emplace_back(lightParams);
    lightPropertyVector.shapes.emplace_back(lightShape);
}


// ================================================================================ //
// gfx_scene member functions
// ================================================================================ //

go::gfx_scene::gfx_scene(gfx_scene_manager *sceneMgr, size_t entityCacheSize)
    : m_sceneMgr(sceneMgr), m_isRendering(false), m_activeCamera(-1)
{
    grow_entity_cache(entityCacheSize);

    // Component cache
    m_componentManagers.reserve(GO_ENTITY_MAX_COMPONENT_MANAGERS_COUNT_HINT);
    m_componentManagerIDs.reserve(GO_ENTITY_MAX_COMPONENT_MANAGERS_COUNT_HINT);

    // Initialize render context (command buffers)
    m_renderContext.initialize();
}


go::gfx_scene::~gfx_scene()
{
}


go::gfx_entity *go::gfx_scene::create_entity(gfx_entity_id parent, const entity_name_t &name)
{
    if(m_isRendering)
    {
        // Adding entities while rendering is not allowed
        return nullptr;
    }

    // Grab a new entity ID
    m_entities.emplace_back();
    auto id = m_entities.size() - 1;
    auto result = &m_entities[id];
    result->_change_id(id);
    result->_change_scene(this);

    // Save its name
    m_entityNames.emplace_back(name);

    // Initialize the data for the new entity
    m_entityData.positions.emplace_back(0.0f, 0.0f, 0.0f, 1.0f);
    m_entityData.orientations.emplace_back(0.0f, 0.0f, 1.0f, 0.0f);
    m_entityData.rotations.emplace_back(0.0f, 0.0f, 0.0f, 1.0f);
    m_entityData.scales.emplace_back(1.0f);
    m_entityData.worldTransforms.emplace_back();

    // Create a link between this entity and its parent (if one exists)
    resolve_parent_dependency(id, parent);

    return result;
}


void go::gfx_scene::resolve_parent_dependency(gfx_entity_id id, gfx_entity_id parent)
{
    // TODO: link the entity pointed to by ID with its parent
}


void go::gfx_scene::destroy_entity(gfx_entity_id id)
{
    if(m_isRendering)
    {
        // Don't allow entities to be destroyed while rendering
        return;
    }

    GO_ASSERT(id < m_entities.size());

    // Reset active camera if this was pointing to it
    if(m_activeCamera == id)
    {
        m_activeCamera = (gfx_entity_id)-1;
    }

    // Detach any lights that are bound to the entity
    detach_spherical_light(id);
    detach_disk_light(id);

    // Swap entity with last entity and remove
    auto &e1 = m_entities[id];
    e1._detach_all_components();
    // TODO: update component managers of e2 to signal that its ID has changed
    auto &e2 = m_entities.back();
    auto e1id = e1.id();
    auto e2id = e2.id();
    e1._change_id(e2id);
    e2._change_id(e1id);

    swap_erase(m_entities, id);
    swap_erase(m_entityNames, id);
    swap_erase(m_entityData.positions, id);
    swap_erase(m_entityData.orientations, id);
    swap_erase(m_entityData.rotations, id);
    swap_erase(m_entityData.scales, id);
    swap_erase(m_entityData.worldTransforms, id);
}


auto go::gfx_scene::find_entity(gfx_entity_id id) -> gfx_entity *
{
    GO_ASSERT(id < m_entities.size());
    return &m_entities[id];
}


void go::gfx_scene::find_entity(const entity_name_t &name, entity_vector_t &outEntities)
{
	// TODO: replace with unordered_map
    auto i = 0;
    for(auto &n : m_entityNames)
    {
        if(n == name)
        {
            outEntities.push_back(m_entities[i]);
        }
        ++i;
    }
}


auto go::gfx_scene::entities() const noexcept -> const entity_vector_t &
{
    return m_entities;
}


void go::gfx_scene::change_active_camera(gfx_entity_id newValue) noexcept
{
    if(m_isRendering)
    {
        // Don't change the active camera while rendering
        return;
    }

    m_activeCamera = newValue;
}


auto go::gfx_scene::active_camera() const noexcept -> gfx_entity_id
{
    return m_activeCamera;
}


go::gfx_ambient_sky &go::gfx_scene::sky() noexcept
{
    return m_renderContext.sky;
}


go::gfx_camera_properties &go::gfx_scene::camera_properties() noexcept
{
    return m_renderContext.renderPoints.main.cameraProperties;
}


void go::gfx_scene::grow_entity_cache(size_t newCacheSize)
{
    // Entity data
    m_entityData.positions.reserve(newCacheSize);
    m_entityData.orientations.reserve(newCacheSize);
    m_entityData.rotations.reserve(newCacheSize);
    m_entityData.scales.reserve(newCacheSize);
    m_entityData.worldTransforms.reserve(newCacheSize);

    // Entity cache
    m_entities.reserve(newCacheSize);
    m_entityNames.reserve(newCacheSize);

    // Spherical light cache
    m_entitySphericalLights.reserve(newCacheSize);
    m_entitySphericalLightShadowCasterFlags.reserve(newCacheSize);

    // Disk light cache
    m_entityDiskLights.reserve(newCacheSize);
    m_entityDiskLightShadowCasterFlags.reserve(newCacheSize);
	m_diskLightData.reserve(newCacheSize);
}


auto go::gfx_scene::entity_data() const noexcept -> const entity_data_t &
{
    return m_entityData;
}


void go::gfx_scene::update_component_managers()
{
    for(auto &m : m_componentManagers)
    {
        m->handle_update();
    }
}


void go::gfx_scene::calculate_world_transforms()
{
    // TODO: only if modified (set flag)
    tbb::parallel_for(0, (int)m_entities.size(), [this](int i)
    {
        auto vPos = XMLoadFloat4A(&m_entityData.positions[i]);
        auto vRotation = XMLoadFloat4A(&m_entityData.rotations[i]);
        auto scale = m_entityData.scales[i];
        auto vScale = XMVectorSet(scale, scale, scale, 1.f);
        XMStoreFloat4x4(&m_entityData.worldTransforms[i], XMMatrixAffineTransformation(vScale, g_XMIdentityR3, vRotation, vPos));
    });
}


void go::gfx_scene::determine_shadow_casting_lights()
{
    // Update lights to match entity transformation
    auto &entityPositions = m_entityData.positions;
	auto &entityOrientations = m_entityData.orientations;

    // Camera position
    auto vCamPos = XMLoadFloat4A(&entityPositions[m_activeCamera]);
    
    // Camera frustum...
    auto &camFrustum = m_renderContext.renderPoints.main.transform.frustum;
    
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

    // Spherical lights
    update_shadow_casting_point_lights(
        vCamFrustumBoundingSphere, camFrustumBoundingSphereRadius,
        vCamFrustumConeApex, vCamFrustumConeDir, camFrustumConeCos, camFrustumConeSin,
        vCamFrustumPlanes,
        vCamPos,
        entityPositions,
        m_entitySphericalLightShadowCasterFlags,
        m_entitySphericalLights,
        m_renderContext.renderPoints.pointLights.data(),
        m_renderContext.lights.spherical.shapes,
        m_renderContext.lights.spherical.parameters,
        m_renderContext.lights.visible.spherical,
        m_renderContext.lights.shadowCasting.spherical);

    // Disk lights
	update_shadow_casting_spot_lights(
        vCamFrustumBoundingSphere, camFrustumBoundingSphereRadius,
        vCamFrustumConeApex, vCamFrustumConeDir, camFrustumConeCos, camFrustumConeSin,
        vCamFrustumPlanes,
        vCamPos,
        entityPositions,
		entityOrientations,
        m_entityDiskLightShadowCasterFlags,
        m_entityDiskLights,
		m_renderContext.renderPoints.spotLights.data(),
		m_renderContext.lights.disk.shapes,
		m_renderContext.lights.disk.parameters,
		m_diskLightData,
		m_renderContext.lights.visible.disk,
		m_renderContext.lights.shadowCasting.disk);
}


void go::gfx_scene::update()
{
	// All components should be updated first
	update_component_managers();

	if(m_activeCamera < m_entities.size())
	{
		// Then we update the world-transform matrices for the modified entities
		calculate_world_transforms();

		// Then we update the light-transform shapes according to the new entity data
		update_spherical_lights(m_entityData, m_entitySphericalLights, m_renderContext.lights.spherical);
		update_disk_lights(m_entityData, m_entityDiskLights, m_diskLightData, m_renderContext.lights.disk);
	}
}


void go::gfx_scene::compute_camera_frustum()
{
    auto id = m_activeCamera;
    auto &transform = m_renderContext.renderPoints.main.transform;
    auto &camProps = m_renderContext.renderPoints.main.cameraProperties;

    // Local camera basis
    auto vPos = XMLoadFloat4A(&m_entityData.positions[id]);
    auto vOrientation = XMLoadFloat4A(&m_entityData.orientations[id]);
    auto isLookingStraightUp = XMScalarNearEqual(abs(XMVectorGetX(XMVector3Dot(g_XMIdentityR1, vOrientation))), 1.f, FLT_EPSILON);
    auto vSide = isLookingStraightUp ? g_XMIdentityR2 : XMVector3Normalize(XMVector3Cross(g_XMIdentityR1, vOrientation));
    auto vUp = XMVector3Normalize(XMVector3Cross(vOrientation, vSide));

    // View
    auto viewMatrix = XMMatrixLookToLH(vPos, vOrientation, vUp);
    XMStoreFloat4x4(&transform.viewMatrix, viewMatrix);

    // Projection
    auto &fmt = the_gfx_device->back_buffer()->format();
    auto aspectRatio = fmt.width / (float)fmt.height;
    auto projectionMatrix = XMMatrixPerspectiveFovLH(camProps.fovY, aspectRatio, camProps.farClip, camProps.nearClip); // NOTE: complementary Z!
    XMStoreFloat4x4(&transform.projectionMatrix, projectionMatrix);

    // Composite transforms
    auto viewProjMatrix = viewMatrix * projectionMatrix;
    XMStoreFloat4x4(&transform.viewProjectionMatrix, viewProjMatrix);

    // Compute the frustum
    auto &frustum = transform.frustum;
    frustum.reset(vPos, vOrientation, vUp, camProps.nearClip, camProps.farClip, camProps.fovY, aspectRatio);
}


void go::gfx_scene::compute_directional_light_frustum()
{
	auto &camFrustum = m_renderContext.renderPoints.main.transform.frustum;

    // Load camera info
    auto vCamPos = XMLoadFloat4A(&m_entityData.positions[m_activeCamera]);
    auto vCamDir = XMLoadFloat4A(&m_entityData.orientations[m_activeCamera]);

    // Figure out the light basis vectors
    auto vSunDir = XMLoadFloat3(&m_renderContext.sky.sunDirection);
    auto vToSunDir = -vSunDir;
    auto isSunDirUpParallel = XMScalarNearEqual(abs(XMVectorGetX(XMVector3Dot(g_XMIdentityR1, vSunDir))), 1.f, FLT_EPSILON);
    auto vSunSide = isSunDirUpParallel ? g_XMIdentityR2 : XMVector3Normalize(XMVector3Cross(g_XMIdentityR1, vSunDir));
    auto vSunUp = XMVector3Normalize(XMVector3Cross(vSunDir, vSunSide));

	// Load the camera frustum bounding sphere
	auto vFrustumBSCenter = XMLoadFloat4A(&camFrustum.boundingSphereCenter);
	auto frustumBSRadius = std::ceil(camFrustum.boundingSphereRadius * 16.0f) / 16.0f; // NOTE: round up to prevent precision problems from recalc
	auto frustumBSDiameter = frustumBSRadius * 2.0f;

	// Push the light back
	const auto lightNearClip = 1.0f;
	const auto extraBackupDist = 1000.0f; // TODO: for culling
	const auto backupDist = extraBackupDist + lightNearClip + frustumBSRadius;
	auto vLightPos = vFrustumBSCenter + vToSunDir * backupDist;

	// Build the light view matrix
	auto mLightView = DirectX::XMMatrixLookAtLH(vLightPos, vFrustumBSCenter, vSunUp);

	// Build the projection matrix
	auto bounds = frustumBSDiameter;
	auto lightFarClip = backupDist + frustumBSRadius;
	auto mLightProj = XMMatrixOrthographicLH(bounds, bounds, lightNearClip, lightFarClip);

    // Combine them
    auto mLightViewProj = mLightView * mLightProj;

	// Snap texels
	auto halfShadowMapSize = DFX_DIRECTIONAL_LIGHT_SHADOW_MAP_SIZE * 0.5f;
	auto originLS = XMVector3TransformCoord(g_XMIdentityR3, mLightViewProj);
	originLS *= halfShadowMapSize;
	auto roundedOriginLS = XMVectorRound(originLS);
	auto rounding = roundedOriginLS - originLS;
	rounding /= halfShadowMapSize;

	auto roundMatrix = DirectX::XMMatrixTranslationFromVector(rounding);

	mLightViewProj *= roundMatrix;

    // Save results
    auto &lightSplit = m_renderContext.renderPoints.directionalLight[0];

    auto &lightTransforms = lightSplit.transform;
    XMStoreFloat4x4(&lightTransforms.projectionMatrix, mLightProj);
    XMStoreFloat4x4(&lightTransforms.viewMatrix, mLightView);
    XMStoreFloat4x4(&lightTransforms.viewProjectionMatrix, mLightViewProj);

    // Update the light frustum
    auto &frustum = lightSplit.transform.frustum;
    frustum.reset(vLightPos, vSunDir, bounds, bounds, lightFarClip);
}


void go::gfx_scene::render()
{
    if(m_activeCamera == (gfx_entity_id)-1)
    {
        // No active camera
        return;
    }

    GO_ASSERT(m_activeCamera < m_entities.size());

    // Prevent the active camera from being changed
    m_isRendering = true;

    // Update the camera frustum
    compute_camera_frustum();

    // Update the sun frustum
    compute_directional_light_frustum();

    // Figure out which lights should be casting shadows
    determine_shadow_casting_lights();

    // Let the component managers issue Draw-calls
    for(auto &m : m_componentManagers)
    {
        m->handle_render();
    }

    // Render!
    flush_command_buffers();

    // We're done rendering
    m_isRendering = false;
}


void go::gfx_scene::flush_command_buffers()
{
    // Finalize the context
    m_renderContext.finalize();

    // Send the data to the renderer
    the_gfx_renderer->render_path()->render(m_renderContext, m_sceneMgr->asset_context());
    
    // And reset the render context
    m_renderContext.reset_frame();
}


void go::gfx_scene::attach_spherical_light(gfx_entity_id id, const DirectX::XMFLOAT3 &color, float luminousPower, float size, bool castsShadows)
{
    go::gfx_spherical_lights::parameters_t lightParams;
    
	// Convert luminous power (lm) to luminance (cd/m^2)
	auto luminance = luminousPower / (4.0f * size * size * XM_PI * XM_PI);

	// Pre-scale the color
	auto vColor = XMLoadFloat3(&color);
	vColor *= luminance;
	XMStoreFloat3((XMFLOAT3 *)lightParams.color, vColor);
	
	// Area light radius
	lightParams.radiusWS = size;
	
	// The shadow caster index
	lightParams.shadowCasterIndex = -1;

    go::gfx_spherical_lights::proxy_shape_t lightShape;
    auto &p = m_entityData.positions[id];
	go::copy_float3(lightShape.center, p);
    lightShape.radius = 1.0f; // Updated with the entity radius later

    attach_light(
        id,
        m_entitySphericalLights,
        m_entitySphericalLightShadowCasterFlags,
        m_renderContext.lights.spherical,
        castsShadows, lightParams, lightShape
    );
}


void go::gfx_scene::detach_spherical_light(gfx_entity_id id)
{
	auto it = std::find(m_entitySphericalLights.begin(), m_entitySphericalLights.end(), id);
	if(it != m_entitySphericalLights.end())
	{
		auto lightIndex = it - m_entitySphericalLights.begin();

		go::swap_erase(m_entitySphericalLights, lightIndex);
		go::swap_erase(m_entitySphericalLightShadowCasterFlags, lightIndex);
		go::swap_erase(m_renderContext.lights.spherical.parameters, lightIndex);
		go::swap_erase(m_renderContext.lights.spherical.shapes, lightIndex);
	}
}


const go::gfx_spherical_lights &go::gfx_scene::spherical_lights() const noexcept
{
    return m_renderContext.lights.spherical;
}


void go::gfx_scene::attach_disk_light(gfx_entity_id id, const DirectX::XMFLOAT3 &color, float luminousPower, float size, float coneOuterAngle, float coneInnerAngle, bool castsShadows)
{
	go::gfx_disk_lights::parameters_t lightParams;

	// Area light radius
    lightParams.radiusWS = size;

    go::gfx_disk_lights::proxy_shape_t lightShape;
    auto &p = m_entityData.positions[id];
	go::copy_float3(lightShape.center, p);
    lightShape.radius = 1.0f; // Updated with the entity radius later

	// The shadow caster index
	lightParams.shadowCasterIndex = -1;

	// Add the disk light data
	disk_light_data diskLightData;
	diskLightData.halfOuterAngle = coneOuterAngle * 0.5f;
	diskLightData.rcpTanHalfOuterAngle = 1.0f / std::tan(diskLightData.halfOuterAngle);
	diskLightData.cosOuter = std::cos(coneOuterAngle * 0.5f);
	diskLightData.cosInner = std::cos(std::min(coneInnerAngle, coneOuterAngle - FLT_EPSILON) * 0.5f);
	diskLightData.angleScale = 1.0f / std::max(0.001f, diskLightData.cosInner - diskLightData.cosOuter);
	diskLightData.angleBias = -diskLightData.cosOuter * diskLightData.angleScale;
	m_diskLightData.push_back(std::move(diskLightData));

    attach_light(
        id,
        m_entityDiskLights,
        m_entityDiskLightShadowCasterFlags,
        m_renderContext.lights.disk,
        castsShadows, lightParams, lightShape
    );

	// Change the color
	change_disk_light_color(m_diskLightData.size() - 1, color, luminousPower);
}


void go::gfx_scene::detach_disk_light(gfx_entity_id id)
{
	auto it = std::find(m_entityDiskLights.begin(), m_entityDiskLights.end(), id);
	if(it != m_entityDiskLights.end())
	{
		auto lightIndex = it - m_entityDiskLights.begin();

		go::swap_erase(m_entityDiskLights, lightIndex);
		go::swap_erase(m_entityDiskLightShadowCasterFlags, lightIndex);
		go::swap_erase(m_diskLightData, lightIndex);
		go::swap_erase(m_renderContext.lights.disk.parameters, lightIndex);
		go::swap_erase(m_renderContext.lights.disk.shapes, lightIndex);
	}
}


const go::gfx_disk_lights &go::gfx_scene::disk_lights() const noexcept
{
    return m_renderContext.lights.disk;
}


const go::gfx_entity_id_vector &go::gfx_scene::disk_light_entities() const noexcept
{
	return m_entityDiskLights;
}


void go::gfx_scene::change_disk_light_color(int32_t lightIndex, const DirectX::XMFLOAT3 &color, float luminousPower)
{
	// Convert luminous power (lm) to luminance (cd/m^2)
	auto lightRadius = m_renderContext.lights.disk.parameters[lightIndex].radiusWS;
	auto luminance = luminousPower / (lightRadius * lightRadius * XM_PI * XM_PI);

    // Update the color
	XMStoreFloat3((XMFLOAT3 *)m_renderContext.lights.disk.parameters[lightIndex].color, XMLoadFloat3(&color) * luminance);
}


void go::gfx_scene::translate_entity(gfx_entity_id id, FXMVECTOR vDelta) noexcept
{
    GO_ASSERT(id >= 0 && id < m_entityData.positions.size());
    XMStoreFloat4A(&m_entityData.positions[id], XMLoadFloat4A(&m_entityData.positions[id]) + vDelta);
}


void go::gfx_scene::rotate_entity(gfx_entity_id id, float pitch, float yaw, float roll) noexcept
{
    rotate_entity(id, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}


void go::gfx_scene::rotate_entity(gfx_entity_id id, DirectX::FXMVECTOR vQuat) noexcept
{
    // Update the orientation
    auto p = XMVector3Rotate(XMLoadFloat4A(&m_entityData.orientations[id]), vQuat);
    XMStoreFloat4A(&m_entityData.orientations[id], p);

    // Change the current rotation transform
    auto v = XMQuaternionMultiply(XMLoadFloat4A(&m_entityData.rotations[id]), vQuat);
    XMStoreFloat4A(&m_entityData.rotations[id], v);
}


void go::gfx_scene::scale_entity(gfx_entity_id id, float delta) noexcept
{
    m_entityData.scales[id] *= delta;
}


void go::gfx_scene::change_entity_scale(gfx_entity_id id, float scale) noexcept
{
    m_entityData.scales[id] = scale;
}


go::gfx_render_context &go::gfx_scene::render_context() noexcept
{
    return m_renderContext;
}


go::gfx_scene_manager *go::gfx_scene::scene_manager() noexcept
{
    return m_sceneMgr;
}