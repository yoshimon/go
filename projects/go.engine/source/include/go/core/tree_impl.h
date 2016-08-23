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


// ================================================================================ //
// tree member functions
// ================================================================================ //

template<typename Node, int32_t MaxNodes>
tree<Node, MaxNodes>::tree()
    : m_usedNodes(0)
{
}


template<typename Node, int32_t MaxNodes>
auto tree<Node, MaxNodes>::insert(node_t *parent) -> node_t *
{
    if(m_usedNodes >= MaxNodes)
    {
        return nullptr;
    }

    auto n = &m_nodes[m_usedNodes];
    
    if(parent)
    {
        std::swap(parent->firstChild, n->nextSibling);
    }

    n->firstChild = nullptr;
    n->parent = parent;

    ++m_usedNodes;
    return n;
}


template<typename Node, int32_t MaxNodes>
int32_t tree<Node, MaxNodes>::used_nodes() const
{
    return m_usedNodes;
}


template<typename Node, int32_t MaxNodes>
auto tree<Node, MaxNodes>::first_node() const -> node_t *
{
    return m_usedNodes ? m_nodes : nullptr;
}
