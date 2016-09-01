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
    \file core/double_buffered.h
    \brief Contains a class for double-buffering data.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


//! A class, which represents a double-buffered element.
template<typename T>
class double_buffered
{
public:
    //! Constructor.
    double_buffered() : m_A(), m_B(), m_active(&m_A), m_swapCount(0) {}
public:
    /*!
        Returns the active element.

        \return The active element.
     */
    const T &active() const { return *m_active; }
    /*!
        Returns the active element.

        \return The active element.
     */
    T &active() { return *m_active; }
    /*!
        Returns the active element.

        \return The active element.
     */
    const T &inactive() const { return m_active == &m_A ? m_B : m_A; }
    /*!
        Returns the inactive element.

        \return The inactive element.
     */
    T &inactive() { return m_active == &m_A ? m_B : m_A; }
    /*!
        Swaps the active element.

        \return The old active element.
     */
    T &swap()
    {
        auto &&result = *m_active;
        m_active = m_active == &m_A ? &m_B : &m_A;
        ++m_swapCount;
        return result;
    }
    /*!
        Swaps the active element.

        \return The old active element.
     */
    const T &swap() const
    {
        auto &&result = *m_active;
        m_active = m_active == &m_A ? &m_B : &m_A;
        ++m_swapCount;
        return result;
    }
    /*!
        Returns the number of times that the buffer has been swapped.

        \return The number of times that the buffer has been swapped.
     */
    uint64_t swap_count() const { return m_swapCount; }
private:
    //! The first timer set.
    T m_A;
    //! The second timer set.
    T m_B;
    //! The active element.
    T *m_active;
    //! The number of times that the buffer has been swapped.
    uint64_t m_swapCount;
};