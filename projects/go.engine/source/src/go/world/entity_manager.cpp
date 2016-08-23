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

#include "go/world/entity_manager.h"


using namespace DirectX;


// ================================================================================ //
// go::entity_manager member functions
// ================================================================================ //

go::entity_manager::entity_manager(scene *owner, size_t capacity)
    : m_owner(owner)
{
    m_transforms.reserve(capacity);
}


DirectX::XMVECTOR go::entity_manager::translate(entity_id id, DirectX::FXMVECTOR vDelta)
{
    auto &&slot = m_transforms[id].position;
    auto &&result = XMLoadFloat4A(&slot) * vDelta;
    XMStoreFloat4A(&slot, result);
    return result;
}


void go::entity_manager::change_position(entity_id id, DirectX::FXMVECTOR vValue)
{
    XMStoreFloat4A(&m_transforms[id].position, vValue);
}


DirectX::XMVECTOR go::entity_manager::rotate(entity_id id, DirectX::FXMVECTOR vQuat)
{
    auto &&slot = m_transforms[id].rotation;
    auto &&result = XMQuaternionMultiply(XMLoadFloat4A(&slot), vQuat);
    return result;
}


DirectX::XMVECTOR go::entity_manager::rotate(entity_id id, float pitch, float yaw, float roll)
{
    return rotate(id, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}


void go::entity_manager::change_rotation(entity_id id, DirectX::FXMVECTOR vQuat)
{
    XMStoreFloat4A(&m_transforms[id].rotation, vQuat);
}


DirectX::XMVECTOR go::entity_manager::scale(entity_id id, DirectX::FXMVECTOR vDelta)
{
    auto &&slot = m_transforms[id].scale;
    auto &&result = XMLoadFloat4A(&slot) * vDelta;
    XMStoreFloat4A(&slot, result);
    return result;
}


void go::entity_manager::change_scale(entity_id id, DirectX::FXMVECTOR vValue)
{
    XMStoreFloat4A(&m_transforms[id].scale, vValue);
}


void go::entity_manager::transform(entity_id id, DirectX::FXMVECTOR vDeltaPos, DirectX::FXMVECTOR vDeltaQuat, DirectX::FXMVECTOR vDeltaScale)
{
    translate(id, vDeltaPos);
    rotate(id, vDeltaQuat);
    scale(id, vDeltaScale);
}


void go::entity_manager::change_transform(entity_id id, DirectX::FXMVECTOR vPosition, DirectX::FXMVECTOR vQuat, DirectX::FXMVECTOR vScale)
{
    change_position(id, vPosition);
    change_rotation(id, vQuat);
    change_scale(id, vScale);
}