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
    \file world/scene.h
    \brief Contains classes which represent the game world.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/short_string.h"
#include "go/core/aligned.h"
#include "go/graphics/camera_properties.h"
#include "go/graphics/lights.h"
#include "go/graphics/sky.h"
#include "go/graphics/render_context.h"
#include "go/graphics/entity.h"
#include "go/graphics/entity_component_manager.h"
#include "go/variables.h"


GO_BEGIN_NAMESPACE

class gfx_scene_manager;

//! A class, which represents a 3D scene.
class GO_SSE_ALIGN gfx_scene : public aligned<gfx_scene>
{
public:
    typedef name_string entity_name_t;
    typedef std::vector<DirectX::XMFLOAT4X4A> float4x4_vector_t;
    typedef std::vector<DirectX::XMFLOAT4A> float4_vector_t;
    typedef std::vector<float> float_vector_t;
    /*!
        A structure, which holds information about the entities in the scene.

        Represents the minimum amount of component data that is attached to an entity.
     */
    struct entity_data_t
    {
        //! The position of the object.
        float4_vector_t positions;
        //! The orientation vector.
        float4_vector_t orientations;
        //! The rotation vector.
        float4_vector_t rotations;
        //! The entity scales.
        float_vector_t scales;
        /*!
            Transformation matrix cache for entities.

            The transforms represent the final world matrix to use for rendering.
         */
        float4x4_vector_t worldTransforms;
    };
    typedef std::vector<std::unique_ptr<gfx_entity_component_manager>> component_manager_vector_t;
    typedef std::vector<entity_name_t> entity_name_vector_t;
    typedef std::vector<size_t> component_manager_id_vector_t;
    typedef std::vector<bool32_t> shadow_caster_flag_vector_t;
    typedef std::vector<gfx_entity> entity_vector_t;
	//! A structure, which holds additional data for disk lights.
	struct disk_light_data
	{
		//! Half of the outer angle.
		float halfOuterAngle;
		//! The rcp of the tan of half of the outer angle.
		float rcpTanHalfOuterAngle;
		//! The cosine of the outer angle.
		float cosOuter;
		//! The cosine of the inner angle.
		float cosInner;
		//! The angular scale.
		float angleScale;
		//! The angular bias.
		float angleBias;
	};
	typedef std::vector<disk_light_data> disk_light_data_vector_t;
public:
    /*!
        Constructor.

        \param sceneMgr The scene manager.
        \param entityCacheSize Upper bound for the number of entities in the scene.
     */
    GO_API gfx_scene(gfx_scene_manager *sceneMgr, size_t entityCacheSize = GO_ENTITY_MAX_COUNT_HINT);
    //! Destructor.
    GO_API ~gfx_scene();
public:
    /*!
        Creates a new entity.

        \param parent The parent entity.
        \param name The name of the entity.

        \return The new entity.
     */
    GO_API gfx_entity *create_entity(gfx_entity_id parent = -1, const entity_name_t &name = "");
    /*!
        Destroys an existing entity.

        \param id The entity identifier.
     */
    GO_API void destroy_entity(gfx_entity_id id);
    /*!
        Searches for an existing entity.

        \param id The entity identifier.

        \return The entity pointer or nullptr, if the identifier is invalid.
     */
    GO_API gfx_entity *find_entity(gfx_entity_id id);
    //! \overload
    GO_API void find_entity(const entity_name_t &name, entity_vector_t &outEntities);
    /*!
        Returns a list of all entities in the scene.

        \return The entity list.
     */
    GO_API const entity_vector_t &entities() const noexcept;
    /*!
        Changes the active camera.

        \param newValue The entity identifier.
     */
    GO_API void change_active_camera(gfx_entity_id newValue) noexcept;
    /*!
        Translates an entity.

        \param id The entity identifier.
        \param vDelta The translation vector.
     */
    GO_API void translate_entity(gfx_entity_id id, DirectX::FXMVECTOR vDelta) noexcept;
    /*!
        Rotates an entity.

        \param id The entity identifier.
        \param pitch Delta-pitch.
        \param yaw Delta-yaw.
        \param roll Delta-pitch.
     */
    GO_API void rotate_entity(gfx_entity_id id, float pitch, float yaw, float roll) noexcept;
    //! \overload
    GO_API void rotate_entity(gfx_entity_id id, DirectX::FXMVECTOR vQuat) noexcept;
    /*!
        Scales an entity.

        \param id The entity identifier.
        \param delta The scale modifier.
     */
    GO_API void scale_entity(gfx_entity_id id, float delta) noexcept;
    /*!
        Changes the scale of an entity.

        \param id The entity identifier.
        \param scale The new scale.
     */
    GO_API void change_entity_scale(gfx_entity_id id, float scale) noexcept;
    /*!
        Returns the active camera.

        \return The active camera.
     */
    GO_API gfx_entity_id active_camera() const noexcept;
    /*!
        Returns the current skydome descriptor.

        \return The current skydome descriptor.
     */
    GO_API go::gfx_ambient_sky &sky() noexcept;
    /*!
        Returns the camera property descriptor.

        \return The camera property descriptor.
     */
    GO_API gfx_camera_properties &camera_properties() noexcept;
    /*!
        Grows the entity cache.

        \param newCacheSize The new cache size.
     */
    GO_API void grow_entity_cache(size_t newCacheSize);
    /*!
        Returns the entity data list.

        \return The entity data list.
     */
    GO_API const entity_data_t &entity_data() const noexcept;
    /*!
        Attaches a spherical-light to an entity.

        \param id The entity identifier.
        \param color The light color.
		\param luminousPower The luminious power of the light.
		\param size The size of the light (area light).
        \param castsShadows State flag, indicating whether the light can cast shadows.
     */
    GO_API void attach_spherical_light(gfx_entity_id id, const DirectX::XMFLOAT3 &color, float luminousPower, float size, bool castsShadows);
    /*!
        Detaches a spherical-light from an entity.

        \param id The entity identifier.
     */
    GO_API void detach_spherical_light(gfx_entity_id id);
    /*!
        Returns the spherical-light list.

        \return The spherical-light list.
     */
    GO_API const gfx_spherical_lights &spherical_lights() const noexcept;
    /*!
        Attaches a disk-light to an entity.

        \param id The entity identifier.
        \param color The light color.
		\param luminousPower The luminious power of the light.
        \param size The size of the light (area light).
		\param coneOuterAngle The outer cone angle of the light.
		\param coneInnerAngle The inner cone angle of the light.
        \param castsShadows State flag, indicating whether the light can cast shadows.
     */
    GO_API void attach_disk_light(gfx_entity_id id, const DirectX::XMFLOAT3 &color, float luminousPower, float size, float coneOuterAngle, float coneInnerAngle, bool castsShadows);
    /*!
        Detaches a disk-light from an entity.

        \param id The entity identifier.
     */
    GO_API void detach_disk_light(gfx_entity_id id);
    /*!
        Returns the disk-light list.

        \return The disk-light list.
     */
    GO_API const gfx_disk_lights &disk_lights() const noexcept;
	/*!
		Returns the list of entities that have disk-lights attached to them.

		\return The entities that have disk lights attached to them.
	 */
	GO_API const gfx_entity_id_vector &disk_light_entities() const noexcept;
	/*!
		Changes the color of a disk light.

		\param lightIndex The disk light index.
		\param color The new color.
		\param luminousPower The luminous power of the light.
	 */
	GO_API void change_disk_light_color(int32_t lightIndex, const DirectX::XMFLOAT3 &color, float luminousPower);
	/*!
        Registers a new component manager with the scene.

        \tparam T The component manager class.
        \tparam Args The arguments to forward to the component manager.

        \param componentCountHint The max-estimation of the number of components to manage.

        \return The new component manager or nullptr if no component manager was created.
     */
    template<class T, class... Args>
    T *register_entity_component_manager(size_t componentCountHint, Args &&... args);
    /*!
        Searches for an entity component manager.

        \tparam T The component manager to search for.
    */
    template<class T>
    T *find_entity_component_manager();
    //! Updates the scene.
    GO_API void update();
    //! Renders the scene.
    GO_API void render();
    /*!
        Changes the bounding-sphere of an entity.

        \remarks Mostly intended for internal-use only.

        \param id The entity identifier.
        \param boundingSphere The bounding-sphere.
     */
    GO_API void change_bounding_sphere(gfx_entity_id id, const DirectX::XMFLOAT4A &boundingSphere);
    /*!
        Returns a reference to the current render-context.

        \return The render-context.
     */
    GO_API gfx_render_context &render_context() noexcept;
    /*!
        Returns the scene manager.

        \return The scene manager.
     */
    GO_API gfx_scene_manager *scene_manager() noexcept;
private:
    //! Computes the camera frustum.
    void compute_camera_frustum();
    //! Computes the directional light frustum.
    void compute_directional_light_frustum();
    /*!
        Resolves an entity dependency by sorting the entity list.

        \param id The entity identifier.
        \param parent The parent identifier.
     */
    void resolve_parent_dependency(gfx_entity_id id, gfx_entity_id parent);
    //! Updates all component managers.
    void update_component_managers();
    //! Calculates the updated world transformations.
    void calculate_world_transforms();
    //! Determines which lights cast shadows.
    void determine_shadow_casting_lights();
    //! Flushes the render-context.
    void flush_command_buffers();
    //! Compacts the shadow-caster point light list.
    void compact_shadow_casting_point_lights();
private:
    //! The scene manager.
    gfx_scene_manager *m_sceneMgr;

    //! Data about the entities in the scene.
    entity_data_t m_entityData;
    //! The entities in the scene.
    entity_vector_t m_entities;
    //! Vector containing the entity names.
    entity_name_vector_t m_entityNames;
    //! The component managers.
    component_manager_vector_t m_componentManagers;
    //! The component manager identifiers.
    component_manager_id_vector_t m_componentManagerIDs;
    
    //! State flag.
    bool m_isRendering;
    
    //! The active camera.
    gfx_entity_id m_activeCamera;
    
    //! The render context.
    gfx_render_context m_renderContext;
    
    //! The list of all spherical lights.
    gfx_entity_id_vector m_entitySphericalLights;
    //! State flag for shadow-casting spherical lights.
    shadow_caster_flag_vector_t m_entitySphericalLightShadowCasterFlags;

    //! The list of all disk lights.
    gfx_entity_id_vector m_entityDiskLights;
    //! State flag for shadow-casting disk lights.
    shadow_caster_flag_vector_t m_entityDiskLightShadowCasterFlags;
	//! Properties for the disk lights.
	disk_light_data_vector_t m_diskLightData;
};


#include "scene_impl.h"

GO_END_NAMESPACE
