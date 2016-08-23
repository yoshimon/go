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
	\file graphics/texture2d_atlas.h
	\brief Contains classes that can .

	\author Gokhan Ozdogan
	\version 1.0
 */


#pragma once


#include "go/core/common.h"
#include "go/graphics/api/texture.h"


GO_BEGIN_NAMESPACE

//! A structure, that describes the properties of a texture 2D atlas.
struct gfx_texture2d_regular_atlas_info
{
	//! The width of a single element.
	int32_t elementWidth;
	//! The height of a single element.
	int32_t elementHeight;
	//! The number of rows.
	int32_t numRows;
	//! The number of columns.
	int32_t numColumns;
	//! The color format for all elements.
	gfx_color_format colorFormat;
};

//! A class, which represents a 2D texture atlas.
template<class Texture>
class gfx_texture2d_regular_atlas
{
	GO_DECLARE_NON_COPYABLE(gfx_texture2d_regular_atlas)
public:
	typedef Texture texture_t;
public:
	/*!
		Constructor.

		\param options The cache parameters.
	 */
	gfx_texture2d_regular_atlas(const gfx_texture2d_regular_atlas_info &options);
	//! \overload
	gfx_texture2d_regular_atlas() = default;
public:
	/*!
		Re-initializes the object.

		\param options The cache parameters.
	 */
	void reset(const gfx_texture2d_regular_atlas_info &options);
	/*!
		Returns the cache descriptor.

		\return The cache descriptor.
	 */
	const gfx_texture2d_regular_atlas_info &info() const noexcept;
	/*!
		Requests a new texture.

		\param[out] elementIndex The element index.
		\param id An optional identifier that can be used to identify the allocation.
		Subsequent allocations with the same ID will return the same index.

		\return True, if an element was allocated. Otherwise false.
	 */
    bool allocate(int32_t *elementIndex, int32_t id = -1);
    /*!
        This will try to allocate a shadow map by ID.

        \param[out] elementIndex The element index.
        \param id An optional identifier that can be used to identify the allocation.
        Subsequent allocations with the same ID will return the same index.

        \return If no matching ID is found and there is no slot without an invalid ID (-1) the
        function will return false. Otherwise, the return value is true.
     */
    bool try_allocate_by_id(int32_t *elementIndex, int32_t id = -1);
	/*!
		Calculates the pixel bounds of a given element.

		\param elementIndex The element index.

		\return The bounds of the element.
	 */
	rect<float> px_bounds_of(int32_t elementIndex);
	/*!
		Calculates the UV bounds of a given element.

		\param elementIndex The element index.

		\return The bounds of the element.
	 */
	rect<float> uv_bounds_of(int32_t elementIndex);
	/*!
		Releases all elements.

		\param preserveIDs If true, IDs are preserved so that subsequent allocations will return the same element index.
	 */
	void free_all(bool preserveIDs);
	/*!
		Returns whether the atlas space is exhausted.

		\return True, if all the space is exhausted. Otherwise false.
	 */
	bool is_full() const noexcept;
	/*!
		Returns the underlying texture.

		\return The texture.
	 */
	texture_t *texture() noexcept;
private:
	//! An allocation entry.
	struct tile_t
	{
		//! The allocation index.
		int32_t elementIndex;
		//! The tile ID.
		int32_t id;
	};
private:
	//! Information about the cache.
	gfx_texture2d_regular_atlas_info m_info;
	//! The texture cache.
	texture_t m_texture;
	//! The flattened matrix of used entries.
	std::vector<tile_t> m_usedTiles;
	//! The flattened matrix of free entries.
	std::vector<tile_t> m_freeTiles;
};


typedef gfx_texture2d_regular_atlas<gfx_depth_stencil_buffer> gfx_depth_stencil_buffer_regular_atlas;


#include "texture2d_atlas_impl.h"

GO_END_NAMESPACE