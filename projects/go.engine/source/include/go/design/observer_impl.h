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
// observer<class, class, class> member functions
// ================================================================================ //

template<class Derived, class Observable, class Event>
observer<Derived, Observable, Event>::observer(observer_type &&other) noexcept
{
    *this = std::move(other);
}


template<class Derived, class Observable, class Event>
typename observer<Derived, Observable, Event>::observer_type &
observer<Derived, Observable, Event>::operator=(observer_type &&other) noexcept
{
    m_callback = other.m_callback;
    m_derivedObserver = other.m_derivedObserver;
    m_isDestroying = other.m_isDestroying;
    m_knownObservables = std::move(other.m_knownObservables);
    return *this;
}


template<class Derived, class Observable, class Event>
observer<Derived, Observable, Event>::observer(Derived *derived,
    bool (Derived::*callback)(Observable *, Event)) noexcept
    : m_derivedObserver(derived), m_callback(callback), m_isDestroying(false)
{
}


template<class Derived, class Observable, class Event>
observer<Derived, Observable, Event>::~observer()
{
    m_isDestroying = true;
    for(auto &o : m_knownObservables)
    {
        o->remove_observer(static_cast<Derived *>(this));
    }
}


template<class Derived, class Observable, class Event>
bool observer<Derived, Observable, Event>::invoke_callback(Observable *observable, Event event)
{
    return (m_derivedObserver->*m_callback)(observable, event);
}


template<class Derived, class Observable, class Event>
void observer<Derived, Observable, Event>::add_observable(Observable *observable)
{
    if(observable)
    {
        m_knownObservables.insert(observable);
    }
}


template<class Derived, class Observable, class Event>
void observer<Derived, Observable, Event>::remove_observable(Observable *observable)
{
    if(!m_isDestroying)
    {
        m_knownObservables.erase(observable);
    }
}


// ================================================================================ //
// observer<class, class, void> member functions
// ================================================================================ //

template<class Derived, class Observable>
observer<Derived, Observable, void>::observer(Derived *derived,
    bool (Derived::*callback)(Observable *)) noexcept
    : m_derivedObserver(derived), m_callback(callback), m_isDestroying(false)
{
}


template<class Derived, class Observable>
observer<Derived, Observable, void>::observer(observer_type &&other) noexcept
{
    *this = std::move(other);
}


template<class Derived, class Observable>
typename observer<Derived, Observable, void>::observer_type &
observer<Derived, Observable, void>::operator=(observer_type &&other) noexcept
{
    m_callback = other.m_callback;
    m_derivedObserver = other.m_derivedObserver;
    m_isDestroying = other.m_isDestroying;
    m_knownObservables = std::move(other.m_knownObservables);
    return *this;
}


template<class Derived, class Observable>
observer<Derived, Observable, void>::~observer()
{
    m_isDestroying = true;
    for(auto &o : m_knownObservables)
    {
        o->remove_observer(static_cast<Derived *>(this));
    }
}


template<class Derived, class Observable>
bool observer<Derived, Observable, void>::invoke_callback(Observable *observable)
{
    return (m_derivedObserver->*m_callback)(observable);
}


template<class Derived, class Observable>
void observer<Derived, Observable, void>::add_observable(Observable *observable)
{
    if(observable)
    {
        m_knownObservables.insert(observable);
    }
}


template<class Derived, class Observable>
void observer<Derived, Observable, void>::remove_observable(Observable *observable)
{
    if(!m_isDestroying)
    {
        m_knownObservables.erase(observable);
    }
}


// ================================================================================ //
// observable<class, class> member functions
// ================================================================================ //

template<class Observable, class Event>
observable<Observable, Event>::observable(observable_type &&other) noexcept
{
    *this = std::move(other);
}


template<class Observable, class Event>
typename observable<Observable, Event>::observable_type &
observable<Observable, Event>::operator=(observable_type &&other) noexcept
{
    m_observers = std::move(other.m_observers);
    return *this;
}


template<class Observable, class Event>
observable<Observable, Event>::~observable()
{
    for(auto &o : m_observers)
    {
        o->remove_observable(static_cast<Observable *>(this));
    }
}


template<class Observable, class Event>
void observable<Observable, Event>::add_observer(observer_type *observer)
{
    if(observer)
    {
        m_observers.push_back(observer);
        observer->add_observable(static_cast<Observable *>(this));
    }
}


template<class Observable, class Event>
void observable<Observable, Event>::remove_observer(observer_type *observer)
{
    const auto p = std::find(m_observers.begin(), m_observers.end(), observer);
    if(p != m_observers.end())
    {
        observer->remove_observable(static_cast<Observable *>(this));
        m_observers.erase(p);
    }
}


template<class Observable, class Event>
bool observable<Observable, Event>::has_observers() const noexcept
{
    return !m_observers.empty();
}


template<class Observable, class Event>
void observable<Observable, Event>::post_event(Event e)
{
    for(auto &o : m_observers)
    {
        if(!o->invoke_callback(static_cast<Observable *>(this), e))
        {
            break;
        }
    }
}


template<class Observable, class Event>
auto observable<Observable, Event>::observers() const -> const observer_list &
{
    return m_observers;
}


// ================================================================================ //
// observable<class, void> member functions
// ================================================================================ //

template<class Observable>
observable<Observable, void>::observable(observable_type &&other) noexcept
{
    *this = std::move(other);
}


template<class Observable>
typename observable<Observable, void>::observable_type &
observable<Observable, void>::operator=(observable_type &&other) noexcept
{
    m_observers = std::move(other.m_observers);
    return *this;
}


template<class Observable>
observable<Observable, void>::~observable()
{
    for(auto &o : m_observers)
    {
        o->remove_observable(static_cast<Observable *>(this));
    }
}


template<class Observable>
void observable<Observable, void>::add_observer(observer_type *observer)
{
    if(observer)
    {
        m_observers.push_back(observer);
        observer->add_observable(static_cast<Observable *>(this));
    }
}


template<class Observable>
void observable<Observable, void>::remove_observer(observer_type *observer)
{
    auto p = std::find(m_observers.begin(), m_observers.end(), observer);
    if(p != m_observers.end())
    {
        observer->remove_observable(static_cast<Observable *>(this));
        m_observers.erase(p);
    }
}


template<class Observable>
bool observable<Observable, void>::has_observers() const noexcept
{
    return !m_observers.empty();
}


template<class Observable>
void observable<Observable, void>::post_event()
{
    for(auto &o : m_observers)
    {
        if(!o->invoke_callback(static_cast<Observable *>(this)))
        {
            break;
        }
    }
}


template<class Observable>
auto observable<Observable, void>::observers() const -> const observer_list &
{
    return m_observers;
}
