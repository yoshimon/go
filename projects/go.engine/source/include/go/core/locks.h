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
    \file core/locks.h
    \brief Contains a class which represents a spin-based lock.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include <atomic>


GO_BEGIN_NAMESPACE

//! A class, which represents a spin-based lock.
class GO_API spin_lock
{
public:
    //! Constructor.
    spin_lock() { m_lock.clear(); }
    //! Locks the spin-lock.
    void lock() { while(m_lock.test_and_set(std::memory_order_acquire)) {} }
    //! Unlocks the spin-lock.
    void unlock() { m_lock.clear(std::memory_order_release); }
private:
    //! Internal lock.
    std::atomic_flag m_lock;
};


//! A class, which implements RAII for a lock.
template<typename Lock>
class lock_guard
{
public:
    typedef Lock lock_t;
public:
    lock_guard(const lock_guard &) = delete;
    /*!
        Locks the lock.

        \param lock The lock to lock.
     */
    explicit lock_guard(lock_t &lock) : m_lock(lock) { lock.lock(); }
    /*!
        Destructor.

        Unlocks the lock.
     */
    ~lock_guard() { m_lock.unlock(); }
public:
    lock_guard &operator=(lock_guard &) = delete;
private:
    //! The internal lock reference.
    lock_t &m_lock;
};

GO_END_NAMESPACE
