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
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

//! The texture converter property.
extern std::string config_prop_texconv;
//! The project world directory.
extern std::string config_prop_worlddir;
//! The project models folder.
extern std::string config_prop_mdlfolder;
//! The project animation folder.
extern std::string config_prop_anmfolder;
//! The project texture folder.
extern std::string config_prop_texfolder;
//! The model database name.
extern std::string config_prop_mdldb;
//! The texture database name.
extern std::string config_prop_texdb;
//! The material database name.
extern std::string config_prop_mtldb;
//! The animation database name.
extern std::string config_prop_anmdb;

//! A class, which allows loading of the application configuration.
class config_file
{
public:
    //! Constructor.
    config_file() = default;
    //! \overload
    config_file(const go::file_path &filePath);
public:
    //! The application file path.
    go::file_path applicationFilePath;
    //! The application directory path.
    go::file_path applicationDirPath;
    /*!
        Load the configuration from disk.

        \param filePath The file path.
     */
    void load(const go::file_path &filePath);
    /*!
        Queries the value of a property.

        \param propName The property name.

        \return The value of the property.
     */
    std::string query(const std::string &propName) const;
private:
    //! The settings properties.
    std::unordered_map<std::string, std::string> m_properties;
};

GAT_END_NAMESPACE