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
    \file design/observer.h
    \brief Contains classes for an implementation of the observer pattern.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

/*!
    An abstract class, which statically implements the observer in the observer pattern.

    \tparam Observable The type of the class for which a notification function is called when an
    event occurs.
    \tparam Event The type of an event object that is sent to observers when an event occurs.
    If there is no event object, then this parameter should be omitted.
 */
template<class Observable, class Event = void>
class observer_base
{
public:
    //! The type of the observable class.
    typedef Observable observable_type;
    //! The type of the event class.
    typedef Event event_type;
public:
    /*!
        Invokes the callback function.

        \param[in] observable A pointer to the observable instance which invoked the function.
        \param event The event parameter passed to #Observable::PostEvent.

        \return If the return value is true, the event is passed on to the next observer.
        Otherwise, the event will be consumed by the observer.
     */
    virtual bool invoke_callback(Observable *observable, Event event) = 0;
    /*!
        Adds a new observable to the list of observable objects.

        \param[in] observable The observable instance.
     */
    virtual void add_observable(Observable *observable) = 0;
    /*!
        Removes an observable from the list of observable objects.

        \param[in] observable The observable instance.
     */
    virtual void remove_observable(Observable *observable) = 0;
};


/*!
    An abstract class, which statically implements the observer in the observer pattern.

    \tparam Observable The type of the class for which a notification function is called when an
    event occurs.
    \tparam Event The type of an event object that is sent to observers when an event occurs.
    If there is no event object, then this parameter should be omitted.
 */
template<class Observable>
class observer_base<Observable, void>
{
public:
    //! The type of the observable class.
    typedef Observable observable_type;
    //! The type of the event class.
    typedef void event_type;
public:
    /*!
        Invokes the callback function.

        \param[in] observable A pointer to the observable instance which invoked the function.

        \return If the return value is true, the event is passed on to the next observer.
        Otherwise, the event will be consumed by the observer.
     */
    virtual bool invoke_callback(Observable *observable) = 0;
    /*!
        Adds a new observable to the list of observable objects.

        \param[in] observable The observable instance.
     */
    virtual void add_observable(Observable *observable) = 0;
    /*!
        Removes an observable from the list of observable objects.
        
        \param[in] observable The observable instance.
     */
    virtual void remove_observable(Observable *observable) = 0;
};


/*!
    An abstract class, which statically implements the observer in the observer pattern.

    \tparam Derived The type of the class for which a notification function is called when an event
    occurs.
    \tparam Observable The type of the class for which a notification function is called when an
    event occurs.
    \tparam Event The type of an event object that is sent to observers when an event occurs.
    If there is no event object, then this parameter should be omitted.
 */
template<class Derived, class Observable, class Event = typename Observable::event_type>
class observer : public observer_base<Observable, Event>
{
public:
    //! The type of the class.
    typedef observer<Derived, Observable, Event> observer_type;
    //! The type of the derived class.
    typedef Derived derived_type;
    //! The type of the observable being observed.
    typedef Observable observable_type;
    //! The type of the event class.
    typedef Event event_type;
public:
    bool invoke_callback(Observable *observable, Event event) override;
    void add_observable(Observable *observable) override;
    void remove_observable(Observable *observable) override;
public:
    /*!
        Constructs the object and initializes it.

        \param[in] derived A pointer to the object for which a notification function is called when
        an event occurs.
        \param[in] callback A pointer to a member function (the notification function) of the
        object specified by the callback parameter that is called when an event occurs.
     */
    observer(Derived *derived, bool (Derived::*callback)(Observable *, Event)) noexcept;
    //! \overload
    observer(observer_type &&other) noexcept;
    //! Destructor.
    virtual ~observer();
public:
    /*!
        Move-assignment operator.

        \param other The object to move.

        \return The object being moved to.
     */
    observer_type &operator=(observer_type &&other) noexcept;
private:
    //! A set of observables.
    typedef std::set<Observable *> observable_set;
private:
    //! The derived object instance to notify.
    Derived *m_derivedObserver;
    //! The callback function of the derived object instance to invoke.
    bool (Derived::*m_callback)(Observable *, typename Observable::event_type);
    //! The objects being observed.
    observable_set m_knownObservables;
    //! Dirty-flag, which is set when the object is being destroyed.
    bool m_isDestroying;
};


/*!
    An abstract class, which statically implements the observer in the observer pattern.

    \tparam Derived The type of the class for which a notification function is called when an event
    occurs.
    \tparam Observable The type of the class for which a notification function is called when an
    event occurs.
    \tparam Event The type of an event object that is sent to observers when an event occurs.
    If there is no event object, then this parameter should be omitted.
 */
