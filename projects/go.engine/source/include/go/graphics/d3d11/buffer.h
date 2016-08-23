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
    \file graphics/d3d11/buffer.h
    \brief Contains classes that represent different GPU buffers.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/platform/windows.h"
#include "go/graphics/d3d11/resource.h"


GO_BEGIN_NAMESPACE

//! Enumeration of possible update frequencies.
enum class gfx_buffer_update_frequency
{
    //! The number of updates is unknown.
    unknown,
    //! The buffer will be initialized once and cannot be changed.
    never,
    //! The buffer will be updated less than once per frame.
    low,
    //! The buffer will be updated more than once per frame.
    high
};


//! A base class, which represents a graphics buffer.
class GO_API gfx_buffer
{
    GO_DECLARE_NON_COPYABLE(gfx_buffer)
public:
    //! Destructor.
    virtual ~gfx_buffer() = default;
public:
    //! Releases all internal resources.
    virtual void release();
    /*!
        Locks the buffer for an update.

        Do not call this function if the buffer was created with the update frequency hint
        #go::gfx_buffer::update_frequency::never.

        \return Pointer to a writable location in the data buffer.
    */
    virtual void *begin_update() = 0;
    /*!
        Unlocks a previously locked buffer.

        The buffer should be locked before calling this function using #begin_update.

        Do not call this function if the buffer was created with the update frequency hint
        #go::gfx_buffer::update_frequency::never.
    */
    virtual void end_update() = 0;
    /*!
        Updates the data buffer.

        This is equivalent to updating the buffer by locking it.

        \param data The data buffer.
        \param dataSize The size of the data to copy.
     */
    void update(const void *data, uint32_t dataSize);
    /*!
        Returns the size of the buffer in bytes.

        \return Size of the buffer in bytes.
     */
    uint32_t size() const noexcept { return m_size; }
    /*!
        Returns the internal D3D buffer.

        \return A pointer to the internal D3D buffer.
     */
    ID3D11Buffer *d3d_buffer() const noexcept { return m_d3dBuffer.Get(); }
protected:
    //! Constructor.
    gfx_buffer() = default;
protected:
    //! The D3D buffer object.
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_d3dBuffer;
    /*!
        Size of the buffer in bytes.

        Must be initialized by the inheriting class.
     */
    uint32_t m_size;
};


//! A class, which represents a constant buffer.
class GO_API gfx_constant_buffer : public gfx_buffer
{
public:
    //! \overload
    gfx_constant_buffer() = default;
    /*!
        Constructs the object and initializes it.

        \param size The size of the buffer in bytes.
        \param updateFrequency Indicates the update frequency of the buffer.
        \param initialData Pointer to the initial subresource data.
     */
    gfx_constant_buffer(
        uint32_t size,
        gfx_buffer_update_frequency updateFrequency = gfx_buffer_update_frequency::unknown,
        void *initialData = nullptr
    );
public:
    void *begin_update() override;
    void end_update() override;
public:
    /*!
        Resets the buffer.

        \param size The size of the buffer.
        \param updateFrequency The update frequency.
        \param initialData Optional pointer to the initial data.
     */
    void reset(
        uint32_t size,
        gfx_buffer_update_frequency updateFrequency = gfx_buffer_update_frequency::unknown,
        void *initialData = nullptr
    );
};


//! Enumeration of possible data types.
enum class gfx_stageable_buffer_type
{
    //! The buffer holds vertex data.
    vertex,
    //! The buffer holds index data.
    index
};


//! Enumeration of possible update modes.
enum class gfx_stageable_buffer_update_mode
{
    //! Discards the previous content of the buffer.
    discard,
    /*!
        Appends data to the last data block in the buffer.

        This flag is only supported, if the update frequency of the buffer is set to
        #go::gfx_buffer::update_frequency::high.
     */
    append
};


//! A class, which represents a buffer with an additional staging buffer.
class GO_API gfx_stageable_buffer : public gfx_buffer
{
public:
    /*!
        Constructor.

        \param type The buffer type.
        \param size The size of the buffer in bytes.
        \param updateFrequency Indicates the update frequency of the buffer.
        \param initialData Pointer to the initial subresource data.
     */
    gfx_stageable_buffer(
        gfx_stageable_buffer_type type,
        uint32_t elementCount, uint32_t elementSize,
        gfx_buffer_update_frequency updateFrequency = gfx_buffer_update_frequency::unknown,
        void *initialData = nullptr
    );
    //! \overload
    gfx_stageable_buffer(gfx_stageable_buffer_type type);
    //! Destructor.
    virtual ~gfx_stageable_buffer() {}
public:
    void release() override;
    void *begin_update() override;
    void end_update() override;
public:
    /*!
        Resets the buffer.

        \param size The size of the buffer.
        \param updateFrequency The update frequency.
        \param initialData Optional pointer to the initial data.
     */
    void reset(
        uint32_t elementCount, uint32_t elementSize,
        gfx_buffer_update_frequency updateFrequency = gfx_buffer_update_frequency::unknown,
        void *initialData = nullptr
    );
    /*!
        Locks the buffer for an update.

        \param updateMode The update mode.
     */
    void *begin_update(gfx_stageable_buffer_update_mode updateMode);
    /*!
        Returns the buffer type.

        \return The buffer type.
     */
    gfx_stageable_buffer_type type() const noexcept { return m_type; }
    /*!
        Returns the size of an element in the buffer.

        \return The size of an element in the buffer.
     */
    uint32_t element_size() const noexcept { return m_elementSize; }
private:
    //! An optional staging buffer.
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_stagingBuffer;
    //! Size of a single element in the buffer.
    uint32_t m_elementSize;
    //! The buffer type.
    gfx_stageable_buffer_type m_type;
    //! The buffer update frequency.
    gfx_buffer_update_frequency m_updateFrequency;
};


