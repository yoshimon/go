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

#include "gat/defines.h"


GAT_BEGIN_NAMESPACE

/*!
    Calculates the axis-aligned bounding box for a set of vertices.

    \param vertices The vertex set.
    \param[in,out] vBBMin The first corner of the AABB.
    \param[in,out] vBBMax The opposite corner of the AABB.
 */
template<class VertexContainer>
inline void calculate_vertex_aabb(const VertexContainer &vertices, DirectX::XMVECTOR &vBBMin,
    DirectX::XMVECTOR &vBBMax)
{
    for(auto &v : vertices)
    {
        auto vPos = DirectX::XMLoadFloat3(&v.position);
        vBBMin = DirectX::XMVectorMin(vBBMin, vPos);
        vBBMax = DirectX::XMVectorMax(vBBMax, vPos);
    }
}

GAT_END_NAMESPACE