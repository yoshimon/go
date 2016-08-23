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


#include "go/golib.h"
#include "tbb/parallel_for.h"


class gfx_perspective_camera_component_manager : public go::gfx_entity_component_manager
{
    GO_DECLARE_ENTITY_COMPONENT_MANAGER("Go.PerspectiveCamera")
public:
    typedef go::gfx_entity_component_manager entity_component_manager_t;
    typedef entity_component_manager_t base_t;
public:
    struct component_data_t
    {
        bool isInputEnabled = false;
        float fovY = 1.6f;
        float aspectRatio = 1.777f;
        float nearClip = 1.f;
        float farClip = 100.f;
        float translationSpeed = 1.f;
    };
public:
    gfx_perspective_camera_component_manager(go::gfx_scene *scene, size_t componentCountHint)
        : base_t(scene, componentCountHint)
    {
        m_data.reserve(componentCountHint);
    }
public:
    void handle_attach_entity(go::gfx_entity_id id) override
    {
        m_data.emplace_back();
        base_t::handle_attach_entity(id);
    }
    void handle_detach_entity(go::gfx_entity_id id) override
    {
        auto index = id_to_index(id);
        base_t::handle_detach_entity(index);
        go::swap_erase(m_data, index);
    }
public:
    component_data_t &component(go::gfx_entity_id id)
    {
        return m_data[id_to_index(id)];
    }
private:
    typedef std::vector<component_data_t> component_data_vector_t;
private:
    component_data_vector_t m_data;
};


