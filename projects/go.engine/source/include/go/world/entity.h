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
    \file world/entity.h
    \brief Contains a class that represents an entity in the world.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

typedef uint32_t entity_id;
typedef std::vector<entity_id> entity_id_vector;

class scene;
class entity_component_manager;

//! A class, which represents an entity in the world.
class entity
{
public:
    /*!
        Implicit conversion operator.

        \return The entity identifier.
     */
    GO_API operator entity_id() const noexcept { return m_id; };
public:
    /*!
        Returns the entity identifier.

        \return The entity identifier.
     */
    GO_API entity_id id() const noexcept { return m_id; }
    /*!
        Changes the entity identifier.

        \remarks Internal use only.

        \param newValue The new identifier value.
     */
    GO_API void _change_id(uint32_t newValue) noexcept { m_id = newValue; }
    /*!
        Changes the scene pointer.

        \remarks Internal use only.

        \param scene The new scene pointer.
     */
    GO_API void _change_scene(scene *scene) noexcept { m_scene = scene; }
    /*!
        Detaches all compnoents from the entity.

        \remarks Internal use only.
     */
    GO_API void _detach_all_components();
    /*!
        Returns a component, that is attached to the entity.

        \tparam T The component manager type.

        \param className The component class name.
     */
    template<typename SomeComponentManager>
    SomeComponentManager *find_component() const
    {
        auto cid = SomeComponentManager::component_class_id();
        for(auto &cm : m_componentManagers)
        {
            if(!cm)
            {
                // No manager found
                break;
            }
            else
            {
                // Match found?
                if(cm->component_instance_class_id() == cid)
                {
                    return static_cast<SomeComponentManager *>(cm);
                }
            }
        }

        return nullptr;
    }
    /*!
        Attaches a new component to the entity.

        \param T The component manager type.
     */
    template<typename SomeComponentManager>
    SomeComponentManager *attach_component()
    {
        auto &&cid = SomeComponentManager::component_class_id();
        size_t i = 0;
        for(auto &&cm : m_componentManagers)
        {
            if(!cm)
            {
                // Insert new manager
                auto &&mgr = m_scene->find_entity_component_manager<SomeComponentManager>();

                // Component manager not registered?
                if(!mgr)
                {
                    return false;
                }

                // Bind manager
                m_componentManagers[i] = mgr;
                mgr->handle_attach_entity(*this);
                return mgr;
            }
            else if(cm->component_instance_class_id() == cid)
            {
                // Component manager class already attached
                return (SomeComponentManager *)cm;
            }

            ++i;
        }

        return nullptr;
    }
private:
    //! The entity identifier.
    entity_id m_id;
    //! The component managers that are referenced by this entity.
    std::array<entity_component_manager *, GO_ENTITY_MAX_COMPONENT_COUNT> m_componentManagers;
    //! The scene pointer.
    scene *m_scene = nullptr;
};

GO_END_NAMESPACE