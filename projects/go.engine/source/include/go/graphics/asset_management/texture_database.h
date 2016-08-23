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
    \file graphics/asset_management/texture_database.h
    \brief Contains classes for managing different textures.

    \author Gokhan Ozdogan
    \version 1.0
*/


#pragma once


#include "go/core/common.h"
#include "go/asset_management/asset_database.h"


GO_BEGIN_NAMESPACE

//! A structure, which represents a material.
struct gfx_texture_definition : public asset_record
{
    //! The texture file path.
    file_path filePath;
};


//! A class, which manages a set of textures.
class GO_API gfx_texture_database : public asset_database<gfx_texture_definition>
{
public:
    //! Constructor.
    gfx_texture_database();
protected:
    void process_record(rapidxml::xml_node<> *n, gfx_texture_definition &def) override;
};

GO_END_NAMESPACE