class gfx_model_component_manager : public go::gfx_entity_component_manager
{
    GO_DECLARE_ENTITY_COMPONENT_MANAGER("Go.Model")
public:
    typedef go::gfx_entity_component_manager base_t;
    typedef go::gfx_model_manager::resource_handle_t model_resource_handle_t;
public:
    gfx_model_component_manager(go::gfx_scene *scene, size_t componentCountHint)
        : base_t(scene, componentCountHint)
    {
        m_data.reserve(componentCountHint);
    }
public:
    void change_model(
        go::gfx_entity_id id,
        const model_resource_handle_t &hModel,
        bool castsShadows,
        bool castsShadowsOnly)
    {
        auto index = id_to_index(id);
        m_data[index] = { hModel, castsShadows, castsShadowsOnly };
    }
public:
    void handle_render() override
    {
        using namespace go;
        using namespace DirectX;

        auto &assetContext = scene()->scene_manager()->asset_context();
        auto &matMgr = assetContext.material_mgr();

        // TODO: replace this function by a global ::fill_command_buffers function or something along those lines

        // Cache scene data
        auto s = scene();
        auto &entityData = s->entity_data();
        auto &entityPositions = entityData.positions;
        auto &entityScales = entityData.scales;
		auto &entityTransforms = entityData.worldTransforms;
		auto &entityRotations = entityData.rotations;
        auto &renderContext = s->render_context();

        // Cache camera position
        auto vCamPos = XMLoadFloat4(&entityPositions[s->active_camera()]);

        // Cache camera bounding volumes
        XMVECTOR vCamFrustumBoundingSphere;
        float camFrustumBoundingSphereRadius;
        XMVECTOR vCamDir;
        XMVECTOR vCamFrustumPlanes[6];
        float camFrustumConeCos, camFrustumConeSin;
        {
            auto &frustum = renderContext.renderPoints.main.transform.frustum;
            frustum.load_planes(vCamFrustumPlanes);

            {
                // Bounding sphere
                vCamFrustumBoundingSphere = XMLoadFloat4(&frustum.boundingSphereCenter);
                camFrustumBoundingSphereRadius = frustum.boundingSphereRadius;

                // Bounding cone
                vCamDir = XMLoadFloat4(&frustum.direction);
                camFrustumConeCos = frustum.boundingConeAngleCos;
                camFrustumConeSin = frustum.boundingConeAngleSin;
            }
        }

        // Cache sun bounding volumes
        XMVECTOR vSunFrustumPos;
        XMVECTOR vSunFrustumBoundingCylinderAxis;
        float sunFrustumBoundingCylinderRadius;
        {
            auto &frustum = renderContext.renderPoints.directionalLight[0].transform.frustum;

            vSunFrustumPos = XMLoadFloat4(&frustum.position);

            // Cylinder
            vSunFrustumBoundingCylinderAxis = XMLoadFloat4(&frustum.boundingCylinderAxis);
            sunFrustumBoundingCylinderRadius = frustum.boundingCylinderRadius;
        }

        tbb::parallel_for(0, (int)m_data.size(), [&](int i)
		//auto i = -1;
		//for(auto &hModel : m_models)
        {
			//++i;

			auto &&data = m_data[i];
            if(data.hModel)
            {
                // Lookup model pointer
                auto &&model = *data.hModel;
                if(model->unskinnedMeshes.empty() && model->skinnedMeshes.empty())
                {
                    // Probably default model
                    return;
                }

                // Convert index to entity ID
                auto id = index_to_id(i);

				auto &&entityScale = entityScales[id];

                // Grab the entity bounding sphere
                auto &&sphere = model->definition->boundingSphere;
                auto &&vEntityBoundingSphere = XMLoadFloat4A(&sphere);
                auto entityBoundingSphereRadius = sphere.w * entityScale;

                // Transform the bounding sphere to follow the entity
                auto &&mEntityTransform = XMLoadFloat4x4(&entityTransforms[id]);
				auto &&mEntityRotation = XMLoadFloat4(&entityRotations[id]);
                vEntityBoundingSphere = XMVector3TransformCoord(vEntityBoundingSphere, mEntityTransform);

				// Load the AABB corners
				auto &&vAABBExtents = entityScale * XMLoadFloat4A(&model->definition->aabbExtents);
				
				auto &&vOBBCenter = vEntityBoundingSphere;
				auto &&mOBBAxis = XMMatrixRotationQuaternion(mEntityRotation);

                // Setup command
				// The render command (can be down-cast to a static-render command)
				gfx_dynamic_model_render_command command;
                command.model = model;

				// Calculate approximate distance of entity to camera
				auto &&distance = XMVectorGetX(XMVector3LengthSq(vEntityBoundingSphere - vCamPos)) - entityBoundingSphereRadius;
				command.viewZ = std::max(0.0f, distance);
                command.worldTransform = &entityTransforms[id]; // Point to this transform

                // Main camera
                auto &&mainCamVisible = false;
                if(!data.castsShadowsOnly)
                {
                    mainCamVisible =
                        intersection_tests::sphere_sphere(vCamFrustumBoundingSphere, camFrustumBoundingSphereRadius, vEntityBoundingSphere, entityBoundingSphereRadius) &&
                        intersection_tests::cone_sphere(vCamPos, vCamDir, camFrustumConeCos, camFrustumConeSin, vEntityBoundingSphere, entityBoundingSphereRadius) != intersection_tests::result_t::disjoint &&
                        intersection_tests::frustum_obb(vCamFrustumPlanes, vOBBCenter, vAABBExtents, (XMVECTOR *)&mOBBAxis) != intersection_tests::result_t::disjoint;
                    if(mainCamVisible)
                    {
                        renderContext.renderPoints.main.commandBuffers.staticGeometry.enqueue(command);
                    }
                }

                // Shadow caster flags
                auto &&castsShadows = false;
                if(data.castsShadows)
                {
                    // Directional light
                    auto dirLightVisible = intersection_tests::cylinder_sphere(vSunFrustumPos, vSunFrustumBoundingCylinderAxis, sunFrustumBoundingCylinderRadius, vEntityBoundingSphere, entityBoundingSphereRadius);
                    if(dirLightVisible)
                    {
                        renderContext.renderPoints.directionalLight[0].commandBuffers.staticGeometry.enqueue(command);
                        castsShadows = true;
                    }

                    // Point lights
                    for(auto &&pointLight : renderContext.renderPoints.pointLights)
                    {
                        if(!pointLight.isActive)
                        {
                            continue;
                        }

                        // The light position doesn't change
                        auto vLightPos = XMLoadFloat4(&pointLight.faceTransforms[0].frustum.position);

                        for(auto faceIndex = 0; faceIndex < 6; ++faceIndex)
                        {
                            if(pointLight.isFaceActive[faceIndex])
                            {
                                auto &&faceFrustum = pointLight.faceTransforms[faceIndex].frustum;
                                auto vLightDir = XMLoadFloat4(&faceFrustum.direction);
                                auto vLightFrustumBoundingSphere = XMLoadFloat4(&faceFrustum.center);

                                if(intersection_tests::sphere_sphere(vLightFrustumBoundingSphere, faceFrustum.boundingSphereRadius, vEntityBoundingSphere, entityBoundingSphereRadius))
                                {
                                    if(intersection_tests::cone_sphere(vLightPos, vLightDir, faceFrustum.boundingConeAngleCos, faceFrustum.boundingConeAngleSin, vEntityBoundingSphere, entityBoundingSphereRadius) != intersection_tests::result_t::disjoint)
                                    {
                                        XMVECTOR vLightFaceFrustumPlanes[6];
                                        faceFrustum.load_planes(vLightFaceFrustumPlanes);

                                        if(intersection_tests::frustum_obb(vLightFaceFrustumPlanes, vOBBCenter, vAABBExtents, (XMVECTOR *)&mOBBAxis) != intersection_tests::result_t::disjoint)
                                        {
                                            pointLight.faceCommandBuffers[faceIndex].staticGeometry.enqueue(command);
                                            castsShadows = true;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Spot lights
                    for(auto &&spotLight : renderContext.renderPoints.spotLights)
                    {
                        if(!spotLight.isActive)
                        {
                            continue;
                        }

                        // The light position doesn't change
                        auto vLightPos = XMLoadFloat4(&spotLight.transform.frustum.position);

                        auto &&frustum = spotLight.transform.frustum;
                        auto vLightDir = XMLoadFloat4(&frustum.direction);
                        auto vLightFrustumBoundingSphere = XMLoadFloat4(&frustum.center);

                        if(intersection_tests::sphere_sphere(vLightFrustumBoundingSphere, frustum.boundingSphereRadius, vEntityBoundingSphere, entityBoundingSphereRadius))
                        {
                            if(intersection_tests::cone_sphere(vLightPos, vLightDir, frustum.boundingConeAngleCos, frustum.boundingConeAngleSin, vEntityBoundingSphere, entityBoundingSphereRadius) != intersection_tests::result_t::disjoint)
                            {
                                XMVECTOR vLightFaceFrustumPlanes[6];
                                frustum.load_planes(vLightFaceFrustumPlanes);

                                if(intersection_tests::frustum_obb(vLightFaceFrustumPlanes, vOBBCenter, vAABBExtents, (XMVECTOR *)&mOBBAxis) != intersection_tests::result_t::disjoint)
                                {
                                    spotLight.commandBuffers.staticGeometry.enqueue(command);
                                    castsShadows = true;
                                }
                            }
                        }
                    }
                }

				auto isVisible = mainCamVisible || castsShadows;
				if(isVisible)
				{
					// Issue an async request for the material textures upfront
					// TODO: material textures might not be needed if it casts shadows only and it doesn't use alpha testing
					for(auto &material : model->definition->materials)
					{
						matMgr.load_material_textures(material);
					}
				}
            }
		});
    }
    void handle_attach_entity(go::gfx_entity_id id) override
    {
        m_data.emplace_back();
        base_t::handle_attach_entity(id);
    }
    void handle_detach_entity(go::gfx_entity_id id) override
    {
        auto index = id_to_index(id);
        base_t::handle_detach_entity(index);
        go::swap_erase(m_data, index);
    }
private:
    struct data_t
    {
        model_resource_handle_t hModel;
        bool castsShadows;
        bool castsShadowsOnly;
    };
    typedef std::vector<data_t> data_vector_t;
private:
    data_vector_t m_data;
};