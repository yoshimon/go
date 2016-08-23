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
    \file graphics/scene_manager.h
    \brief Contains a class for managing multiple graphics scenes.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/graphics/asset_management/asset_context.h"
#include "go/graphics/scene.h"


GO_BEGIN_NAMESPACE

//! A class, which represents a collection of scenes.
class GO_API gfx_scene_manager
{
    GO_DECLARE_NON_COPYABLE(gfx_scene_manager)
public:
    typedef std::vector<std::unique_ptr<gfx_scene>> scene_vector_t;
public:
    /*!
        Constructor.

        Initializes the scene manager using default values.
     */
    gfx_scene_manager(go::cdefault_value);
    //! \overload
    gfx_scene_manager() = default;
    //! \overload
    gfx_scene_manager(const gfx_asset_context_parameters &assetContextParameters) { reset(assetContextParameters); }
public:
    /*!
        Resets the object.

        \param assetContextParameters The asset context parameters.
     */
    void reset(const gfx_asset_context_parameters &assetContextParameters);
    /*!
        Returns the asset context of this scene manager.

        \return The asset context.
     */
    gfx_asset_context &asset_context() { return m_assetContext; }
    /*!
        Creates a new scene.

        Assets will be shared across scenes.

        \return The new scene object.
     */
    gfx_scene *create_scene();
    /*!
        Destroys an existing scene.

        \param scn The scene to destroy.
     */
    void destroy_scene(gfx_scene *scn);
    /*!
        Returns the scene list.

        \return The scene list.
     */
    const scene_vector_t &scenes() const noexcept { return m_scenes; }
    /*!
        Renders a user-specified scene.

        \param index The scene index.
     */
    void render(size_t index);
    //! Updates all scenes and the asset context.
    void update();
private:
    //! The scene collection.
    gfx_asset_context m_assetContext;
    //! The scene collection.
    scene_vector_t m_scenes;
};

GO_END_NAMESPACE