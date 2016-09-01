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

#include "go/core/performance_counter.h"
#include "tbb/parallel_for.h"


using namespace DirectX;


// ================================================================================ //
// go::entity_manager member functions
// ================================================================================ //

go::entity_manager::entity_manager(scene *owner, size_t capacity)
    : m_owner(owner)
{
    m_transforms[0].reserve(capacity);
    m_transforms[1].reserve(capacity);
}


auto go::entity_manager::create(const entity_transform *transform) -> entity_id
{
    // Initialize the data for the new entity
    if(transform)
    {
        auto &&t = *transform;
        m_transforms[0].emplace_back(t);
        m_transforms[1].emplace_back(t);
    }
    else
    {
        entity_transform t;
        t.position = XMFLOAT4A(0.0f, 0.0f, 0.0f, 1.0f);
        t.rotation = XMFLOAT4A(0.0f, 0.0f, 0.0f, 1.0f);
        t.scale = XMFLOAT4A(1.0f, 1.0f, 1.0f, 1.0f);
        m_transforms[0].emplace_back(t);
        m_transforms[1].emplace_back(t);
    }

    // TODO: create a link between this entity and its parent (if one exists)

    return m_transforms[0].size() - 1;
}


void go::entity_manager::destroy(entity_id id)
{
    // FIRE EVENT: DELETING ENTITY

    swap_erase(m_transforms[0], id);
    swap_erase(m_transforms[1], id);

    // FIRE EVENT: ENTITY DELETED
}


DirectX::XMVECTOR go::entity_manager::translate(entity_id id, DirectX::FXMVECTOR vDelta)
{
    auto &&slot = m_transforms[1][id].position;
    auto &&result = XMLoadFloat4A(&slot) * vDelta;
    XMStoreFloat4A(&slot, result);
    return result;
}


void go::entity_manager::change_position(entity_id id, DirectX::FXMVECTOR vValue)
{
    XMStoreFloat4A(&m_transforms[1][id].position, vValue);
}


DirectX::XMVECTOR go::entity_manager::rotate(entity_id id, DirectX::FXMVECTOR vQuat)
{
    auto &&slot = m_transforms[1][id].rotation;
    auto &&result = XMQuaternionMultiply(XMLoadFloat4A(&slot), vQuat);
    return result;
}


DirectX::XMVECTOR go::entity_manager::rotate(entity_id id, float pitch, float yaw, float roll)
{
    return rotate(id, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}


void go::entity_manager::change_rotation(entity_id id, DirectX::FXMVECTOR vQuat)
{
    XMStoreFloat4A(&m_transforms[1][id].rotation, vQuat);
}


DirectX::XMVECTOR go::entity_manager::scale(entity_id id, DirectX::FXMVECTOR vDelta)
{
    auto &&slot = m_transforms[1][id].scale;
    auto &&result = XMLoadFloat4A(&slot) * vDelta;
    XMStoreFloat4A(&slot, result);
    return result;
}


void go::entity_manager::change_scale(entity_id id, DirectX::FXMVECTOR vValue)
{
    XMStoreFloat4A(&m_transforms[1][id].scale, vValue);
}


void go::entity_manager::transform(entity_id id, DirectX::FXMVECTOR vDeltaPos, DirectX::FXMVECTOR vDeltaQuat, DirectX::FXMVECTOR vDeltaScale)
{
    scale(id, vDeltaScale);
    rotate(id, vDeltaQuat);
    translate(id, vDeltaPos);
}


void go::entity_manager::change_transform(entity_id id, DirectX::FXMVECTOR vPosition, DirectX::FXMVECTOR vQuat, DirectX::FXMVECTOR vScale)
{
    change_scale(id, vScale);
    change_rotation(id, vQuat);
    change_position(id, vPosition);
}


void go::entity_manager::update() noexcept
{
    std::copy(m_transforms[1].begin(), m_transforms[1].end(), m_transforms[0].begin());
}


void go::entity_manager::interpolate_transforms() noexcept
{
    tbb::parallel_for(0, (int)m_transforms[0].size(), [this](int i)
    {
        auto &&t0 = m_transforms[1][i];
        auto &&t1 = m_transforms[0][i];

        auto &&vPosT0 = XMLoadFloat4A(&t0.position);
        auto &&vRotT0 = XMLoadFloat4A(&t0.rotation);
        auto &&vScaleT0 = XMLoadFloat4A(&t0.scale);

        auto &&vPosT1 = XMLoadFloat4A(&t1.position);
        auto &&vRotT1 = XMLoadFloat4A(&t1.rotation);
        auto &&vScaleT1 = XMLoadFloat4A(&t1.scale);

        auto &&t = go::performance.blendFactor;

        auto &&vPos = XMVectorLerp(vPosT0, vPosT1, t);
        auto &&vRot = XMQuaternionSlerp(vRotT0, vRotT1, t);
        auto &&vScale = XMVectorLerp(vScaleT0, vScaleT1, t);

        auto &&td = m_interpolatedTransforms[i];
        XMStoreFloat4A(&td.position, vPos);
        XMStoreFloat4A(&td.rotation, vRot);
        XMStoreFloat4A(&td.scale, vScale);

        auto &&mTransform = XMMatrixAffineTransformation(vScale, g_XMIdentityR3, vRot, vPos);
        XMStoreFloat4x4A(&m_interpolatedTransformMatrices[i], mTransform);
    });
}