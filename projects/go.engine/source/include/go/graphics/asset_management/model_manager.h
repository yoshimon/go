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
    \file graphics/asset_management/model_manager.h
    \brief Contains a class for managing model resources.

    \author Gokhan Ozdogan
    \version 1.0
*/

#pragma once


#include "go/core/common.h"
#include "go/core/path_utility.h"
#include "go/design/concurrent_resource_manager.h"
#include "go/graphics/model.h"
#include "go/graphics/asset_management/model_database.h"
#include "go/filesystem/async.h"


GO_BEGIN_NAMESPACE

class gfx_model_manager;

//! A helper class for loading model files.
struct GO_API gfx_model_loader : public concurrent_resource_loader<gfx_model, gfx_model_manager>
{
public:
    //! Constructor.
    gfx_model_loader();
public:
    bool load(manager_ptr_t mgr, resource_desc_ptr_t desc) override;
    bool load_async(manager_ptr_t mgr, resource_desc_ptr_t desc) override;
    bool reload_async(manager_ptr_t mgr, resource_desc_ptr_t desc) override;
    void unload(resource_ptr_t resource) override;
private:
    //! The async model loader.
    std::unique_ptr<go::async_io_callback> m_asyncLoader;
};


//! A class, which manages model resources.
class GO_API gfx_model_manager : public concurrent_resource_manager<gfx_model_loader>
{
public:
    /*!
        Constructor.

        \param modelDB The model database.
        \param materialDB The material database.
     */
    gfx_model_manager(gfx_model_database &modelDB, gfx_material_database &materialDB);
public:
    /*!
        Returns the model database.

        \return The model database.
     */
    gfx_model_database &model_db() noexcept;
    /*!
        Returns the material database.

        \return The material database.
     */
    gfx_material_database &material_db() noexcept;
    /*!
        Returns the default model.

        \return The default model.
     */
    gfx_model *default_model() noexcept;
private:
    //! The default model (empty model).
    gfx_model *m_defaultModel;
    //! The model database.
    gfx_model_database &m_modelDB;
    //! The material database.
    gfx_material_database &m_materialDB;
};

GO_END_NAMESPACE
