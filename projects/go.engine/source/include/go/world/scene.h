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
    \file world/scene.h
    \brief Contains classes which represent the game world.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/aligned.h"
#include "go/graphics/render_context.h"
#include "go/world/entity_manager.h"
#include "go/world/entity_component_manager.h"
#include "go/world/entity_manager.h"
#include "go/world/light_manager.h"
#include "go/world/scene_environment.h"


GO_BEGIN_NAMESPACE

class scene_manager;


//! A class, that holds a collection of entity and component managers to represent a scene.
class GO_API GO_SSE_ALIGN scene : public aligned<scene>
{
    friend class go::light_manager;
public:
    /*!
        Constructor.

        \param sceneMgr The scene manager.
        \param entityCapacity Upper bound for the number of entities in the scene.
     */
    scene(scene_manager *sceneMgr, size_t entityCapacity = GO_ENTITY_MAX_COUNT_HINT);
public:
    /*!
        Registers a new component manager with the scene.

        \tparam T The component manager class.
        \tparam Args The arguments to forward to the component manager.

        \param componentCountHint The max-estimation of the number of components to manage.

        \return The new component manager or nullptr if no component manager was created.
     */
    template<class T, class... Args>
    T *register_entity_component_manager(size_t componentCountHint, Args &&... args);
    /*!
        Searches for an entity component manager.

        \tparam T The component manager to search for.
     */
    template<class T>
    T *find_entity_component_manager();
    /*!
        Returns the entity manager.

        \return The entity manager.
     */
    entity_manager *entity_manager() noexcept { return &m_entityManager; }
    /*!
        Returns the light manager.

        \return The light manager.
     */
    light_manager *light_manager() noexcept { return &m_lightManager; }
    /*!
        Returns the scene environment.

        \return The scene environment.
     */
    const scene_environment &environment() const noexcept { return m_environment; }
    /*!
        Changes the scene environment.

        \param value The new environment.
     */
    void change_environment(const scene_environment &value);
    /*!
        Changes the active camera.

        \param entityID The entity identifier.
     */
    void change_active_camera(entity_id entityID) noexcept;
    /*!
        Returns the active camera.

        \return The active camera.
     */
    entity_id active_camera() const noexcept { return m_activeCamera; }
    /*!
        Changes the main camera properties.

        \return The main camera properties.
     */
    const gfx_camera_properties &camera_properties() const noexcept { return m_cameraProperties; }
    /*!
        Changes the main camera properties.

        \param value The new properties.

        \return The main camera properties.
     */
    void change_camera_properties(const gfx_camera_properties &value);
    //! Renders the scene.
    void render();
    //! Updates the scene.
    void update();
private:
    //! Flushes the render context.
    void flush_render_context();
    //! Updates the main camera frustum.
    void update_camera_frustum();
private:
    typedef std::vector<std::unique_ptr<entity_component_manager>> component_manager_vector_t;
private:
    //! The owner.
    go::scene_manager *m_owner;
    //! The entity manager.
    go::entity_manager m_entityManager;
    //! The light manager.
    go::light_manager m_lightManager;
    //! The scene environment.
    go::scene_environment m_environment;
    //! The graphics context used for rendering.
    gfx_render_context m_renderContext;
    //! The component managers.
    component_manager_vector_t m_componentManagers;
    //! The active camera entity.
    entity_id m_activeCamera;
    //! The main camera properties.
    gfx_camera_properties m_cameraProperties;
};


#include "scene_impl.h"

GO_END_NAMESPACE