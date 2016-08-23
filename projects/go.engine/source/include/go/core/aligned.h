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
	\file core/aligned.h
	\brief Contains a base class for aligned memory allocations.

	\author Gokhan Ozdogan
	\version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

/*!
	 A class, that can be used as a base class for aligned memory allocation.

	 \tparam T The type of the parent class.
 */
template<typename T>
class aligned
{
public:
	/*!
		Operator override.

		\param size The number of bytes to allocate.
	 */
	static void *operator new(size_t size)
	{
		// Deduce the alignment of type T
		constexpr auto alignment = alignof(T);

		// T must be at least 64-bits
		static_assert(alignment > 8, "Aligned allocation is only useful for types with > 8 byte alignment. Did you forget a alignas(align) on T?");

		// Allocate the memory
		auto &&p = _aligned_malloc(size, alignment);

		if(!p)
		{
			throw std::bad_alloc();
		}

		return p;
	}
	/*!
		Operator override.

		\param size The number of bytes to allocate.
	 */
	static void *operator new[](size_t size)
	{
		return operator new(size);
	}
	/*!
		Operator override.

		\param p The pointer to free.
	 */
	static void operator delete(void *p)
	{
		_aligned_free(p);
	}
	/*!
		Operator override.

		\param p The pointer to free.
	 */
	static void operator delete[](void *p)
	{
		operator delete(p);
	}
};

GO_END_NAMESPACE
