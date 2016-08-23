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
    \file environment/variable.h
    \brief Contains a class, which represents a runtime variable.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/stdafx.h"
#include "go/design/observer.h"


GO_BEGIN_NAMESPACE

//! A class, which represents a runtime variable.
class GO_API variable : public observable<variable>
{
public:
    //! The variable should not be saved across different executions of the engine.
    static constexpr uint32_t nonpersistent = (1 << 0);
    //! The variable cannot be deleted by the user.
    static constexpr uint32_t permanent = (1 << 1);
public:
    /*!
        Constructs the object and initializes it.

        \param name The name of the variable.
        \param val The value of the variable.
        \param description The variable description.
        \param flags The variable flags.
        \param[in] observer An observer that is invoked when the variable is modified.
     */
    variable
    (
        std::string name,
        std::string val,
        std::string description,
        uint32_t flags,
        observer_type *observer = nullptr
    );
    //! \overload
    variable(variable &&other);
public:
    /*!
        Move-assignment operator.

        \param other The object to move.

        \return The object being moved to.
     */
    variable &operator=(variable &&other);
public:
    /*!
        Returns the variable name.

        \return The name of the variable.
     */
    const std::string &name() const noexcept;
    /*!
        Returns the variable description.

        \return The description of the variable.
     */
    const std::string &description() const noexcept;
    /*!
        Returns the variable flags.

        \return The variable flags.
     */
    uint32_t flags() const noexcept;
    /*!
        Sets the variable flags.

        \param flags The new variable flags.
     */
    void set_flags(uint32_t flags) noexcept;
    /*!
        Returns the variable value as a string.

        \return The value of the variable as a string.
     */
    const std::string &value() const noexcept;
    /*!
        Sets the variable value as a string.

        \param value The new variable value.
     */
    void set_value(std::string value);
    /*!
        Returns the variable value as an integer.

        \return The value of the variable as an integer.
     */
    int32_t valuei() const;
    /*!
        Sets the variable value as an integer.

        \param value The new variable value.
     */
    void set_value(int32_t value);
    //! \overload
    int64_t valuel() const;
    //! \overload
    void set_value(int64_t value);
    /*!
        Returns the variable value as a floating-point number.

        \return The value of the variable as a floating-point number.
     */
    float valuef() const;
    /*!
        Returns the variable value as a floating-point number.

        \param value The new variable value.
     */
    void set_value(float value);
private:
    //! Function to handle value changes.
    void on_value_changed();
private:
    //! The name of the variable.
    std::string m_name;
    //! The value of the variable.
    std::string m_value;
    //! The description of the variable.
    std::string m_description;
    //! Variable flags.
    uint32_t m_flags;
};

GO_END_NAMESPACE
