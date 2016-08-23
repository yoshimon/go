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
    \file core/semaphore.h
    \brief Contains a semaphore class.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

//! A class, which implements a semaphore.
class GO_API semaphore
{
    GO_DECLARE_NON_COPYABLE(semaphore)
public:
    /*!
        Constructs the object and initializes it.

        \param value The initial semaphore value.
     */
    semaphore(int32_t value);
public:
    //! Waits for the semaphore to be signaled.
    void wait();
    //! Signals the semaphore to wakeup.
    void signal();
private:
    //! The current semaphore value.
    int32_t m_value;
    //! Helper variable to block the condition variable.
    int32_t m_numWakeups;
    //! The internal lock.
    std::mutex m_lock;
    //! The internal semaphore helper.
    std::condition_variable m_condVar;
};

GO_END_NAMESPACE
