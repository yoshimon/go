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
// bitmask member functions
// ================================================================================ //

template<size_t BitCount>
inline bitmask<BitCount>::bitmask()
{
    clear_all();
}


template<size_t BitCount>
inline bitmask<BitCount>::bitmask(submask_t value)
{
    *this = value;
}


template<size_t BitCount>
inline bitmask<BitCount> &bitmask<BitCount>::operator=(submask_t value)
{
    set_all(value);
    return *this;
}


template<size_t BitCount>
inline void bitmask<BitCount>::clear_all()
{
    set_all(0);
}


template<size_t BitCount>
void bitmask<BitCount>::set_all(submask_t value)
{
    for(auto &m : m_masks)
    {
        m = value;
    }
}


template<size_t BitCount>
inline bool bitmask<BitCount>::is_zero() const
{
    return (*this) == 0;
}


template<size_t BitCount>
inline bool bitmask<BitCount>::operator==(submask_t value) const noexcept
{
    for(auto &m : m_masks)
    {
        if(m != value)
        {
            return false;
        }
    }

    return true;
}


template<size_t BitCount>
inline auto bitmask<BitCount>::operator|=(const bitmask<BitCount> &other) noexcept
    -> bitmask<BitCount> &
{
    for(size_t i = 0, end = m_masks.max_size(); i < end; ++i)
    {
        m_masks[i] |= other.m_masks[i];
    }

    return *this;
}


template<size_t BitCount>
inline auto bitmask<BitCount>::operator&=(const bitmask<BitCount> &other) noexcept
    -> bitmask<BitCount> &
{
    for(size_t i = 0, end = m_masks.max_size(); i < end; ++i)
    {
        m_masks[i] &= other.m_masks[i];
    }

    return *this;
}


template<size_t BitCount>
inline bool bitmask<BitCount>::operator!=(submask_t value) const noexcept
{
    return !((*this) == value);
}


template<size_t BitCount>
inline void bitmask<BitCount>::set(size_t index)
{
    GO_ASSERT(index < BitCount);
    auto bucketIndex = bucket_index_from_slot<submask_t>(index);
    m_masks[bucketIndex] |= bucket_bitmask_set<submask_t>(bucketIndex, index);
}


template<size_t BitCount>
inline void bitmask<BitCount>::clear(size_t index)
{
    GO_ASSERT(index < BitCount);
    auto bucketIndex = bucket_index_from_slot<submask_t>(index);
    m_masks[bucketIndex] &= ~bucket_bitmask_set<submask_t>(bucketIndex, index);
}


template<size_t BitCount>
inline bool bitmask<BitCount>::forward_null_scan(uint32_t *index)
{
    for(size_t i = 0, end = m_masks.max_size(); i < end; ++i)
    {
        auto j = i * 8 * sizeof(submask_t);
        auto m = ~m_masks[i]; // NOTE: invert the mask

        unsigned long s;
#ifdef _WIN64
        auto bitFound = BitScanForward64(&s, m);
#else
        auto bitFound = BitScanForward(&s, m);
#endif

        if(bitFound)
        {
            *index = j + s;
            return true;
        }
    }

    return false;
}


template<size_t BitCount>
template<class Callback, typename... Args>
void bitmask<BitCount>::for_each_set(const Callback &callback, Args&&... args) const
{
    for(size_t i = 0, end = m_masks.max_size(); i < end; ++i)
    {
        auto j = i * 8 * sizeof(submask_t);
        auto m = m_masks[i];

        while(m != 0)
        {
            unsigned long s;
#ifdef _WIN64
            BitScanForward64(&s, m);
#else
            BitScanForward(&s, m);
#endif

            callback(j + s, std::forward<Args>(args)...);
            j += s + 1;
            m >>= s + 1;
        }
    }
}


template<size_t BitCount>
template<class Callback, typename... Args>
void bitmask<BitCount>::for_each_groupset(const Callback &callback, Args&&... args) const
{
    size_t groupStart = 0;
    auto startFound = false;
    for(size_t i = 0, end = m_masks.max_size(); i < end; ++i)
    {
        auto m = m_masks[i];

        if(m == 0)
        {
            continue;
        }
        
        auto j = i * 8 * sizeof(submask_t);
        auto mi = ~m;

        // Find start
        do
        {
            if(!startFound)
            {
                unsigned long start = 0;
#ifdef _WIN64
                BitScanForward64(&start, m);
#else
                BitScanForward(&start, m);
#endif

                startFound = true;
                groupStart = j + start;

                // Zero out LSBs
                mi >>= start;
                mi <<= start;
            }

            unsigned long end;
#ifdef _WIN64
            auto endFound = BitScanForward64(&end, mi);
#else
            auto endFound = BitScanForward(&end, mi);
#endif
            if(endFound)
            {
                auto groupSize = j + end - groupStart;
                callback(groupStart, groupSize, std::forward<Args>(args)...);
                startFound = false;

                m >>= end;
                m <<= end;
            }
            else
            {
                // Go to next mask
                break;
            }
        } while(m != 0);
    }

    if(startFound)
    {
        auto groupSize = m_masks.max_size() * 8 * sizeof(submask_t) - groupStart;
        callback(groupStart, groupSize, std::forward<Args>(args)...);
    }
}
