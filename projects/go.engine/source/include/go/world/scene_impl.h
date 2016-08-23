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


// ================================================================================ //
// scene member functions
// ================================================================================ //

template<class ComponentManager, class... Args>
ComponentManager *scene::register_entity_component_manager(size_t componentCountHint, Args &&... args)
{
    // Duplicates are not allowed
    auto &&m = find_entity_component_manager<ComponentManager>();
    if(m)
    {
        return m;
    }

    // Create new manager
    auto &&result = std::make_unique<ComponentManager>(this, componentCountHint, std::forward(args)...);

    // Register
    m_componentManagerIDs.push_back(ComponentManager::component_class_id());
    m_componentManagers.push_back(std::move(result));

    return (ComponentManager *)m_componentManagers.back().get();
}


template<class ComponentManager>
ComponentManager *scene::find_entity_component_manager()
{
    auto cid = ComponentManager::component_class_id();
    size_t i = 0;
    for(auto cmid : m_componentManagerIDs)
    {
        if(cmid == cid)
        {
            return (ComponentManager *)m_componentManagers[i].get();
        }

        ++i;
    }

    return nullptr;
}
