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
    \file world/light_manager.h
    \brief Contains classes for managing a collection of lights.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/world/entity.h"
#include "go/world/entity_component_manager.h"
#include "go/world/lights.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

class scene;
class entity_manager;


/*!
    A structure, that holds information about a light.

    \tparam Desc The light descriptor.
 */
template<typename Desc>
struct light_data
{
    //! The target entity.
    entity_id entityID;
    //! The light descriptor.
    Desc descriptor;
};

typedef light_data<spherical_light_desc> spherical_light_data;
typedef std::vector<spherical_light_data> spherical_light_data_vector;

typedef light_data<disk_light_desc> disk_light_data;
typedef std::vector<disk_light_data> disk_light_data_vector;


//! A class, that holds a collection of entities.
class GO_API light_manager
{
public:
    /*!
        Constructor.

        \param owner The owner manager.
     */
    light_manager(scene *owner);
public:
    /*!
        Returns the owner of the light manager.

        \return The owner.
     */
    scene *owner() noexcept { return m_owner; }
    //! Computes all visible lights and caches the results.
    void update_visible_lights();
    /*!
        Returns the directional light descriptor.

        \return The directional light descriptor.
     */
    directional_light_desc &directional_light();
    /*!
        Attaches a spherical light to an entity.

        \param entityID The entity ID.
        \param desc The light descriptor.
     */
    void attach_spherical_light(entity_id entityID, const spherical_light_desc &desc);
    /*!
        Detaches a spherical light from an entity.

        \param entityID The entity ID.
     */
    void detach_spherical_light(entity_id entityID);
    /*!
        Returns the spherical light data for an entity.

        \param entityID The entity ID.

        \return The spherical light data.
     */
    const spherical_light_data &find_spherical_light(entity_id entityID) const noexcept;
    /*!
        Returns whether a spherical light is already attached to an entity.

        \return True, if the entity already has a spherical light. Otherwise false.
     */
    bool has_spherical_light(entity_id entityID) const noexcept;
    /*!
        Returns the data for spherical lights.

        \return The spherical light data.
     */
    const spherical_light_data_vector &spherical_lights() const noexcept { return m_sphericalLights; }
    /*!
        Attaches a disk light to an entity.

        \param entityID The entity ID.
        \param desc The light descriptor.
     */
    void attach_disk_light(entity_id entityID, const disk_light_desc &desc);
    /*!
        Detaches a disk light from an entity.

        \param entityID The entity ID.
     */
    void detach_disk_light(entity_id entityID);
    /*!
        Returns the disk light data for an entity.

        \param entityID The entity ID.

        \return The disk light data.
     */
    const disk_light_data &find_disk_light(entity_id entityID) const noexcept;
    /*!
        Returns whether a disk light is already attached to an entity.

        \return True, if the entity already has a disk light. Otherwise false.
     */
    bool has_disk_light(entity_id entityID) const noexcept;
    /*!
        Returns the data for disk lights.

        \return The disk light data.
     */
    const disk_light_data_vector &disk_lights() const noexcept { return m_diskLights; }
private:
    //! Updates the directional light frustum to fit the active camera frustum.
    void update_directional_light_frustum();
    //! Culls the shadow casting spherical lights.
    void cull_shadow_casting_spherical_lights();
    //! Culls the shadow casting disk lights.
    void cull_shadow_casting_disk_lights();
    /*!
        Culls lights with a spherical proxy.

        \param 
     */
    void cull_spherical_proxy_lights();
private:
    //! The owner of the manager.
    scene *m_owner;
    
    //! Maps entity IDs to spherical light indices.
    entity_id_vector m_entityToSphericalLightIndex;
    //! The light data for spherical lights.
    spherical_light_data_vector m_sphericalLights;
    //! The spherical lights after culling.
    index_vector m_visibleSphericalLights;

    //! The visible shadow-casting point lights.
    index_vector m_visibleShadowCastingPointLights;

    //! Maps entity IDs to disk light indices.
    entity_id_vector m_entityToDiskLightIndex;
    //! The light data for disk lights.
    disk_light_data_vector m_diskLights;
};

GO_END_NAMESPACE