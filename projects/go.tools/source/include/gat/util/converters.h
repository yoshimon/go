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

#pragma once

#include "gat/tools/tool_base.h"
#include "gat/files/asset_meta_file.h"


GAT_BEGIN_NAMESPACE

//! A structure, which represents the parameters for the scene converter.
struct scene_converter_parameters
{
    struct
    {
        //! The input scene file path.
        go::file_path sceneFilePath;
    } in;
    struct
    {
        //! The output model file path.
        go::file_path sceneFilePath;
        //! The output model directory path.
        go::file_path assetDirPath;
    } out;
};


//! A structure, which represents the parameters for the texture converter.
struct texture_converter_parameters
{
    struct
    {
        //! The input texture file path.
        go::file_path texFilePath;
        //! Skips the conversion if the target file already exists.
        bool skipIfExisting;
    } in;
    struct
    {
        //! The output texture file path.
        go::file_path texFilePath;
        //! The output texture format.
        tstring format;
    } out;
};


//! A structure, which represents the parameters for the material converter.
struct unity_material_converter_parameters
{
    struct
    {
        //! The input material set file path.
        go::file_path materialSetFilePath;
        //! The input asset directory path.
        go::file_path assetDirPath;
    } in;
    struct
    {
        //! The output asset directory path.
        go::file_path assetDirPath;
    } out;
};


//! A structure, containing the parameters to convert a Unity model.
struct unity_model_converter_parameters
{
    struct
    {
        struct
        {
            //! The input model file path.
            go::file_path modelFilePath;
        } in;
        struct
        {
            //! The output model file path.
            go::file_path modelFilePath;
        } out;
    };
};


//! A structure, containing the parameters to convolve a specular radiance map.
struct specular_ibl_convolver_parameters
{
    struct
    {
        struct
        {
            //! The input model file path.
            go::file_path cubemapFilePath;
            /*!
                The number of iterations to run.

                More iterations can lead to better results.
                Acceptable convergence is usually achieved after ~10 iterations.
             */
            uint32_t iterations;
        } in;
        struct
        {
            //! The output model file path.
            go::file_path cubemapFilePath;
        } out;
    };
};


//! A structure, containing functions for converting assets.
struct converters
{
    /*!
        Converts a Unity scene to a Go compatible format.

        \param cfg The configuration file.
        \param parameters The converter parameters.
        \param metaGenerators The meta generators to add the converted files to.
     */
    static void convert_unity_scene(const config_file &cfg, const scene_converter_parameters &parameters, asset_meta_generators &metaGenerators);
    //! \overload
    static void convert_unity_scene(int argc, const _TCHAR *argv[], const config_file &cfg, asset_meta_generators &metaGenerators);
    /*!
        Converts a texture to a Go compatible format.

        \param cfg The configuration file.
        \param parameters The converter parameters.
        \param metaGenerators The meta generators to add the converted files to.
     */
    static void convert_texture(const config_file &cfg, texture_converter_parameters &parameters, asset_meta_generators &metaGenerators);
    //! \overload
    static void convert_texture(int argc, const _TCHAR *argv[], const config_file &cfg, asset_meta_generators &metaGenerators);
    //! \overload
    static void convert_unity_material_set(int argc, const _TCHAR *argv[], const config_file &cfg, asset_meta_generators &metaGenerators);
    /*!
        Converts a material to a Go compatible format.

        \param cfg The configuration file.
        \param parameters The converter parameters.
        \param metaGenerators The meta generators to add the converted files to.
     */
    static void convert_unity_material_set(const config_file &cfg, const unity_material_converter_parameters &parameters, asset_meta_generators &metaGenerators);
    //! \overload
    static void convolve_specular_radiance_map(int argc, const _TCHAR *argv[], const config_file &cfg, asset_meta_generators &metaGenerators);
    /*!
        Convolves a radiance map with the specular D term of the BRDF.

        \param cfg The configuration file.
        \param parameters The converter parameters.
        \param metaGenerators The meta generators to add the converted files to.
     */
    static void convolve_specular_radiance_map(const config_file &cfg, const specular_ibl_convolver_parameters &parameters, asset_meta_generators &metaGenerators);
    /*!
        Converts a Unity model to a Go compatible format.

        \param cfg The configuration file.
        \param parameters The converter parameters.
        \param metaGenerators The meta generators to add the converted files to.
     */
    static void convert_unity_model(const config_file &cfg, const unity_model_converter_parameters &parameters, asset_meta_generators &metaGenerators);
};

GAT_END_NAMESPACE