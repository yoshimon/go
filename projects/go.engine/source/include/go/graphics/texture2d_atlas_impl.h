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
// gfx_texture2d_regular_atlas member functions
// ================================================================================ //

template<class Texture>
gfx_texture2d_regular_atlas<Texture>::gfx_texture2d_regular_atlas(const gfx_texture2d_regular_atlas_info &options)
{
	reset(options);
}


template<class Texture>
void gfx_texture2d_regular_atlas<Texture>::reset(const gfx_texture2d_regular_atlas_info &options)
{
	GO_ASSERT(options.numRows > 0);
	GO_ASSERT(options.numColumns > 0);

	// Copy the descriptor
	m_info = options;

	// Pre-allocate the textures
	auto xSize = options.numColumns * options.elementWidth;
	auto ySize = options.numRows * options.elementHeight;

	// Initialize the default texture descriptor
	auto fmt = gfx_texture_format::as_2d(xSize, ySize, m_info.colorFormat);
	m_texture.reset(fmt);

	free_all(false);
}


template<class Texture>
auto gfx_texture2d_regular_atlas<Texture>::info() const noexcept -> const gfx_texture2d_regular_atlas_info &
{
	return m_info;
}


template<class Texture>
bool gfx_texture2d_regular_atlas<Texture>::allocate(int32_t *elementIndex, int32_t id)
{
	if(m_freeTiles.empty())
	{
		return false;
	}

    // Try allocating by ID first
    if(try_allocate_by_id(elementIndex, id))
    {
        return true;
    }

    // Just grab whatever free tile there is
	auto tile = m_freeTiles.back();
	m_freeTiles.pop_back();

    // And overwrite the previous ID
	tile.id = id;

	// Mark it as used
	m_usedTiles.push_back(tile);

	// Write the result back
	*elementIndex = tile.elementIndex;

	return true;
}


template<class Texture>
bool gfx_texture2d_regular_atlas<Texture>::try_allocate_by_id(int32_t *elementIndex, int32_t id)
{
    if(m_freeTiles.empty())
    {
        return false;
    }

    // Try to find a tile with the same ID
    if(id >= 0)
    {
        for(auto &&t : m_usedTiles)
        {
            if(t.id == id)
            {
                // Nothing to change
                *elementIndex = t.elementIndex;
                return true;
            }
        }

        // Find a free tile where the ID matches
        for(auto &&it = m_freeTiles.rbegin(), end = m_freeTiles.rend(); it != end; ++it)
        {
            auto &&t = *it;
            if(t.id == id)
            {
                // Mark it as used
                m_usedTiles.push_back(t);

                // Write the result back
                *elementIndex = t.elementIndex;

                // Remove the tile
                m_freeTiles.erase(--(it.base()));

                return true;
            }
        }

        // Otherwise grab one that has not been previously allocated
        for(auto &&it = m_freeTiles.rbegin(), end = m_freeTiles.rend(); it != end; ++it)
        {
            auto &&t = *it;
            if(t.id == -1)
            {
                // Update the tile ID
                t.id = id;

                // Mark it as used
                m_usedTiles.push_back(t);

                // Write the result back
                *elementIndex = t.elementIndex;

                // Remove the tile
                m_freeTiles.erase(--(it.base()));

                return true;
            }
        }
    }

    return false;
}


template<class Texture>
go::rect<float> gfx_texture2d_regular_atlas<Texture>::px_bounds_of(int32_t elementIndex)
{
	auto col = (float)(elementIndex % m_info.numColumns);
	auto row = (float)(elementIndex / m_info.numColumns);

	auto xMin = col * m_info.elementWidth;
	auto yMin = row * m_info.elementHeight;

	return go::rect<float>{ xMin, yMin, xMin + (float)m_info.elementWidth, yMin + (float)m_info.elementHeight };
}


template<class Texture>
go::rect<float> gfx_texture2d_regular_atlas<Texture>::uv_bounds_of(int32_t elementIndex)
{
	// Figure out the tiles AABB
	auto &&fmt = m_texture.format();

	auto texelSizeX = 1.0f / fmt.width;
	auto texelSizeY = 1.0f / fmt.height;

	auto &&bounds = px_bounds_of(elementIndex);
	bounds.left *= texelSizeX;
	bounds.top *= texelSizeY;
	bounds.right *= texelSizeX;
	bounds.bottom *= texelSizeY;
	return bounds;
}


template<class Texture>
void gfx_texture2d_regular_atlas<Texture>::free_all(bool preserveIDs)
{
	auto &&numElements = m_info.numColumns * m_info.numRows;
	m_freeTiles.reserve(numElements);
	m_usedTiles.reserve(numElements);

	if(!preserveIDs)
	{
		m_freeTiles.clear();

		// Fill free tiles
		for(int32_t y = m_info.numRows - 1; y >= 0; --y)
		{
			for(int32_t x = m_info.numColumns - 1; x >= 0; --x)
			{
				tile_t tile;
				tile.elementIndex = y * m_info.numColumns + x;
				tile.id = -1;
				m_freeTiles.push_back(std::move(tile));
			}
		}
	}
	else
	{
		m_freeTiles.insert(std::end(m_freeTiles), std::begin(m_usedTiles), std::end(m_usedTiles));
		std::sort(std::begin(m_freeTiles), std::end(m_freeTiles), [](const tile_t &a, const tile_t &b)
		{
			return a.elementIndex > b.elementIndex;
		});
	}

	m_usedTiles.clear();
}


template<class Texture>
auto gfx_texture2d_regular_atlas<Texture>::texture() noexcept -> texture_t *
{
	return &m_texture;
}


template<class Texture>
bool gfx_texture2d_regular_atlas<Texture>::is_full() const noexcept
{
	return !m_freeTiles.empty();
}