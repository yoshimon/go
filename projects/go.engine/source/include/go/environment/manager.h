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
    \file environment/manager.h
    \brief Contains a class for managing runtime variables.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/design/singleton.h"
#include "go/core/path_utility.h"


GO_BEGIN_NAMESPACE

class variable;


//! A class, which manages runtime variables.
class GO_API environment_manager : public singleton<environment_manager>
{
    GO_DECLARE_NON_COPYABLE(environment_manager)
public:
    //! Result of a parsing process.
    enum class parse_result_t
    {
        //! No errors.
        ok,
        //! Unknown command name.
        unknown_command,
        //! Missing variable name.
        missing_variable,
        //! Invalid variable name.
        invalid_variable,
        //! Missing variable value.
        missing_value
    };
    typedef std::unordered_map<std::string, variable *> variable_map;
public:
    //! Constructor.
    environment_manager();
public:
    /*!
        Adds a variable.

        \param variable The variable to add.

        \return If the variable is successfully added, then the return value is true.
        Otherwise, the return value is false.
     */
    bool add_variable(variable *var);
    /*!
        Finds a variable.

        \param name The name of the variable.

        \return The variable matching the name specified by <i>name</i>.
        If no such variable exists, the return value is nullptr.
     */
    variable *find(const std::string &name);
    /*!
        Writes the persistent engine configuration to a file.

        \param name The name of the file.
     */
    void write_file(const file_path &name = GO_VARIABLES_CONFIG_FILE) const;
    /*!
        Reads the engine configuration from a file.

        \param name The name of the file.
     */
    void read_file(const file_path &name = GO_VARIABLES_CONFIG_FILE);
    /*!
        Executes a command.

        \param command The command to execute.

        \return The result of the parsing operation.
     */
    parse_result_t execute_command(std::string command);
    /*!
        Returns the variable map.

        \return The variable map.
     */
    const variable_map &variables() const noexcept;
private:
    //! A list of strings.
    typedef std::vector<std::string> string_list;
private:
    //! \overload
    void read_file(const file_path &name, string_list &visitedFiles);
    //! \overload
    parse_result_t execute_command(std::string command, string_list &visitedFiles);
private:
    //! Maps variable names to variable instances.
    variable_map m_variableMap;
};


//! Global environment manager instance.
extern GO_API environment_manager *the_environment;

GO_END_NAMESPACE