template<class Derived, class Observable>
class observer<Derived, Observable, void> : public observer_base<Observable, void>
{
public:
    //! The type of the class.
    typedef observer<Derived, Observable, void> observer_type;
    //! The type of the derived class.
    typedef Derived derived_type;
    //! The type of the observable being observed.
    typedef Observable observable_type;
    //! The type of the event class.
    typedef void event_type;
public:
    bool invoke_callback(Observable *observable) override;
    void add_observable(Observable *observable) override;
    void remove_observable(Observable *observable) override;
public:
    /*!
        Constructs the object and initializes it.
        \param[in] derived A pointer to the object for which a notification function is called when
        an event occurs.
        \param[in] callback A pointer to a member function (the notification function) of the
        object specified by the m_callback parameter that is called when an event occurs.
     */
    observer(Derived *derived, bool (Derived::*callback)(Observable *)) noexcept;
    //! \overload
    observer(observer_type &&other) noexcept;
    //! Destructor.
    virtual ~observer();
public:
    /*!
        Move-assignment operator.

        \param other The object to move.

        \return The object being moved to.
     */
    observer_type &operator=(observer_type &&other) noexcept;
private:
    //! A set of observables.
    typedef std::set<Observable *> observable_set;
private:
    //! The derived object instance to notify.
    Derived *m_derivedObserver;
    //! The callback function of the derived object instance to invoke.
    bool (Derived::*m_callback)(Observable *);
    //! The objects being observed.
    observable_set m_knownObservables;
    //! Dirty-flag, which is set when the object is being destroyed.
    bool m_isDestroying;
};


/*!
    An abstract class, which statically implements the Observable in the Observer Pattern.
    
    \tparam Observable The type of the class for which a notification function is called when an
    event occurs.
    \tparam Event The type of an event object that is sent to observers when an event occurs.
    If there is no event object, then this parameter should be omitted. 
 */
template<class Observable, class Event = void>
class observable
{
public:
    //! The type of the class.
    typedef observable<Observable, Event> observable_type;
    //! Type of the observers, which can be registered with the observable.
    typedef observer_base<Observable, Event> observer_type;
    //! Type of the event, which is dispatched by the observable.
    typedef Event event_type;
    //! A list of observers.
    typedef std::vector<observer_type *> observer_list;
public:
    /*!
        Move constructor.

        \param other The object to move.
     */
    observable(observable_type &&other) noexcept;
    //! Destructor.
    virtual ~observable();
public:
    /*!
        Move-assignment operator.

        \param other The object to move.

        \return The object being moved to.
     */
    observable_type &operator=(observable_type &&other) noexcept;
public:
    /*!
        Installs an observer on an observable object.

        \param[in] observer The observer to install.
     */
    void add_observer(observer_type *observer);
    /*!
        Removes an observer from an observable object.

        \param[in] observer The observer to install.
     */
    void remove_observer(observer_type *observer);
    /*!
        Returns a boolean value indicating whether an observable object has any observers.

        \return If there is at least one observer installed, the return value is true.
        Otherwise, the return value is false.
     */
    bool has_observers() const noexcept;
    /*!
        Posts an event to an observable object.

        \param event The event to post.
     */
    void post_event(Event e);
    /*!
        Returns the observer list.

        \return The observer list.
     */
    const observer_list &observers() const;
protected:
    //! Abstract class.
    observable() {}
private:
    //! The objects observing this instance.
    observer_list m_observers;
};


/*!
    An abstract class, which statically implements the observable in the observer pattern.

    \tparam Observable The type of the class for which a notification function is called when an
    event occurs.
    \tparam Event The type of an event object that is sent to observers when an event occurs.
    If there is no event object, then this parameter should be omitted. 
 */
template<class Observable>
class observable<Observable, void>
{
public:
    //! The type of the class.
    typedef observable<Observable, void> observable_type;
    //! Type of the observers, which can be registered with the observable.
    typedef observer_base<Observable, void> observer_type;
    //! Type of the event, which is dispatched by the observable.
    typedef void event_type;
    //! A list of observers.
    typedef std::vector<observer_type *> observer_list;
public:
    /*!
        Move constructor.

        \param other The object to move.
     */
    observable(observable_type &&other) noexcept;
    //! Destructor.
    virtual ~observable();
public:
    /*!
        Move-assignment operator.

        \param other The object to move.

        \return The object being moved to.
     */
    observable_type &operator=(observable_type &&other) noexcept;
public:
    /*!
        Installs an observer on an observable object.

        \param[in] observer The observer to install.
     */
    void add_observer(observer_type *observer);
    /*!
        Removes an observer from an observable object.

        \param[in] observer The observer to install.
     */
    void remove_observer(observer_type *observer);
    /*!
        Returns a boolean value indicating whether an observable object has any observers.
        
        \return If there is at least one observer installed, the return value is true.
        Otherwise, the return value is false.
     */
    bool has_observers() const noexcept;
    //! Posts an event to an observable object.
    void post_event();
    /*!
        Returns the observer list.

        \return The observer list.
     */
    const observer_list &observers() const;
protected:
    //! Abstract class.
    observable() {}
private:
    //! The objects observing this instance.
    observer_list m_observers;
};


#include "observer_impl.h"

GO_END_NAMESPACE
