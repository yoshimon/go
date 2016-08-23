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


// ================================================================================ //
// go::light_manager member functions
// ================================================================================ //

go::light_manager::light_manager(scene *owner)
    : m_owner(owner)
{
    auto &&maxEntityCount = owner->entity_manager()->capacity();
    m_entityToSphericalLightIndex.reserve(maxEntityCount);
    m_sphericalLights.reserve(maxEntityCount);
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
    m_diskLights.push_back(disk_light_data{entityID, desc });
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
