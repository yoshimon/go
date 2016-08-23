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

#include "gat/files/config_file.h"


#include "gat/util/exception.h"
#include "gat/util/file_util.h"
#include "gat/util/rapidxml_helper.h"
#include "go/golib.h"


std::string gat::config_prop_texconv = "texconv";
std::string gat::config_prop_worlddir = "worlddir";
std::string gat::config_prop_mdlfolder = "mdlfolder";
std::string gat::config_prop_anmfolder = "anmfolder";
std::string gat::config_prop_texfolder = "texfolder";
std::string gat::config_prop_mdldb = "mdldb";
std::string gat::config_prop_mtldb = "mtldb";
std::string gat::config_prop_texdb = "texdb";
std::string gat::config_prop_anmdb = "anmdb";


//===========================================================================
// gat::config_file
//===========================================================================

gat::config_file::config_file(const go::file_path &filePath)
{
    load(filePath);
}


void gat::config_file::load(const go::file_path &filePath)
{
    m_properties.clear();

    auto txtFile = gat::file_util::read_text_file(filePath);
    rapidxml::xml_document<> doc;
    try
    {
        doc.parse<0>((char *)txtFile.c_str());

        auto nRoot = doc.first_node("config");
        throw_if_not<tchar_exception>(nRoot, _T("Could not parse the configuration file."));
        
        m_properties[config_prop_texconv] = xml_require_child(nRoot, config_prop_texconv.c_str())->value();
        m_properties[config_prop_worlddir] = xml_require_child(nRoot, config_prop_worlddir.c_str())->value();
        m_properties[config_prop_mdlfolder] = xml_require_child(nRoot, config_prop_mdlfolder.c_str())->value();
        m_properties[config_prop_texfolder] = xml_require_child(nRoot, config_prop_texfolder.c_str())->value();
        m_properties[config_prop_anmfolder] = xml_require_child(nRoot, config_prop_anmfolder.c_str())->value();
        m_properties[config_prop_mdldb] = xml_require_child(nRoot, config_prop_mdldb.c_str())->value();
        m_properties[config_prop_mtldb] = xml_require_child(nRoot, config_prop_mtldb.c_str())->value();
        m_properties[config_prop_texdb] = xml_require_child(nRoot, config_prop_texdb.c_str())->value();
        m_properties[config_prop_anmdb] = xml_require_child(nRoot, config_prop_anmdb.c_str())->value();
    }
    catch(...)
    {
        throw tchar_exception(_T("Could not parse the configuration file."));
    }
}


std::string gat::config_file::query(const std::string &propName) const
{
    auto it = m_properties.find(propName);
    return it == m_properties.end() ? "" : it->second;
}