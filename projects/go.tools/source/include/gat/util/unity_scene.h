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

#include "gat/defines.h"
#include "gat/files/gmd_file.h"
#include "go/golib.h"


GAT_BEGIN_NAMESPACE

//! Represents a Unity entity.
struct unity_entity
{
    //! The entity name.
    std::string name;
    //! The entity ID.
    int id;
    //! The parent ID.
    int parentID;
    //! The entity tag.
    std::string tag;
    //! Static flag.
    bool isStatic;
    //! Unknown usage.
    bool activeSelf;
    //! Unknown usage.
    bool activeInHierarchy;
    //! Hide flags.
    std::string hideFlags;
    //! The entity position.
    DirectX::XMFLOAT3 position;
    //! The entity rotation.
    DirectX::XMFLOAT4 rotation;
    //! The entity scale.
    float scale;
    //! Shadow caster flag.
    bool castsShadows;
    //! Shadow caster flag.
    bool castsShadowsOnly;
    struct
    {
        struct
        {
            //! The model meta file asset path.
            go::file_path modelMetaFileAssetPath;
		} meshFilter;
        //! State flag.
        bool hasMeshFilter = false;
        struct
        {
            //! The probe importance.
            float importance;
            //! The probe resolution per face in pixels.
            int resolution;
            //! The probe box-projection volume.
            bool hasProxyBoxProj;
			//! The size of the proxy box.
            DirectX::XMFLOAT3 proxySize;
            //! The proxy center offset.
            DirectX::XMFLOAT3 proxyCenterOffset;
            //! The proxy bounds center.
            DirectX::XMFLOAT3 proxyBoundsCenter;
            //! The proxy bounds size.
            DirectX::XMFLOAT3 proxyBoundsSize;
        } reflectionProbe;
        //! State flag.
        bool hasReflectionProbe = false;
        struct
        {
            //! The light type.
            std::string type;
            //! The intensity of the light.
            float intensity;
            //! The radius of the light.
            float radius;
            //! Flag, which indicates whether the light casts shadows.
            bool castsShadows;
            //! The light color.
            DirectX::XMFLOAT4 color;
        } light;
        //! State flag.
        bool hasLight = false;
    } components;
};

//! A class, which represents a Unity scene.
class unity_scene
{
public:
    typedef std::vector<unity_entity> entity_vector_t;
public:
    //! Constructor.
    unity_scene() = default;
    //! \overload
    unity_scene(const go::file_path &filePath);
public:
    /*!
        Loads the scene from a file.
     */
    void load(const go::file_path &filePath);
    /*!
        Returns the entity list.

        \return The entity list.
     */
    const entity_vector_t &entities() const noexcept;
    /*!
        Returns the data directory path.
        
        \return The data directory path.
     */
    const go::file_path &data_directory_path() const noexcept;
private:
    //! The entities in the scene.
    entity_vector_t m_entities;
    //! The data directory path.
    go::file_path m_dataDirPath;
};

GAT_END_NAMESPACE