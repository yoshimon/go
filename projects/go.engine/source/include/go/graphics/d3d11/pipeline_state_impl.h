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
// gfx_pipeline_state member functions
// ================================================================================ //

template<size_t SlotCount, typename... SlotArgs>
inline gfx_pipeline_state<SlotCount, SlotArgs...>::gfx_pipeline_state()
{
    reset();
}


template<size_t SlotCount, typename... SlotArgs>
inline void gfx_pipeline_state<SlotCount, SlotArgs...>::reset()
{
    m_modifiedEntries.clear_all();
    m_slotValues.fill(data_tuple_t());
}


template<size_t SlotCount, typename... SlotArgs>
inline auto gfx_pipeline_state<SlotCount, SlotArgs...>::bind_slot(uint32_t slot, SlotArgs... args) -> first_data_t
{
    auto &&result = std::get<0>(m_slotValues[slot]);
    m_slotValues[slot] = std::make_tuple(std::move(args)...);
    m_modifiedEntries.set(slot);
    return result;
}


template<size_t SlotCount, typename... SlotArgs>
void gfx_pipeline_state<SlotCount, SlotArgs...>::bind_changes(
    gfx_pipeline_state<SlotCount, SlotArgs...> &other, void *userData, bind_function_t bindFunc, equality_cmp_function_t equalityCmpFunc)
{
    data_t slotValues[] = { m_slotValues.data(), other.m_slotValues.data() };

    m_modifiedEntries.for_each_groupset([](size_t groupStart, size_t groupSize, void *userData, bind_function_t bindFunc,
        equality_cmp_function_t equalityCmpFunc, data_t slotValues[2])
    {
        size_t curGroupSize = 0; // Current group size
        for(size_t i = groupStart, end = groupStart + groupSize; i < end; ++i)
        {
            // Group end?
            if(equalityCmpFunc(slotValues[0][i], slotValues[1][i]))
            {
                // Valid group?
                if(curGroupSize > 0)
                {
                    // Bind to device
                    bindFunc(groupStart, curGroupSize, slotValues, userData);

                    // Copy values
                    for(size_t i = groupStart, end = groupStart + curGroupSize; i < end; ++i)
                    {
                        slotValues[1][i] = slotValues[0][i];
                    }

                    // Reset group
                    curGroupSize = 0;
                }
            }
            else
            {
                if(curGroupSize > 0)
                {
                    // Existing group
                    ++curGroupSize;
                }
                else
                {
                    // New group
                    groupStart = i;
                    curGroupSize = 1;
                }
            }
        }

        // Bind open group
        if(curGroupSize > 0)
        {
            bindFunc(groupStart, curGroupSize, slotValues, userData);

            // Copy values
            for(size_t i = groupStart, end = groupStart + curGroupSize; i < end; ++i)
            {
                slotValues[1][i] = slotValues[0][i];
            }
        }
    }, userData, bindFunc, equalityCmpFunc, slotValues);

    // Reset markers
    m_modifiedEntries.clear_all();
    other.m_modifiedEntries.clear_all();
}


template<size_t SlotCount, typename... SlotArgs>
inline auto gfx_pipeline_state<SlotCount, SlotArgs...>::slots() noexcept -> data_tuple_t *
{
    return m_slotValues;
}


template<size_t SlotCount, typename... SlotArgs>
inline auto gfx_pipeline_state<SlotCount, SlotArgs...>::operator[](size_t index) -> data_tuple_t &
{
    return m_slotValues[index];
}


template<size_t SlotCount, typename... SlotArgs>
inline auto gfx_pipeline_state<SlotCount, SlotArgs...>::modified_mask() noexcept -> bitmask<SlotCount> &
{
    return m_modifiedEntries;
}


template<size_t SlotCount, typename... SlotArgs>
void gfx_pipeline_state<SlotCount, SlotArgs...>::unbind_by_mask(const first_data_t resource, const bitmask<SlotCount> &unbindMask)
{
    unbindMask.for_each_set([](size_t slot, data_tuple_t *slotValues, first_data_t r)
    {
        if(r == nullptr || std::get<0>(slotValues[slot]) == r)
        {
            slotValues[slot] = data_tuple_t();
        }
    }, m_slotValues.data(), resource);
}
