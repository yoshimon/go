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
// windowed_average_filter member functions
// ================================================================================ //

template<typename T, int32_t NumElements>
inline windowed_average_filter<T, NumElements>::windowed_average_filter()
    : m_currentSample(0), m_sampleCount(0), m_average(static_cast<T>(0))
{
}


template<typename T, int32_t NumElements>
inline void windowed_average_filter<T, NumElements>::add_sample(const T &newSample)
{
    if(m_sampleCount == NumElements)
    {
        m_average -= m_samples[m_currentSample];
    }
    else
    {
        ++m_sampleCount;
    }

    m_samples[m_currentSample] = newSample;
    m_average += newSample;
    ++m_currentSample;
    if(m_currentSample >= NumElements)
    {
        m_currentSample = 0;
    }
}


template<typename T, int32_t NumElements>
inline float windowed_average_filter<T, NumElements>::average() const noexcept
{
    if(m_sampleCount == 0)
    {
        return 0.0f; // div 0
    }

    return m_average / static_cast<float>(m_sampleCount);
}


template<typename T, int32_t NumElements>
inline void windowed_average_filter<T, NumElements>::reset() noexcept
{
    m_average = 0;
    m_sampleCount = 0;
}
