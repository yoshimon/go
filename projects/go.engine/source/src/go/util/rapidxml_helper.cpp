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

#include "go/util/rapidxml_helper.h"

#include "rapidxml/rapidxml_print.hpp"
#include <filesystem>
#include <fstream>


using namespace DirectX;
namespace fs = std::tr2::sys;


//===========================================================================
// Global functions
//===========================================================================

rapidxml::xml_node<> *go::xml_require_child(rapidxml::xml_node<> *node, const char *name, const char *value)
{
    name = name ? node->document()->allocate_string(name) : nullptr;
    value = value ? node->document()->allocate_string(value) : nullptr;
    auto result = node->first_node(name);
    if(!result)
    {
        result = node->document()->allocate_node(rapidxml::node_element, name, value);
        node->append_node(result);
    }
    else if(value)
    {
            result->value(value);
    }

    return result;
}


rapidxml::xml_node<> *go::xml_add_child(rapidxml::xml_node<> *node, const char *name, const char *value)
{
    name = name ? node->document()->allocate_string(name) : nullptr;
    value = value ? node->document()->allocate_string(value) : nullptr;
    auto result = node->document()->allocate_node(rapidxml::node_element, name, value);
    node->append_node(result);
    return result;
}


rapidxml::xml_node<> *go::xml_child_at(rapidxml::xml_node<> *node, size_t pos, const char *filter)
{
    auto n = node->first_node(filter);
    while(pos > 0 && n)
    {
        --pos;
        n = n->next_sibling(filter);
    }

    return n;
}


rapidxml::xml_node<> *go::xml_insert_child(rapidxml::xml_node<> *node, rapidxml::xml_node<> *pos,
    const char *name, const char *value)
{
    name = name ? node->document()->allocate_string(name) : nullptr;
    value = value ? node->document()->allocate_string(value) : nullptr;
    auto result = node->document()->allocate_node(rapidxml::node_element, name, value);
    node->insert_node(pos, result);
    return result;
}


rapidxml::xml_attribute<> *go::xml_require_attribute(rapidxml::xml_node<> *node, const char *name, const char *value)
{
    auto doc = node->document();
    name = name ? doc->allocate_string(name) : nullptr;
    value = value ? doc->allocate_string(value) : nullptr;
    auto result = node->first_attribute(name);
    if(!result)
    {
        result = doc->allocate_attribute(name, value);
        node->append_attribute(result);
    }
    else if(value)
    {
        result->value(value);
    }

    return result;
}


rapidxml::xml_attribute<> *go::xml_require_bool_attribute(rapidxml::xml_node<> *node, const char *name, bool value)
{
    return go::xml_require_attribute(node, name, value ? "true" : "false");
}


bool go::xml_get_bool_attribute(rapidxml::xml_node<> *node, const char *name,
    bool &outValue, bool defaultValue)
{
    auto n = node ? node->first_attribute(name) : nullptr;
    if(!n)
    {
        outValue = defaultValue;
        return false;
    }

    outValue = !strcmp(n->value(), "true");
    return true;
}


void go::xml_save_document(rapidxml::xml_document<> &doc, const file_path &fileName)
{
    // Force-create the parent directory
    fs::create_directories(fileName.directory_path().c_str());

    // Save the actual file
    std::ofstream f(fileName);
    if(!f.is_open())
    {
        throw std::exception("Could not save XML document.");
    }
    
    f << doc;
}
