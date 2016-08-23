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
#include "gat/files/config_file.h"


GAT_BEGIN_NAMESPACE

//! A structure, which represents a base for all GAT tools.
struct tool_base
{
    /*!
        Returns a tool by name.

        \param name The tool name.

        \return The tool or nullptr, if no matching tool was found.
     */
    static tool_base *from_name(const _TCHAR *name);
    /*!
        Constructor.

        \param name The name of the tool.
     */
    tool_base(const _TCHAR *name);
    /*!
        Callback function, which is invoked to run the tool.

        \param argc The argument count.
        \param argv The argument vector.
        \param cfg The application configuration.
     */
    virtual void run(int argc, const _TCHAR *argv[], const config_file &cfg) = 0;
};

GAT_END_NAMESPACE


/*!
    Declares a class as a GAT tool.

    \param ClassName The class name.
 */
#define GAT_DECLARE_TOOL(ClassName) public: static const _TCHAR *kConverterName; ClassName();
/*!
    Defines a GAT tool.

    \param ClassName The class name.
    \param Name The name of the tool.
 */
#define GAT_DEFINE_TOOL(ClassName, Name) const _TCHAR *gat::ClassName::kConverterName = Name; \
    gat::ClassName::ClassName() : gat::tool_base(gat::ClassName::kConverterName) {}
/*!
    Registers a GAT tool.

    \param ClassName The class name.
 */
#define GAT_REGISTER_TOOL(ClassName, ...) std::unique_ptr<gat::ClassName> s_##ClassName = std::make_unique<gat::ClassName>(__VA_ARGS__); 