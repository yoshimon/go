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
    \file graphics/entity_componpent_manager.h
    \brief Contains a class that manages a set of entity components.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/graphics/entity.h"


GO_BEGIN_NAMESPACE

class gfx_scene;

//! A class, which represents a component manager.
class GO_API gfx_entity_component_manager
{
    GO_DECLARE_ENTITY_COMPONENT_MANAGER_BASE
public:
    /*!
        Constructor.

        \param scene The scene.
        \param componentCountHint The estimated number of components to hold.
     */
    gfx_entity_component_manager(gfx_scene *scene, size_t componentCountHint)
        : m_scene(scene)
    {
        m_entityIDs.reserve(componentCountHint);
    }
    //! Destructor.
    virtual ~gfx_entity_component_manager() {}
public:
    /*!
        Returns the scene.

        \return The scene.
     */
    gfx_scene *scene() const noexcept { return m_scene; }
public:
    //! Updates the components in the manager.
    virtual void handle_update() {}
    //! Renders the components in the manager.
    virtual void handle_render() {}
    /*!
        Callback function which is invoked when an entity gets attached.
        
        \param id The entity identifier.
     */
    virtual void handle_attach_entity(gfx_entity_id id) { m_entityIDs.emplace_back(id); }
    /*!
        Callback function which is invoked when an entity gets detached.

        \param index The local entity index. The lowest level instance should convert the entity
        identifier to an index before calling this function.
     */
    virtual void handle_detach_entity(gfx_entity_id index)
    {
        GO_ASSERT(index < m_entityIDs.size());
        swap_erase(m_entityIDs, index);
    }
    /*!
        Translates a global entity identifier to a local index.

        \return id The entity identifier.

        \return The data index.
     */
    gfx_entity_id id_to_index(gfx_entity_id id) const
    {
        auto it = std::find(m_entityIDs.begin(), m_entityIDs.end(), id);
        return it == m_entityIDs.end() ? -1 : (it - m_entityIDs.begin());
    }
    /*!
        Converts an index to an entity identifier.

        \param index The index.

        \return The entity identifier.
     */
    gfx_entity_id index_to_id(size_t index) const  { return m_entityIDs[index]; }
private:
    //! A pointer to the scene.
    gfx_scene *m_scene;
    //! The entity identifiers.
    gfx_entity_id_vector m_entityIDs;
};

GO_END_NAMESPACE