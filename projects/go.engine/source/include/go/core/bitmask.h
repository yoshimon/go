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
    \file core/bitmask.h
    \brief Contains a helper class for stepping through byte vectors.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"


GO_BEGIN_NAMESPACE

/*!
    A class, which allows byte-vectors to be traversed with user-defined types.

    \tparam BitCount The number of bits in the mask.
 */
template<size_t BitCount>
class bitmask
{
public:
    typedef size_t submask_t;
	//! The number of bits in the mask.
	static const size_t kBitCount = BitCount;
	//! The number of submasks in the bitmask.
	static const size_t kSubmaskCount = 1 + BitCount / (8 * sizeof(submask_t) + 1);
public:
    //! Constructor.
    bitmask();
    //! \overload
    bitmask(submask_t value);
public:
    /*!
        Value-assignment operator.

        \param value The value to assign to each submask.

        \return The object being assigned to.
     */
    bitmask<BitCount> &operator=(submask_t value);
    /*!
        Submask comparison operator.

        \param value The value to compare each submask with.

        \return If all submasks match the specified value, the return value is true.
        Otherwise, the return value is false.
     */
    bool operator==(submask_t value) const noexcept;
    /*!
        Submask comparison operator.

        \param value The value to compare each submask with.

        \return If any submask does not match the specified value, the return value is true.
        Otherwise, the return value is false.
     */
    bool operator!=(submask_t value) const noexcept;
    /*!
        Performs a bitwise OR-operation.

        \param other The other bitmask.

        \return The object being assigned to.
     */
    bitmask<BitCount> &operator|=(const bitmask<BitCount> &other) noexcept;
    /*!
        Performs a bitwise AND-operation.

        \param other The other bitmask.

        \return The object being assigned to.
     */
    bitmask<BitCount> &operator&=(const bitmask<BitCount> &other) noexcept;
public:
    /*!
        Returns whether the bitmask is zero.
     
        \return Returns true, if the bitmask is zero.
     */
    bool is_zero() const;
    //! Clears all bits in the mask.
    void clear_all();
    /*!
        Sets all submasks to a specified value.

        \param value The value to set.
     */
    void set_all(submask_t value);
    /*!
        Sets a bit.

        \param index The index of the bit to set.
     */
    void set(size_t index);
    /*!
        Unsets a bit.

        \param index The index of the bit to unset.
     */
    void clear(size_t index);
	/*!
		Scans for the first set LSB.

		\param[out] index The first bit set.

		\return True, if a set bit was found. Otherwise false.
	 */
	bool forward_null_scan(uint32_t *index);
    /*!
        Invokes a function for each bit that was set.

        \param callback The callback function.
        \param args Optional user arguments to forward to the callback function.
     */
    template<class Callback, typename... Args>
    void for_each_set(const Callback &callback, Args&&... args) const;
    /*!
        Invokes a function for each bit group that was set.

        \param callback The callback function.
        \param args Optional user arguments to forward to the callback function.
     */
    template<class Callback, typename... Args>
    void for_each_groupset(const Callback &callback, Args&&... args) const;
private:
    //! The internal sub-bitmasks.
    std::array<submask_t, kSubmaskCount> m_masks;
};


#include "bitmask_impl.h"

GO_END_NAMESPACE
