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


#include "go/graphics/render_context.h"
#include "tbb/parallel_invoke.h"


// ================================================================================ //
// go::gfx_render_context member functions
// ================================================================================ //

void go::gfx_render_context::initialize()
{
    // Main camera
    renderPoints.main.commandBuffers.resize(GO_MAX_NUM_DRAW_CALLS_PER_PASS);

    // Directional light
    for(auto &split : renderPoints.directionalLight)
    {
        split.commandBuffers.resize(GO_MAX_NUM_DRAW_CALLS_PER_PASS);
    }

    // Shadow-casting point lights
    for(auto &light : renderPoints.pointLights)
    {
        for(auto &commandBuffers : light.faceCommandBuffers)
        {
            commandBuffers.resize(GO_MAX_NUM_DRAW_CALLS_PER_PASS);
        }
    }

    // Shadow-casting spot lights
    for(auto &light : renderPoints.spotLights)
    {
        light.commandBuffers.resize(GO_MAX_NUM_DRAW_CALLS_PER_PASS);
    }

    // Lights
    lights.spherical.parameters.reserve(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE);
    lights.spherical.shapes.reserve(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE);
    lights.visible.spherical.reserve(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE);

    lights.disk.parameters.reserve(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE);
    lights.disk.shapes.reserve(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE);
    lights.visible.disk.reserve(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE);

    lights.shadowCasting.spherical.reserve(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE);
    lights.shadowCasting.disk.reserve(GO_MAX_NUM_VISIBLE_LIGHTS_PER_TYPE);
}


void go::gfx_render_context::finalize()
{
    // Sort all commands buffers
    tbb::parallel_invoke(
        [this] {
            // Main camera
            renderPoints.main.commandBuffers.sort();
        },
        [this] {
            // Directional light
            for(auto &split : renderPoints.directionalLight)
            {
                split.commandBuffers.sort();
            }
        },
        [this] {
            // Point lights
            for(auto &light : renderPoints.pointLights)
            {
                for(auto &commandBuffers : light.faceCommandBuffers)
                {
                    commandBuffers.sort();
                }
            }
        },
        [this] {
            // Spot lights
            for(auto &light : renderPoints.spotLights)
            {
                light.commandBuffers.sort();
            }
        }
    );
}


void go::gfx_render_context::clear_command_buffers()
{
    // Main camera
    renderPoints.main.commandBuffers.clear();

    // Directional light
    for(auto &split : renderPoints.directionalLight)
    {
        split.commandBuffers.clear();
    }

    // Point lights
    for(auto &light : renderPoints.pointLights)
    {
        // Clear the cmd buffers
        for(auto &commandBuffers : light.faceCommandBuffers)
        {
            commandBuffers.clear();
        }

        // Disable the light
        light.isActive = false;

        // Disable all faces
        for(auto &&face : light.isFaceActive)
        {
            face = false;
        }
    }

    // Spot lights
    for(auto &light : renderPoints.spotLights)
    {
        // Clear the cmd buffer
        light.commandBuffers.clear();

        // Disable the light
        light.isActive = false;
    }
}


void go::gfx_render_context::reset_light_caches()
{
    // Unmark shadow casting lights
    for(auto scl : lights.shadowCasting.spherical)
    {
        lights.spherical.parameters[scl].shadowCasterIndex = -1.0f;
    }

    for(auto scl : lights.shadowCasting.disk)
    {
        lights.disk.parameters[scl].shadowCasterIndex = -1.0f;
    }

    lights.shadowCasting.disk.clear();
    lights.shadowCasting.spherical.clear();

    // Reset visibility cache
    lights.visible.disk.clear();
    lights.visible.spherical.clear();
}


void go::gfx_render_context::reset_frame()
{
    clear_command_buffers();
    reset_light_caches();
}