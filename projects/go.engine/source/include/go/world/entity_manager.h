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
    \file world/entity_manager.h
    \brief Contains classes for managing a collection of entities.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/aligned.h"
#include "go/world/entity.h"
#include "go/world/entity_component_manager.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

class scene;
class entity_manager;


//! A struct, that represents the transform of an entity.
struct GO_SSE_ALIGN entity_transform
{
    /*!
        The position in world-space.
     
        The w component is set to 1.
     */
    DirectX::XMFLOAT4A position;
    //! The rotation (quaternion) in world-space.
    DirectX::XMFLOAT4A rotation;
    /*!
        The scale in world-space.

        The w component is set to 1.
     */
    DirectX::XMFLOAT4A scale;
};


typedef std::vector<entity_transform> entity_transform_vector;
typedef uint32_t entity_id;


//! A class, that holds a collection of entities.
class GO_API GO_SSE_ALIGN entity_manager : public aligned<entity_manager>
{
public:
    /*!
        Constructor.

        \param owner The owner manager.
        \param capacity Upper bound for the number of entities in the scene.
     */
    entity_manager(scene *owner, size_t capacity);
public:
    /*!
        Returns the owner of the entity manager.

        \return The owner.
     */
    scene *owner() noexcept { return m_owner; }
    /*!
        Returns the cache size of the manager.

        \return The cache size in entity count.
     */
    size_t capacity() noexcept { return m_transforms.capacity(); }
    /*!
        Returns the entity transforms.

        \return The entity transforms.
     */
    const entity_transform_vector &transforms() const noexcept { return m_transforms; }
    /*!
        Translates an entity.

        \param id The entity ID.
        \param vDelta The delta translation vector.

        \return The new entity position.
     */
    DirectX::XMVECTOR translate(entity_id id, DirectX::FXMVECTOR vDelta);
    /*!
        Changes the position of an entity.

        \param id The entity ID.
        \param vValue The new entity position.
     */
    void change_position(entity_id id, DirectX::FXMVECTOR vValue);
    /*!
        Rotates an entity.

        \param id The entity ID.
        \param vDeltaQuat The delta quaternion.

        \return The new entity rotation.
     */
    DirectX::XMVECTOR rotate(entity_id id, DirectX::FXMVECTOR vDeltaQuat);
    //! \overload
    DirectX::XMVECTOR rotate(entity_id id, float pitch, float yaw, float roll);
    /*!
        Changes the rotation of an entity.

        \param id The entity ID.
        \param vQuat The new entity rotation.
     */
    void change_rotation(entity_id id, DirectX::FXMVECTOR vQuat);
    /*!
        Scales an entity.

        \param id The entity ID.
        \param vDelta The delta scale vector.

        \return The new entity scale.
     */
    DirectX::XMVECTOR scale(entity_id id, DirectX::FXMVECTOR vDelta);
    /*!
        Changes the scale of an entity.

        \param id The entity ID.
        \param vValue The new entity scale.
     */
    void change_scale(entity_id id, DirectX::FXMVECTOR vValue);
    /*!
        Transforms an entity.

        \param id The entity ID.
        \param vDeltaPos The delta position vector.
        \param vDeltaQuat The delta rotation quaternion.
        \param vDeltaScale The delta scale vector.
     */
    void transform(entity_id id, DirectX::FXMVECTOR vDeltaPos, DirectX::FXMVECTOR vDeltaQuat, DirectX::FXMVECTOR vDeltaScale);
    /*!
        Changes the transform of an entity.

        \param id The entity ID.
        \param vPosition The position vector.
        \param vQuat The rotation quaternion.
        \param vScale The scale vector.
     */
    void change_transform(entity_id id, DirectX::FXMVECTOR vPosition, DirectX::FXMVECTOR vQuat, DirectX::FXMVECTOR vScale);
private:
    //! The owner of the manager.
    scene *m_owner;
    //! The entity transforms.
    entity_transform_vector m_transforms;
};

GO_END_NAMESPACE