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


#include "go\golib.h"

#include "components.h"


class scene_action_map : public go::input_action_map
{
public:
    go::input_context *uiContext;
public:
    void change_scene(go::gfx_scene *scene)
    {
        m_scene = scene;
        m_perspectiveCamMgr = m_scene->find_entity_component_manager<gfx_perspective_camera_component_manager>();
    }
    void handle_input_event(const go::input_event &e, bool &wasHandled) override
    {
        using namespace DirectX;

        if(!m_scene)
        {
            return;
        }

        // Grab the active camera
        auto entityID = m_scene->active_camera();
        if(entityID == (go::gfx_entity_id) - 1)
        {
            // No active camera
            return;
        }

        // Grab the camera properties
        auto &camProps = m_perspectiveCamMgr->component(entityID);

        // Keyboard - Translation
        switch(e.device)
        {
        case go::input_event_device::keyboard:
        {
            // Load the position data
            auto isDown = e.digitalData.isDown;

            // Speed boost
            switch(e.digitalData.keyID)
            {
            // Shift
            case 0x10: { speedMultiplier = isDown ? kDefaultSpeed * 2 : kDefaultSpeed; break; }
            // 1,2
            case 0x31:
            case 0x61:
                {
                    if(!e.digitalData.isDown)
                    {
                        GO_LOGFILE_INFO("Switching to Scene 1.\n");
                        m_scene->change_entity_position(entityID, XMVectorSet(-25.7160015f, 1.78305757f, 0.0379160009f, 1.0f));
                        m_scene->change_entity_rotation(entityID, XMVectorSet(0.0f, 0.618101001f, 0.0f, 0.786098003f));
                    }
                    break;
                }
            case 0x32:
            case 0x62:
                {
                    if(!e.digitalData.isDown)
                    {
                        GO_LOGFILE_INFO("Switching to Scene 2.\n");
                        m_scene->change_entity_position(entityID, XMVectorSet(-15.2028084f, 9.28631878f, 8.03242588f, 1.0f));
                        m_scene->change_entity_rotation(entityID, XMVectorSet(0.0183652323f, 0.975361288f, 0.0170506667f, 0.219184279f));
                    }
                    break;
                }
            // A, D, S, W
            case 0x41: { m_translation.x = isDown ? 1 : m_translation.x > 0 ? 0.f : m_translation.x; break; }
            case 0x44: { m_translation.x = isDown ? -1 : m_translation.x < 0 ? 0.f : m_translation.x; break; }
            case 0x53: { m_translation.z = isDown ? -1 : m_translation.z < 0 ? 0.f : m_translation.z; break; }
            case 0x57: { m_translation.z = isDown ? 1 : m_translation.z > 0 ? 0.f : m_translation.z; break; }
            case 0x56: { m_scene->sky().fogDensity = e.digitalData.isDown ? 0.010f : 0.0025f; break; }
            // Space
            case VK_SPACE: { m_translation.y = isDown ? 1 : m_translation.y > 0 ? 0.f : m_translation.y; break; }
            // Ctrl
            case VK_CONTROL: { m_translation.y = isDown ? -1 : m_translation.y < 0 ? 0.f : m_translation.y; break; }
            // Escape
            case VK_ESCAPE:
            {
                if(!isDown)
                {
                    m_translation = DirectX::XMFLOAT3(0, 0, 0);
                    speedMultiplier = 0;
                    go::the_ui_system->hide_screen(1);
                    go::the_ui_system->show_screen(0);
                    go::the_ui_system->show_cursor();
                    go::the_input_dispatcher->change_active_context(uiContext);
                }
                break;
            }
            }

            break;
        }
        case go::input_event_device::mouse:
        {
            if(e.type == go::input_event_type::analog)
            {
                // Load some information about the camera orientation
                auto &entityData = m_scene->entity_data();
                auto orientation = &entityData.orientations[entityID];

                auto vDirN = XMLoadFloat4(orientation);
                auto vLeftN = XMVector3Normalize(XMVector3Cross(vDirN, DirectX::g_XMIdentityR1));
                auto vUpN = XMVector3Normalize(XMVector3Cross(vDirN, vLeftN));

                // Rotate
                auto elapsedTime = go::performance.elapsedTime;
                m_scene->rotate_entity(entityID, XMQuaternionRotationNormal(vLeftN, elapsedTime * 0.05f * -e.dualAnalogData.motionDelta[1]));
                m_scene->rotate_entity(entityID, XMQuaternionRotationNormal(vUpN, elapsedTime * 0.05f * -e.dualAnalogData.motionDelta[0]));
            }
            break;
        }
        }

        wasHandled = true;
    }
    void update() override
    {
        using namespace DirectX;

        // Grab the active camera
        auto entityID = m_scene->active_camera();
        if(entityID == (go::gfx_entity_id) - 1)
        {
            // No active camera
            return;
        }

        // Translate the camera
        auto &entityData = m_scene->entity_data();
        auto orientation = &entityData.orientations[entityID];

        auto vDirN = XMLoadFloat4(orientation);
        auto vLeftN = XMVector3Normalize(XMVector3Cross(vDirN, DirectX::g_XMIdentityR1));
        
        auto vDelta = go::performance.elapsedTime * speedMultiplier * (m_translation.x * vLeftN + m_translation.y * g_XMIdentityR1 + m_translation.z * vDirN);

        m_scene->translate_entity(entityID, vDelta);

        // Update performance stats
        std::string js = "go_performance_update(";
        js.append(std::to_string(go::performance.cpu.shadowMaps) + "," + std::to_string(go::performance.gpu.shadowMaps));
        js.append("," + std::to_string(go::performance.cpu.prepass) + "," + std::to_string(go::performance.gpu.prepass));
        js.append("," + std::to_string(go::performance.cpu.shading) + "," + std::to_string(go::performance.gpu.shading));
        js.append("," + std::to_string(go::performance.cpu.depthDownsampling) + "," + std::to_string(go::performance.gpu.depthDownsampling));
        js.append("," + std::to_string(go::performance.cpu.hbao) + "," + std::to_string(go::performance.gpu.hbao));
        js.append("," + std::to_string(go::performance.cpu.ambientLighting) + "," + std::to_string(go::performance.gpu.ambientLighting));
        js.append("," + std::to_string(go::performance.cpu.volumetricLighting) + "," + std::to_string(go::performance.gpu.volumetricLighting));
        js.append("," + std::to_string(go::performance.cpu.hdrBloom) + "," + std::to_string(go::performance.gpu.hdrBloom));
        js.append("," + std::to_string(go::performance.cpu.tonemapping) + "," + std::to_string(go::performance.gpu.tonemapping));
        js.append("," + std::to_string(go::performance.cpu.fxaa) + "," + std::to_string(go::performance.gpu.fxaa));
        js.append("," + std::to_string(go::performance.cpu.filmGrain) + "," + std::to_string(go::performance.gpu.filmGrain));
        js.append("," + std::to_string(go::performance.cpu.vignette) + "," + std::to_string(go::performance.gpu.vignette));
        js.append(")");
        go::the_ui_system->execute_script(1, js);
    }
public:
    static constexpr float kDefaultSpeed = 2.5f;
    float speedMultiplier = kDefaultSpeed;
private:
    go::gfx_scene *m_scene = nullptr;
    gfx_perspective_camera_component_manager *m_perspectiveCamMgr;
    DirectX::XMFLOAT3 m_translation = DirectX::XMFLOAT3(0, 0, 0);
};


class ui_action_map : public go::input_action_map
{
public:
    go::input_context *sceneContext;
    scene_action_map *sceneInputMap;
public:
    void handle_input_event(const go::input_event &e, bool &wasHandled) override
    {
        if(e.device == go::input_event_device::keyboard)
        {
            if(e.digitalData.keyID == VK_ESCAPE && !e.digitalData.isDown)
            {
                go::the_ui_system->hide_screen(0);
                go::the_ui_system->hide_cursor();
                go::the_ui_system->show_screen(1);
                go::the_input_dispatcher->change_active_context(sceneContext);
                sceneInputMap->speedMultiplier = scene_action_map::kDefaultSpeed;
            }
        }
        
        wasHandled = true;
    }
    void update() override
    {

    }
};