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
    \file core/tree.h
    \brief Contains a class which represents a generic tree.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

/*!
    A structure, which represents a generic tree node.

    \tparam Data The type of the node payload.
 */
template<typename Data>
struct tree_node
{
    typedef Data data_t;
    //! The next sibling node or nullptr, if no such node exists.
    tree_node<data_t> *nextSibling;
    //! The first child node or nullptr, if no such node exists.
    tree_node<data_t> *firstChild;
    //! The parent node or nullptr, if no such node exists.
    tree_node<data_t> *parent;
    //! The data payload.
    data_t data;
};


//! A class, which represents a generic tree.
template<typename Node, int32_t MaxNodes>
class GO_API tree
{
    static_assert(MaxNodes > 0, "Requires at least one node!");
public:
    typedef Node node_t;
    //! The maximum number of nodes the tree can hold.
    constexpr int32_t kMaxNodes = MaxNodes;
public:
    //! Constructor.
    tree();
public:
    /*!
        Inserts a new node into the tree.

        \param parent The parent node or nullptr.

        \return If a new node was inserted, the return value is a pointer to that node.
        Otherwise, the return value is nullptr.
     */
    node_t *insert(node_t *parent);
    /*!
        Returns the number of used nodes.

        \return The number of used nodes.
     */
    int32_t used_nodes() const;
    /*!
        Returns the first node.

        \return The first node.
     */
    node_t *first_node() const;
private:
    //! The node array.
    node_t m_nodes[MaxNodes];
    //! The number of used nodes.
    int32_t m_usedNodes;
};


#include "tree_impl.h"

GO_END_NAMESPACE