//! A class, which represents a vertex buffer.
class GO_API gfx_vertex_buffer : public gfx_stageable_buffer
{
public:
    /*!
        Constructor.

        \param elementCount The number of elements the buffer should hold.
        \param elementSize The size of an element in bytes.
        \param updateFrequency Indicates the update frequency of the buffer.
        \param initialData Pointer to the initial subresource data.
     */
    gfx_vertex_buffer(
        uint32_t elementCount, uint32_t elementSize,
        gfx_buffer_update_frequency updateFrequency = gfx_buffer_update_frequency::unknown,
        void *initialData = nullptr
    );
    //! \overload
    gfx_vertex_buffer();
};


//! A class, which represents a constant buffer.
class GO_API gfx_index_buffer : public gfx_stageable_buffer
{
public:
    /*!
        Constructor.

        \param elementCount The total number of elements in the buffer.
        \param elementSize The size of an element in the buffer in bytes.
        \param updateFrequency Indicates the update frequency of the buffer.
        \param initialData Pointer to the initial subresource data.
     */
    gfx_index_buffer(
        uint32_t elementCount, uint32_t elementSize,
        gfx_buffer_update_frequency updateFrequency = gfx_buffer_update_frequency::unknown,
        void *initialData = nullptr
    );
    //! \overload
    gfx_index_buffer();
};


//! Enumeration of possible read-write access-policies to a buffer.
enum class gfx_access_policy
{
    /*!
        All accesses are executed and visible in order.

        This should be used for read-only buffers.
     */
    read_only,
    //! Read-write accesses are not guaranteed to be immediately visible.
    read_write
};


//! Enumeration of possible data type specifiers.
enum class gfx_structured_buffer_data_type
{
    //! The data type is an unknown, user-defined structure.
    user,
    //! The data type is an unknown built-in type.
    native,
    //! Unstructured, raw 32-bit data.
    raw32
};


//! A class, which represents a structured buffer.
class GO_API gfx_structured_buffer : public gfx_buffer, public gfx_shader_resource
{
public:
    /*!
        Constructs the object and initializes it.

        \param elementCapacity The maximum number of elements that the buffer can hold.
        \param elementSize The size of a single element in the buffer.
        \param accessPolicy The GPU-access-policy.
        \param updateFrequency Indicates the update frequency of the buffer.
        \param dataType The data type.
        \param initialData Optional pointer to the initial data.
     */
    gfx_structured_buffer(
        int32_t elementCapacity, uint32_t elementSize,
        gfx_access_policy accessPolicy,
        gfx_buffer_update_frequency updateFrequency = gfx_buffer_update_frequency::unknown,
        gfx_structured_buffer_data_type dataType = gfx_structured_buffer_data_type::user,
        void *initialData = nullptr
    );
    //! \overload
    gfx_structured_buffer() = default;
public:
    void release() override;
    void *begin_update() override;
    void end_update() override;
public:
    /*!
        Resets the buffer.

        \param elementCapacity The maximum number of elements that the buffer can hold.
        \param elementSize The size of a single element.
        \param accessPolicy The GPU-access-policy.
        \param updateFrequency Indicates the update frequency of the buffer.
        \param dataType The data type.
        \param initialData Optional pointer to the initial data.
     */
    void reset(
        int32_t elementCapacity, uint32_t elementSize,
        gfx_access_policy accessPolicy,
        gfx_buffer_update_frequency updateFrequency = gfx_buffer_update_frequency::unknown,
        gfx_structured_buffer_data_type dataType = gfx_structured_buffer_data_type::user,
        void *initialData = nullptr
    );
    /*!
        Returns the number of elements that the buffer can hold.

        \return The number of elements that the buffer can hold.
     */
    int32_t element_capacity() const noexcept { return m_elementCapacity; }
private:
    //! An optional staging buffer.
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_d3dStagingBuffer;
    //! Maximum number of elements in the buffer.
    uint32_t m_elementCapacity;
    //! The buffer update frequency.
    gfx_buffer_update_frequency m_updateFrequency;
};

GO_END_NAMESPACE