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
    \file core/noise_filter.h
    \brief Contains classes for reducing noise in a sample set.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go\core\common.h"


GO_BEGIN_NAMESPACE

/*!
    A class, which averages a running window of samples.
    MovingAverage filter by Jason Gregory.

    \tparam T The type of the samples.
    \tparam NumElements The size of the sample window in elements.
 */
template<typename T, int32_t NumElements>
class windowed_average_filter
{
public:
    //! The element type.
    typedef T value_type;
    //! The pointer-to-element type.
    typedef T *pointer;
    //! The pointer-to-const-element type.
    typedef const T *const_pointer;
    //! The reference-to-element type.
    typedef T &reference;
    //! The const-reference-to-element type.
    typedef const T &const_reference;
public:
    //! Constructor.
    windowed_average_filter();
public:
    /*!
        Adds a new sample to the averaged sum.
        The new sample replaces the oldest sample in the window.

        \param newSample The sample to add.
     */
    void add_sample(const T &newSample);
    /*!
        Returns the current average.

        \return Current average.
     */
    float average() const noexcept;
    //! Resets the filter.
    void reset() noexcept;
private:
    //! The current sample index.
    int32_t m_currentSample;
    //! The number of samples.
    int32_t m_sampleCount;
    //! The sample buffer.
    T m_samples[NumElements];
    //! The current (running) average.
    T m_average;
};


#include "noise_filter_impl.h"

GO_END_NAMESPACE
