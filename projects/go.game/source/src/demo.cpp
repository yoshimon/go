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

#include "demo.h"


demo_game *the_game = nullptr;


go::application *create_go_application()
{
    return new demo_game();
}


demo_game::demo_game()
    : singleton<demo_game>(the_game)
{
}


demo_game::~demo_game()
{
}


bool demo_game::handle_modify_runtime_configuration(go::runtime_configuration *)
{
    return true;
}


void demo_game::handle_initialize(int, char *[])
{
    m_sceneMgr = std::make_unique<go::gfx_scene_manager>(go::default_value);
    auto scene = m_sceneMgr->create_scene();

    // Register component managers
    scene->register_entity_component_manager<gfx_perspective_camera_component_manager>(1);
    scene->register_entity_component_manager<gfx_model_component_manager>(1024);

    // Register input action maps
    m_sceneInputActionMap.change_scene(scene);

    m_sceneInputActionMap.uiContext = &m_uiInputContext;
    m_uiInputActionMap.sceneContext = &m_sceneInputContext;
	m_uiInputActionMap.sceneInputMap = &m_sceneInputActionMap;

    m_sceneInputContext.push_back(&m_sceneInputActionMap);
    m_uiInputContext.push_back(&m_uiInputActionMap);

    go::the_input_dispatcher->change_active_context(&m_sceneInputContext);

    // Load the performance UI
    auto &&screen = go::the_ui_system->create_screen_from_file(GO_PATH_SYMBOL_UI_BASE "html5/debug/performance.html");
    go::the_ui_system->show_screen(screen);
    go::the_ui_system->hide_cursor();

    // Setup the scene
    auto &sky = scene->sky();

	sky.color = DirectX::XMFLOAT3(0.0f, 0.0f, 00.0);
    sky.material = "sky\\sunset";
	
	auto &&camProps = scene->camera_properties();
	camProps.fovY = 1.4;
	camProps.nearClip = 0.1f;
	camProps.farClip = 100.0f;

	// Load the scene environment map
	auto &&assetContext = m_sceneMgr->asset_context();
	auto &&sceneEnvMapMat = assetContext.material_db().find("envmaps\\sample");
	auto &&sceneEnvSpecularMap = assetContext.texture_mgr().load(sceneEnvMapMat->textureIDs[1], 0, go::resource_eviction_policy::permanent);
	sky.specularEnvMap = *sceneEnvSpecularMap;

    auto vSunColor = DirectX::XMVectorScale(DirectX::XMVectorSet(1.f, 0.9568f, 0.8392f, 0), 2.0f);
    DirectX::XMStoreFloat3(&sky.sunColor, vSunColor);

	auto vAmbientColorAbove = DirectX::XMVectorScale(DirectX::XMVectorSet(0.11f, 0.18f, 0.137f, 0), 3.0f);
	//auto vAmbientColorAbove = DirectX::XMVectorScale(DirectX::XMVectorSet(0.31f, 0.18f, 0.137f, 0), 0.4f);
    DirectX::XMStoreFloat3(&sky.ambientColorAbove, vAmbientColorAbove);

	auto vAmbientColorBelow = DirectX::XMVectorScale(DirectX::XMVectorSet(0.409f, 0.239f, 0.109f, 0), 3.0f);
	//auto vAmbientColorBelow = DirectX::XMVectorScale(DirectX::XMVectorSet(0.409f, 0.239f, 0.109f, 0), 0.4f);
    DirectX::XMStoreFloat3(&sky.ambientColorBelow, vAmbientColorBelow);

    sky.fogDensity = 0.0001f;
    sky.fogColor = DirectX::XMFLOAT3(sky.ambientColorAbove.x, sky.ambientColorAbove.y, sky.ambientColorAbove.z);
	//DirectX::XMStoreFloat3(&sky.sunDirection, DirectX::XMVector3Normalize(DirectX::XMVectorSet(-5, -1.5, -0.5, 0)));
	DirectX::XMStoreFloat3(&sky.sunDirection, DirectX::XMVector3Normalize(DirectX::XMVectorSet(0.0, -1.0, 2.0, 0)));

    // World camera
    go::gfx_entity *cameraEntity = nullptr;

	auto numLights = 0;

    // Load plane
    rapidxml::xml_document<> doc;
    try
    {
        go::file_path sceneFilePath(GO_PATH_SYMBOL_WORLD_BASE "scenes\\woods.scn");
        go::file_path sceneModelPrefix("graveyard");
        auto txt = go::text_file::read(sceneFilePath);
        doc.parse<0>((char *)txt.c_str());

        auto nRoot = doc.first_node("scene");

        // NOTE: extra scene scaling
        float sceneScale = 0.8;

        // <entity>
        auto nEntity = nRoot->first_node("entity");
        while(nEntity)
        {
            // Read out the entity data
            auto scaleXYZ = std::stof(nEntity->first_node("scale")->value()) * sceneScale;
            auto rotation = go::string_to_vector4f<DirectX::XMFLOAT4>(nEntity->first_node("rotation")->value());
            auto pos = go::string_to_vector3f<DirectX::XMFLOAT3>(nEntity->first_node("position")->value());

            // Apply transform
            auto vRot = XMLoadFloat4(&rotation);
            auto vPos = XMLoadFloat3(&pos);
            vPos = DirectX::XMVectorScale(vPos, sceneScale);

            // Create the entity
            auto entity = scene->create_entity();
            scene->rotate_entity(*entity, vRot);
            scene->translate_entity(*entity, vPos);
            scene->change_entity_scale(*entity, scaleXYZ);

            // Spawn entities
            auto attName = nEntity->first_attribute("name");
            if(attName)
            {
                if(strstr(attName->value(), "Main Camera"))
                {
                    if(!cameraEntity)
                    {
                        cameraEntity = entity;
                        cameraEntity->attach_component<gfx_perspective_camera_component_manager>();
                        //scene->scale_entity(*cameraEntity, 1000.0f);
                        //scene->attach_disk_light(*entity, DirectX::XMFLOAT3(1,1,1), 100.f, 1.0f, DirectX::XM_PIDIV4, DirectX::XM_PIDIV4 * 0.5f, true);
						//scene->attach_spherical_light(*entity, DirectX::XMFLOAT3(1,1,1), 100.0f, 0.1f, false);
						scene->change_active_camera(*cameraEntity);
					}
                }
            }

            auto nComponents = nEntity->first_node("components");
            if(nComponents)
            {
                // Light
                auto nLight = nComponents->first_node("light");
                if(nLight)
                {
                    auto attType = nLight->first_attribute("type");
                    if(!strcmp(attType->value(), "Point") || !strcmp(attType->value(), "Spot"))
                    {
						// Point light
						auto attIntensity = nLight->first_attribute("intensity");
						auto attRadius = nLight->first_attribute("radius");
						auto attCastsShadows = nLight->first_attribute("castsShadows");
						auto attColor = nLight->first_attribute("color");

						auto lightIntensityScale = 10.f;
						auto luminousPower = (float)std::atof(attIntensity->value()) * lightIntensityScale;
						auto radius = (float)std::atof(attRadius->value()) * sceneScale;
						auto color = go::string_to_vector3f<DirectX::XMFLOAT3>(attColor->value());
                        auto castsShadows = (bool)std::atoi(attCastsShadows->value());

						//scene->attach_disk_light(*entity, color, luminousPower, 1.0f, DirectX::XM_PI - DirectX::XM_PIDIV4, DirectX::XM_PIDIV4, castsShadows);
						scene->attach_spherical_light(*entity, color, luminousPower, 0.1f, castsShadows);
						scene->change_entity_scale(*entity, radius); // Override entity scale
						++numLights;
                    }
                }

                // Model
                auto nMesh = nComponents->first_node("model");
                if(nMesh)
                {
                    // Grab the model name
                    auto modelAssetName = nMesh->first_attribute("assetName")->value();
                    if(modelAssetName)
                    {
                        // Lookup the model definition
                        auto modelDef = m_sceneMgr->asset_context().model_db().find(modelAssetName);
                        if(!modelDef)
                        {
                            GO_THROW(go::exception, "Model not found in the database.");
                        }

						// TODO: submeshindex, change renderer to draw submeshes, AABB for submeshes

                        // Load the model
                        auto model = m_sceneMgr->asset_context().model_mgr().load_async(modelDef->id);

                        // Make it Model-attachable
                        auto componentManager = entity->attach_component<gfx_model_component_manager>();

                        auto attCastsShadows = nMesh->first_node("castsShadows");
                        auto castsShadows = true;
                        if(attCastsShadows)
                        {
                            castsShadows = (bool)std::atoi(attCastsShadows->value());
                        }

                        auto attCastsShadowsOnly = nMesh->first_node("castsShadowsOnly");
                        auto castsShadowsOnly = false;
                        if(attCastsShadowsOnly)
                        {
                            castsShadowsOnly = (bool)std::atoi(attCastsShadowsOnly->value());
                        }
                        
                        // Bind the model
                        componentManager->change_model(*entity, model, castsShadows, castsShadowsOnly);
                    }
                }
            }

            nEntity = nEntity->next_sibling(nEntity->name());
        }
    }
    catch(...)
    {
        GO_THROW(go::data_error, "Could not load scene file.");
    }
}


void demo_game::handle_shutdown()
{
    m_sceneMgr.reset();
}


void demo_game::handle_update()
{
	// Update the sun
	static float s_time = 0.0f;
	s_time += 0.01f;
	auto sinTime = (std::sin(s_time) + 1.0f) * 0.5f;
	auto sinTimeFast = (std::sin(s_time * 30.0f) + 1.0f) * 0.5f;

	auto &&scene = m_sceneMgr->scenes().back();

	//scene->change_disk_light_color(4, DirectX::XMFLOAT3(1,1,1), 3000000.0f * std::min(1.0f, std::max(0.2f, sinTimeFast * 2.0f)));

    m_sceneMgr->update();
}


void demo_game::handle_draw_world()
{
    m_sceneMgr->render(0);
}


void demo_game::handle_draw_interface()
{
}


void demo_game::handle_activate()
{
}


void demo_game::handle_deactivate()
{
}