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
    \file design/tree.h
    \brief Contains a class which represents a generic tree.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

/*!
    A class, which represents the base class for all tree nodes.

    \tparam Payload The type of the payload.
 */
template<typename DerivedNode, typename Id, typename Payload>
class tree_base_node
{
public:
    typedef Payload payload_t;
    typedef payload_t* payload_ptr_t;
    typedef Id id_t;
    typedef id_t& id_ref_t;
    typedef const id_t &id_cref_t;
    typedef DerivedNode node_t;
    typedef std::vector<std::shared_ptr<node_t>> child_node_vector;
public:
    /*!
        Constructor.

        \param id The node identifier.
        \param parent The parent node.
        \param attachment The entity attachment.
     */
    tree_base_node(id_t id, node_t *parent, payload_ptr_t attachment);
public:
    /*!
        Returns the name of the entity.

        \return The name of the entity.
     */
    id_cref_t id() const;
    /*!
        Attaches a payload to the node.

        \param attachment The new attachment.
     */
    void change_payload(payload_ptr_t attachment);
    //! Detaches the current payload, if one is bound.
    void detach_payload();
    /*!
        Returns the node payload.

        \return The payload or nullptr, if no payload is attached.
     */
    payload_ptr_t payload() const;
    /*!
        Returns the parent node.

        \return The parent node or nullptr, if the node has no parent.
     */
    node_t *parent() const;
    /*!
        Returns a list of child nodes.

        \return The list of child nodes.
     */
    const child_node_vector &children() const;
    /*!
        Destroys a child node.

        \param node The child node to destroy.
     */
    void destroy_child(node_t *node);
    //! \overload
    void destroy_child(id_cref_t id);
public:
    /*!
        Renames the node.

        \param id The new node identifier.
     */
    void internal_rename(id_t id);
    /*!
        Moves all children to the parent node.

        \param node The node to move from.
     */
    void internal_move_children(node_t *node);
    /*!
        Attaches a child to the node.

        \param node The node to attach.
     */
    void internal_attach_child(node_t *node);
private:
    //! The parent node.
    node_t *m_parent;
    //! The child nodes.
    child_node_vector m_children;
    //! The node payload.
    payload_ptr_t m_payload;
    //! The name of the entity.
    id_t m_id;
};


/*!
    A class, which represents a generic tree.

    \tparam DerivedNode The type of the derived node class.
 */
template<typename DerivedNode>
class tree
{
    GO_DECLARE_NON_COPYABLE(tree)
public:
    //! The type of the payload per node.
    typedef DerivedNode node_t;
    //! The type of a node pointer.
    typedef node_t* node_ptr_t;
    //! Enumeration of possible values for the destroy node function.
    enum class destroy_flags_t
    {
        //! Destroys only the specified node and attaches its children to its parent.
        destroy_this,
        //! Destroys the node and all of its children.
        destroy_children
    };
public:
    /*!
        Constructor.

        \param rootNodeId The root node identifier.
     */
    tree(typename node_t::id_t rootNodeId);
    //! Destructor.
    virtual ~tree();
public:
    /*!
        Creates a new scene node.

        \param id The node identifier.
        \param parent The parent node or nullptr, to attach the node to the root node.
        \param payload Optional. Payload to attach to the node.

        \return If the node was successfully created, the return value is a pointer to that node.
        Otherwise, the return value is nullptr.
     */
    node_ptr_t create_node(typename node_t::id_t id, node_ptr_t parent = nullptr, typename node_t::payload_ptr_t payload = nullptr);
    /*!
        Searches for a node.

        \param id The identifier of the node to search for.

        \return If the node was found, the return value is a pointer to that node. Otherwise,
        the return value is nullptr.
    */
    node_ptr_t find_node(typename node_t::id_cref_t id);
    /*!
        Renames an existing node.

        \param node The node to rename.
        \param newId The new node identifier.
     */
    bool rename_node(node_ptr_t node, typename node_t::id_t newId);
    //! \overload
    bool rename_node(typename node_t::id_t node, typename node_t::id_t newId);
    /*!
        Destroys an existing node.

        \param node The node to destroy.
        \param flags Optional deletion flag.
     */
    virtual void destroy_node(node_ptr_t node, destroy_flags_t flags = destroy_flags_t::destroy_children);
    /*!
        Returns the root node.

        \return The root node.
     */
    node_ptr_t root_node() const;
private:
    //! \overload
    node_ptr_t find_node(node_ptr_t currentNode, typename node_t::id_cref_t id);
private:
    //! The root node.
    node_t m_rootNode;
};


#include "tree_impl.h"

GO_END_NAMESPACE
