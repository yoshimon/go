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
#include "gat/util/exception.h"
#include "go/golib.h"
#include "rapidxml/rapidxml.hpp"


GAT_BEGIN_NAMESPACE

/*!
    Requires a child node to exist and returns a pointer to it.

    \param parent The parent node.
    \param name The name of the child node.
 */
inline rapidxml::xml_node<> *xml_require_child(rapidxml::xml_node<> *parent, const char *name)
{
    auto result = parent->first_node(name);
    if(!result)
    {
        _tprintf("Failed to query XML child node '%s'.\n", name);
        throw tchar_exception(_T("Could not query XML child node."));
    }
    return result;
}

/*!
    Requires an attribute to exist and returns a pointer to it.

    \param node The node.
    \param name The name of the child node.
 */
inline rapidxml::xml_attribute<> *xml_require_attribute(rapidxml::xml_node<> *node, const char *name)
{
    auto result = node->first_attribute(name);
    if(!result)
    {
        _tprintf("Failed to query XML node attribute '%s'.\n", name);
        throw tchar_exception(_T("Could not query XML node attribute."));
    }
    return result;
}

/*!
    Executes a function for each child node that matches a node name.

    \param parent The parent node.
    \param childNodeName The child node name.
    \param func The function to execute.
 */
inline void xml_for_each_child(rapidxml::xml_node<> *parent, const char *childNodeName,
    std::function<void(rapidxml::xml_node<> *)> func)
{
    auto child = parent->first_node(childNodeName);
    while(child)
    {
        func(child);
        child = child->next_sibling(childNodeName);
    }
}


/*!
    Reads the child node values.

    \param parent The parent node.
    \param childNodeName The child node name.
    \param out The output values.
 */
template<typename T>
inline void xml_child_node_values(rapidxml::xml_node<> *parent, const char *childNodeName, T &out)
{
    xml_for_each_child(parent, childNodeName, [&](rapidxml::xml_node<> *node) { out.push_back(node->value()); });
}


/*!
    Tries to read a value from an optional child node.

    \param parent The parent node.
    \param childNodeName The child node name.
    \param out The output value.
 */
template<typename T>
inline void xml_opt_child_node_value(rapidxml::xml_node<> *parent, const char *childNodeName, T &out)
{
    out.clear();
    auto child = parent->first_node(childNodeName);
    if(child)
    {
        out = child->value();
    }
}


/*!
    Tries to read a set of values from optional child nodes.

    \param parent The parent node.
    \param setName The name of the set to query.
    \param childNodeName The child node name.
    \param out The output values.
 */
template<typename T>
inline void xml_opt_child_set_values(rapidxml::xml_node<> *parent, const char *setName, const char *childNodeName, T &out)
{
    out.clear();
    if(setName)
    {
        auto set = parent->first_node(setName);
        if(set)
        {
            xml_child_node_values(set, childNodeName, out);
        }
    }
    else
    {
        xml_child_node_values(parent, childNodeName, out);
    }
}


/*!
    Tries to read a set of values from optional child nodes.

    \param parent The parent node.
    \param setName The name of the set to query.
    \param childNodeName The child node name.
    \param values The values to add.
 */
template<typename T>
inline void xml_add_opt_child_set_values(rapidxml::xml_node<> *parent, const char *setName, const char *childNodeName,
    const T &values)
{
    if(!values.empty())
    {
        auto set = setName ? go::xml_add_child(parent, setName) : parent;
        for(auto &val : values)
        {
            go::xml_add_child(set, childNodeName, val.c_str());
        }
    }
}

GAT_END_NAMESPACE