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
    \file graphics/asset_management/asset_context.h
    \brief Contains a class for holding different assets.

    \author Gokhan Ozdogan
    \version 1.0
 */


#pragma once


#include "go/core/common.h"

#include "go/filesystem/system.h"

#include "go/graphics/asset_management/material_database.h"
#include "go/graphics/asset_management/material_manager.h"

#include "go/graphics/asset_management/model_database.h"
#include "go/graphics/asset_management/model_manager.h"

#include "go/graphics/asset_management/texture_database.h"
#include "go/graphics/asset_management/texture_manager.h"

#include "go/graphics/asset_management/shader_manager.h"
#include "go/graphics/model_cache.h"


GO_BEGIN_NAMESPACE

//! A structure holding information about the asset context parameters.
struct gfx_asset_context_parameters
{
    //! The material database parameters.
    asset_db_load_parameters materialDB;
    //! The model database parameters.
    asset_db_load_parameters modelDB;
    //! The shader directory.
    go::file_path shaderDirPath;
    //! The texture database parameters.
    asset_db_load_parameters textureDB;
    //! Information about the default textures.
    gfx_texture_manager::default_textures_info_t defaultTexturesInfo;
};


/*!
    A class, which combines all graphics asset managers and databases.

    \tparam ModelMgr The model manager class.
    \tparam MaterialMgr The material manager class.
    \tparam TextureMgr The texture manager class.
    \tparam ShaderMgr The shader manager class.
    \tparam ModelCache The model cache class.
 */
class GO_API gfx_asset_context
{
public:
    //! Constructor.
    gfx_asset_context() = default;
    //! \overload
    gfx_asset_context(const gfx_asset_context_parameters &parameters) { reset(parameters); }
    //! Destructor.
    virtual ~gfx_asset_context() = default;
public:
    /*!
        Resets the object.
        
        \param parameters The parameters to apply.
     */
    void reset(const gfx_asset_context_parameters &parameters);
    //! Updates all asset managers within the context.
    void update();
    /*!
        Returns the current parameter set that is being used.

        \return The current parameters set.
     */
    const gfx_asset_context_parameters &parameters() const noexcept { return m_parameters; }
    /*!
        Returns the material database.

        \return The material database.
     */
    gfx_material_database &material_db() { return *m_materialDB.get(); }
    /*!
        Returns the material manager.

        \return The material manager.
     */
    gfx_material_manager &material_mgr() { return *m_materialMgr.get(); }
    /*!
        Returns the material database.

        \return The material database.
     */
    gfx_model_database &model_db() { return *m_modelDB.get(); }
    /*!
        Returns the model manager.

        \return The model manager.
     */
    gfx_model_manager &model_mgr() { return *m_modelMgr.get(); }
    /*!
        Returns the material database.

        \return The material database.
     */
    gfx_texture_database &texture_db() { return *m_textureDB.get(); }
    /*!
        Returns the model manager.

        \return The model manager.
     */
    gfx_texture_manager &texture_mgr() { return *m_textureMgr.get(); }
    /*!
        Returns the shader manager.

        \return The shader manager.
     */
    gfx_shader_manager &shader_mgr() { return *m_shaderMgr.get(); }
    /*!
        Returns the model cache.

        \return The model cache.
     */
    gfx_model_cache &model_cache() { return *m_modelCache.get(); }
protected:
    //! The material database.
    std::unique_ptr<gfx_material_database> m_materialDB;
    //! The material manager.
    std::unique_ptr<gfx_material_manager> m_materialMgr;
    //! The model database.
    std::unique_ptr<gfx_model_database> m_modelDB;
    //! The model manager.
    std::unique_ptr<gfx_model_manager> m_modelMgr;
    //! The texture database.
    std::unique_ptr<gfx_texture_database> m_textureDB;
    //! The texture manager.
    std::unique_ptr<gfx_texture_manager> m_textureMgr;
    //! The shader manager.
    std::unique_ptr<gfx_shader_manager> m_shaderMgr;
    //! The model cache.
    std::unique_ptr<gfx_model_cache> m_modelCache;
    //! The context parameters.
    gfx_asset_context_parameters m_parameters;
};

GO_END_NAMESPACE