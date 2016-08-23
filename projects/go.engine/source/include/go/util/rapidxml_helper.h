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


#include "go/core/common.h"
#include "go/core/path_utility.h"
#include "rapidxml/rapidxml.hpp"
#include <DirectXMath.h>


GO_BEGIN_NAMESPACE

/*!
    Requires the existance of a child node.

    Creates a new node if no matching node exists.

    \param node The node to query.
    \param name The name of the child node.
    \param value The child value. Only applies if the child node does not yet exist.

    \return The child node.
 */
GO_API rapidxml::xml_node<> *xml_require_child(rapidxml::xml_node<> *node, const char *name, const char *value = nullptr);
/*!
    Adds a new child node.

    \param node The node to query.
    \param name The name of the child node.
    \param value The child value. Only applies if the child node does not yet exist.

    \return The child node.
 */
GO_API rapidxml::xml_node<> *xml_add_child(rapidxml::xml_node<> *node, const char *name, const char *value = nullptr);
/*!
    Inserts a new child node.

    \param node The node to query.
    \param pos The position to insert at.
    \param name The name of the child node.
    \param value The child value. Only applies if the child node does not yet exist.

    \return The child node.
 */
GO_API rapidxml::xml_node<> *xml_insert_child(rapidxml::xml_node<> *node, rapidxml::xml_node<> *pos, const char *name, const char *value = nullptr);
/*!
    Returns the child node at a given index.

    \param node The node to query.
    \param pos The position to go to.
    \param filter A string, which identifies which nodes to count.

    \return The child node or nullptr, if no match was found.
 */
GO_API rapidxml::xml_node<> *xml_child_at(rapidxml::xml_node<> *node, size_t pos, const char *filter);
/*!
    Requires the existance of an attribute.

    \param node The node to query.
    \param name The attribute name.
    \param value The attribute value. Only applies if the attribute does not yet exist.

    \return The attribute.
 */
GO_API rapidxml::xml_attribute<> *xml_require_attribute(rapidxml::xml_node<> *node, const char *name, const char *value = nullptr);
//! \overload
GO_API rapidxml::xml_attribute<> *xml_require_bool_attribute(rapidxml::xml_node<> *node, const char *name, bool value);
/*!
    Converts a string attribute to an enum value.

    \param node The node to query.
    \param name The attribute name.
    \param table The look-up table.

    \return True, if a match was found for the enum value. Otherwise false.
 */
template<typename Enum, typename EnumStrings, typename LookupFunc>
bool xml_get_enum_attribute(
    rapidxml::xml_node<> *node,
    const char *name,
    const EnumStrings &enumStrings,
    Enum &outValue,
    Enum defaultValue,
    const LookupFunc &lookupFunc)
{
    auto n = node ? node->first_attribute(name) : nullptr;
    if(!n)
    {
        outValue = defaultValue;
        return false;
    }

    auto nodeVal = n->value();

    for(size_t i = 0; i < (size_t)Enum::count; ++i)
    {
        auto v = lookupFunc(enumStrings, i);
        if(!strcmp(v, nodeVal))
        {
            outValue = (Enum)i;
            return true;
        }
    }

    outValue = defaultValue;
    return false;
}
//! \overload
template<typename Enum>
bool xml_get_enum_attribute(
    rapidxml::xml_node<> *node,
    const char *name,
    const char *const *enumStrings,
    Enum &outValue,
    Enum defaultValue = (Enum)0)
{
    return xml_get_enum_attribute(node, name, enumStrings, outValue, defaultValue, []
        (const char *const *enumStrings, size_t i)
    {
        return enumStrings[i];
    });
}
/*!
    
 */
template<typename T, typename ConverterFunc>
bool xml_get_node_value(rapidxml::xml_node<> *node, const char *name,
    T &outValue, const T &defaultValue, const ConverterFunc &converterFunc)
{
    auto n = node ? node->first_node(name) : nullptr;
    if(!n)
    {
        outValue = defaultValue;
        return false;
    }

    outValue = static_cast<T>(converterFunc(n->value()));
    return true;
}
/*!
    Converts an attribute to a boolean value.

    \param node The node to query.
    \param name The name of the attribute to query.
    \param outValue The output value.
    \param defaultValue The default value.
    
    \return True, if the attribute exists. Otherwise false.
 */
GO_API bool xml_get_bool_attribute(
    rapidxml::xml_node<> *node,
    const char *name,
    bool &outValue,
    bool defaultValue);
/*!
    Saves an XML document to a file.

    \param doc The XML document.
    \param filePath The filepath.
 */
GO_API void xml_save_document(rapidxml::xml_document<> &doc, const file_path &filePath);

GO_END_NAMESPACE
