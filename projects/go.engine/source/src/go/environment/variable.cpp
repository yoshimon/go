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


#include "go/environment/variable.h"


#include "go/core/log.h"


// ============================================================================== //
// go::variable member functions
// ============================================================================== //

go::variable::variable(std::string name, std::string val, std::string description,
    uint32_t flags, variable::observer_type *observer)
    : m_name(std::move(name)), m_value(std::move(val)), m_description(std::move(description)), m_flags(flags)
{
    if(observer)
    {
        add_observer(observer);
    }
}


go::variable::variable(variable &&other)
{
    *this = std::move(other);
}


go::variable &go::variable::operator=(variable &&other)
{
    m_flags = other.m_flags;
    m_description = std::move(other.m_description);
    m_name = std::move(other.m_name);
    m_value = std::move(other.m_value);
    return *this;
}


const std::string &go::variable::name() const noexcept
{
    return m_name;
}


const std::string &go::variable::description() const noexcept
{
    return m_description;
}


uint32_t go::variable::flags() const noexcept
{
    return m_flags;
}


void go::variable::set_flags(uint32_t flags) noexcept
{
    m_flags = flags;
}


const std::string &go::variable::value() const noexcept
{
    return m_value;
}


void go::variable::set_value(std::string value)
{
    m_value = std::move(value);
    on_value_changed();
}


int32_t go::variable::valuei() const
{
    return std::atoi(m_value.c_str());
}


void go::variable::set_value(int32_t value)
{
    m_value = std::to_string((_Longlong)value);
    on_value_changed();
}


float go::variable::valuef() const
{
    return (float)std::atof(m_value.c_str());
}


void go::variable::set_value(float value)
{
    m_value = std::to_string((long double)value);
    on_value_changed();
}


int64_t go::variable::valuel() const
{
    return std::atoll(m_value.c_str());
}


void go::variable::set_value(int64_t value)
{
    m_value = std::to_string(value);
    on_value_changed();
}


void go::variable::on_value_changed()
{
    GO_LOGFILE_INFO("CVar '%s' changed to '%s'.\n", m_name.c_str(), m_value.c_str());

    post_event();
}
