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
// tree::node_t member functions
// ================================================================================ //

template<typename DerivedNode, typename Id, typename Payload>
go::tree_base_node<DerivedNode, Id, Payload>::tree_base_node(id_t id, node_t *parent, payload_ptr_t attachment)
: m_id(id), m_parent(parent), m_payload(attachment)
{
}


template<typename DerivedNode, typename Id, typename Payload>
auto go::tree_base_node<DerivedNode, Id, Payload>::id() const -> id_cref_t
{
    return m_id;
}


template<typename DerivedNode, typename Id, typename Payload>
void go::tree_base_node<DerivedNode, Id, Payload>::change_payload(payload_ptr_t attachment)
{
    if(attachment)
    {
        attachment->node->detach_payload();
    }

    m_payload = attachment;
}


template<typename DerivedNode, typename Id, typename Payload>
void go::tree_base_node<DerivedNode, Id, Payload>::detach_payload()
{
    change_payload(nullptr);
}


template<typename DerivedNode, typename Id, typename Payload>
auto go::tree_base_node<DerivedNode, Id, Payload>::payload() const -> payload_ptr_t
{
    return m_payload;
}


template<typename DerivedNode, typename Id, typename Payload>
auto go::tree_base_node<DerivedNode, Id, Payload>::parent() const -> node_t *
{
    return m_parent;
}


template<typename DerivedNode, typename Id, typename Payload>
auto go::tree_base_node<DerivedNode, Id, Payload>::children() const -> const child_node_vector &
{
    return m_children;
}


template<typename DerivedNode, typename Id, typename Payload>
void go::tree_base_node<DerivedNode, Id, Payload>::internal_rename(id_t id)
{
    m_id = std::move(id);
}


template<typename DerivedNode, typename Id, typename Payload>
void go::tree_base_node<DerivedNode, Id, Payload>::destroy_child(node_t *node)
{
    m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
        [&](std::shared_ptr<node_t> &n)
    {
        return n.get() == node;
    }), m_children.end());
}


template<typename DerivedNode, typename Id, typename Payload>
void go::tree_base_node<DerivedNode, Id, Payload>::destroy_child(id_cref_t id)
{
    m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
        [&](std::shared_ptr<node_t> &n)
    {
        return n->id() == id;
    }), m_children.end());
}



template<typename DerivedNode, typename Id, typename Payload>
void go::tree_base_node<DerivedNode, Id, Payload>::internal_move_children(node_t *node)
{
    for(auto &&c : node->m_children)
    {
        m_children.push_back(std::move(c));
    }

    node->m_children.clear();
}


template<typename DerivedNode, typename Id, typename Payload>
void go::tree_base_node<DerivedNode, Id, Payload>::internal_attach_child(node_t *node)
{
    m_children.emplace_back(node);
}


// ================================================================================ //
// implicit_edge_graph member functions
// ================================================================================ //

template<typename DerivedNode>
go::tree<DerivedNode>::tree(typename node_t::id_t rootNodeId)
: m_rootNode(std::move(rootNodeId), nullptr, nullptr)
{
}


template<typename DerivedNode>
go::tree<DerivedNode>::~tree()
{
}


template<typename DerivedNode>
auto go::tree<DerivedNode>::create_node(typename node_t::id_t id,
    node_ptr_t parent, typename node_t::payload_ptr_t attachment) -> node_ptr_t
{
    if(!parent)
    {
        parent = &m_rootNode;
    }

    if(find_node(id))
    {
        return nullptr;
    }

    auto node = new node_t(std::move(id), parent, attachment);
    parent->internal_attach_child(node);
    return node;
}


template<typename DerivedNode>
auto go::tree<DerivedNode>::find_node(typename node_t::id_cref_t id) -> node_ptr_t
{
    return find_node(&m_rootNode, id);
}


template<typename DerivedNode>
auto go::tree<DerivedNode>::find_node(node_ptr_t currentNode,
    typename node_t::id_cref_t id) -> node_ptr_t
{
    if(id == currentNode->id())
    {
        return currentNode;
    }

    for(auto &n : currentNode->children())
    {
        auto f = find_node(n.get(), id);
        if(f)
        {
            return f;
        }
    }

    return nullptr;
}


template<typename DerivedNode>
void go::tree<DerivedNode>::destroy_node(node_ptr_t node, destroy_flags_t flags)
{
    if(!node || node == &m_rootNode)
    {
        return;
    }

    switch(flags)
    {
    case destroy_flags_t::destroy_children:
        node->parent()->destroy_child(node);
        break;
    case destroy_flags_t::destroy_this:
        node->parent()->internal_move_children(node);
        node->parent()->destroy_child(node);
        break;
    }
}


template<typename DerivedNode>
auto go::tree<DerivedNode>::root_node() const -> node_ptr_t
{
    return const_cast<const node_ptr_t>(&m_rootNode);
}


template<typename DerivedNode>
bool go::tree<DerivedNode>::rename_node(typename node_t::id_t node, typename node_t::id_t newId)
{
    return rename_node(find_node(node), newId);
}


template<typename DerivedNode>
bool go::tree<DerivedNode>::rename_node(node_ptr_t node, typename node_t::id_t newId)
{
    if(!node || find_node(newId))
    {
        return false;
    }

    node->internal_rename(std::move(newId));
    return true;
}
