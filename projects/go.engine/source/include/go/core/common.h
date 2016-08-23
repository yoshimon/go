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
    \file core/common.h
    \brief Contains common type declarations.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/defines.h"
#include "go/stdafx.h"


/*!
    Returns the length of a string.
    Excludes the null-terminator.

    \param String The character array.
 */
#define GO_ARRAY_STRLEN(String) (sizeof(String) / sizeof(String[0]) - 1)


// Custom hash operators
namespace std
{
    template<>
    struct hash<std::tuple<std::string, std::string>>
    {
        size_t operator()(const std::tuple<std::string, std::string> &k) const
        {
            size_t h1 = std::hash<std::string>()(std::get<0>(k));
            size_t h2 = std::hash<std::string>()(std::get<1>(k));
            return h1 ^ (h2 << 1);
        }
    };
}


/*!
    Iterator for Go-macro for-each loops.

    \tparam T The iterator type.
 */
template<typename T>
struct foreach_iterator {
    //! The current iterator index.
    size_t index;
    //! The iterator.
    T iterator;
};


GO_BEGIN_NAMESPACE

//! The common character-type.
#if defined(UNICODE) || defined(_UNICODE)
#error Unicode is not supported yet.
#else
typedef char fschar_type;
#endif

/*!
    Hashing function for C-strings.

    \param str The input string.

    \return The hash value.
 */
extern GO_API size_t hash_c_str(const char *str);

//! A hint to the runtime to use the default values of the context.
struct GO_API cdefault_value {};
//! A hint to the runtime to use the current environment settings for the context.
struct GO_API cenvironment_value {};


//! Default value.
extern GO_API const cdefault_value default_value;
//! Environment value.
extern GO_API const cenvironment_value environment_value;


//! A vector of strings.
typedef std::vector<std::string> string_vector;


//! Enumeration of possible return values for common Go functions.
enum class result
{
    //! The operation succeeded.
    okay,
    //! An invalid argument was passed to the function.
    invalid_argument,
    //! A memory allocation failed.
    out_of_memory,
    //! The hardware is either unsupported or does it does not support a request mode.
    hardware_unsupported
};


typedef std::vector<size_t> index_vector;


/*!
    A structure, which describes a synchronization relationship between a source and a target.
    The target is a scaled version of the source.

    \param Dimension The number of scale-dimensions.
 */
template<size_t Dimension>
struct scaled_synchronization
{
    //! The scaling parameters.
    float scale[Dimension];
};


//! A structure, which represents a rectangle.       
template<typename T>
struct rect
{
    /*!
        Constructor.

        \param l Optional left-side coordinate.
        \param t Optional top-side coordinate.
        \param r Optional right-side coordinate.
        \param b Optional bottom-side coordinate.
     */
    rect(T l = 0, T t = 0, T r = 0, T b = 0) : left(std::move(l)), top(std::move(t)), right(std::move(r)), bottom(std::move(b)) {}
    //! The left edge of the rectangle.
    T left;
    //! The top edge of the rectangle.
    T top;
    //! The right edge of the rectangle.
    T right;
    //! The bottom edge of the rectangle.
    T bottom;
    /*!
        Calculates the width of the rectangle.

        \return The width of the rectangle.
     */
    T width() const noexcept
    {
        return right - left;
    }
    /*!
        Calculates the width of the rectangle.

        \return The width of the rectangle.
     */
    T height() const noexcept
    {
        return bottom - top;
    }
};


//! A structure, which represents a point.
struct point
{
    //! Horizontal coordinate.
    int32_t x;
    //! Vertical coordinate.
    int32_t y;
};


//! A structure, which represents a 3x4, column-major matrix.
struct float3x4
{
    //! The first column.
    float c0[4];
    //! The second column.
    float c1[4];
    //! The third column.
    float c2[4];
};


//! A vector of bytes.
typedef std::vector<uint8_t> byte_vector;


//! An integer type, which is used to identify objects.
typedef size_t handle_t;
//! An invalid handle value.
const handle_t invalid_handle_value = (handle_t)-1;


/*!
    Returns whether a value lies within a given range.

    \tparam T The typename.

    \param value The value to test.
    \param min The lower bound.
    \param max The upper bound.
 */
template<typename T>
inline bool in_range(const T &value, const T &min, const T &max)
{
    return value >= min && value <= max;
}


/*!
    Clamps a value.

    \tparam T The typename of the value to normalize.
    \tparam R The typename of the range to normalize with.

    \param value The value to clamp.
    \param minValue The minimum value of the signal.
    \param maxValue The maximum value of the signal.

    \return The clamped value.
 */
template<typename T, typename R>
inline T clamp(const T &value, const R &minValue, const R &maxValue)
{
    return value < minValue ? minValue : (value > maxValue ? maxValue : value);
}

/*!
    Normalizes a value.
    The normalized range is [0, +1].

    \tparam T The typename of the value to normalize.
    \tparam R The typename of the range to normalize with.

    \param signal The signal to normalize.
    \param minValue The minimum value of the signal.
    \param maxValue The maximum value of the signal.

    \return The normalized signal.
 */
template<typename T, typename R>
inline float normalize(const T &value, const R &minValue, const R &maxValue)
{
    // Shift so that minValue lies at 0.
    // auto shiftedValue = -minValue + value;
    // auto shiftedMax = -minValue + maxValue;
    return static_cast<float>((-minValue + value)) / (-minValue + maxValue);
}


/*!
    Normalizes a value.
    The normalized range is [-1, +1].

    \param signal The signal to normalize.
    \param minValue The minimum value of the signal.
    \param maxValue The maximum value of the signal.

    \return The normalized signal.
 */
template<typename T>
inline float normalize_symmetric(const T &value, const T &minValue, const T &maxValue)
{
    return normalize(value, minValue, maxValue) * 2.0f - 1.0f;
}


/*!
    Returns whether a point lies within a rectangle.

    \tparam T The typename of the corners of the rectangle.

    \param x The horizontal coordinate of the point.
    \param y The vertical coordinate of the point.
    \param topLeft The top-left corner of the rectangle.
    \param bottomRight The bottom right of the rectangle.
 */
template<typename T>
inline bool point_in_rect(float x, float y, const T &topLeft, const T &bottomRight)
{
    return x >= topLeft.x && y >= topLeft.y && x <= bottomRight.x && y <= bottomRight.y;   
}


//! \overload
template<typename T>
inline bool point_in_rect(float x, float y, const T &rect)
{
    return x >= rect.left && y >= rect.top && x <= rect.right && y <= rect.bottom;   
}


/*!
    Returns whether a point lies within a rectangle.

    \tparam T The typename of the corners of the rectangle.

    \param x The horizontal coordinate of the point.
    \param y The vertical coordinate of the point.
    \param topLeft The top-left corner of the rectangle.
    \param size The size right of the rectangle.
 */
template<typename T>
inline bool point_in_centered_rectsize(float x, float y, const T &center, const T &size)
{
    float hsx = size.x * 0.5f;
    float hsy = size.y * 0.5f;
    return x >= (center.x - hsx) && y >= (center.y - hsy) && x <= (center.x + hsx) && y <= (center.y + hsy);   
}


/*!
    Returns whether a point lies within a rectangle.

    \tparam T The typename of the corners of the rectangle.

    \param x The horizontal coordinate of the point.
    \param y The vertical coordinate of the point.
    \param topLeft The top-left corner of the rectangle.
    \param size The size right of the rectangle.
 */
template<typename T>
inline bool point_in_rectsize(float x, float y, const T &topLeft, const T &size)
{
    return x >= topLeft.x && y >= topLeft.y && x <= (topLeft.x + size.x) && y <= (topLeft.y + size.y);   
}


/*!
    Helper class.

    \tparam The container class.
 */
template<class T>
class const_reverse_wrapper
{
    const T &container;
public:
    const_reverse_wrapper(const T &cont) : container(cont){ }
    auto begin() const -> decltype(container.rbegin()) { return container.rbegin(); }
    auto end() const -> decltype(container.rend()) { return container.rend(); }
};

/*!
    Helper class.

    \tparam T The container class.
 */
template<class T>
class reverse_wrapper
{
    T &container;
public:
    reverse_wrapper(T &cont) : container(cont){ }
    auto begin() -> decltype(container.rbegin()) { return container.rbegin(); }
    auto end() -> decltype(container.rend()) { return container.rend(); }
};


/*!
    For use with range-based for loops.
    
    \tparam T The container class.

    \param cont The container to iterate.
 */
template<class T>
const_reverse_wrapper<T> reverse(const T &cont)
{
    return const_reverse_wrapper<T>(cont);
}
/*!
    For use with range-based for loops.

    \tparam T The container class.

    \param cont The container to iterate.
 */
template<class T>
reverse_wrapper<T> reverse(T &cont)
{
    return reverse_wrapper<T>(cont);
}


//! Enumeration of possible comparison functions.
enum class comparison_function
{
    //! Never pass the comparison.
    never = 1, // = D3D11_COMPARISON_NEVER,
    //! If the source data is less than the destination data, the comparison passes.
    less, // = D3D11_COMPARISON_LESS,
    //! If the source data is equal to the destination data, the comparison passes.
    equal, // = D3D11_COMPARISON_EQUAL,
    //! If the source data is less than or equal to the destination data, the comparison passes.
    less_equal, // = D3D11_COMPARISON_LESS_EQUAL,
    //! If the source data is greater than the destination data, the comparison passes.
    greater, // = D3D11_COMPARISON_GREATER,
    //! If the source data is not equal to the destination data, the comparison passes.
    not_equal, // = D3D11_COMPARISON_NOT_EQUAL,
    //! If the source data is greater than or equal to the destination data, the comparison passes.
    greater_equal, // = D3D11_COMPARISON_GREATER_EQUAL,
    //! Always pass the comparison.
    always // = D3D11_COMPARISON_ALWAYS
};


/*!
    A structure, which represents a bounding box in n-dimensional space.

    \tparam PointType The type of a point.
 */
template<class PointType>
struct bounding_box
{
    //! The first anchor point.
    PointType p1;
    //! The second anchor point.
    PointType p2;
};


/*!
    A structure, which represents a bounding sphere.

    \tparam PointType The type of a point.
 */
template<class PointType>
struct bounding_sphere
{
    //! The center of the sphere.
    PointType center;
    //! The radius of the sphere.
    float radius;
};


/*!
    Returns whether a value is in a set of values.

    \tparam T The type of the value to test.
    \tparam Arg The type of the value to test against.

    \return If the input value is in the set, the return value is true. Otherwise,
    the return value is false.
 */
template<class T, class Arg>
bool is_in_set(const T &value, Arg arg)
{
    return value == arg;
}


/*!
    Returns whether a value is in a set of values.

    \tparam T The type of the value to test.
    \tparam Arg The type of the values to test against.
    \tparam Args The type of the values to test against.

    \return If the input value is in the set, the return value is true. Otherwise,
    the return value is false.
 */
template<class T, class Arg, class... Args>
bool is_in_set(const T &value, Arg arg, Args&&... args)
{
    return is_in_set(value, arg) || is_in_set(value, args...);
}


#if defined(GO_USE_DIRECT3D11) && (defined(_DEBUG) || defined(PROFILE))
/*!
    Helper function to set the debug name of a D3D object.

    \tparam T The resource type.
    \tparam NameLen The length of the object name.
 */
template<class T, uint32_t NameLen>
inline void d3d_debug_name(T resource, const char (&name)[NameLen])
{
    resource->SetPrivateData(WKPDID_D3DDebugObjectName, NameLen - 1, name);
}

/*!
    Helper function to set the debug name of a D3D object.

    \tparam T The resource type.
    \tparam NameLen The length of the object name.
 */
template<class T>
inline void d3d_debug_name(T resource, const std::string &name)
{
    resource->SetPrivateData(WKPDID_D3DDebugObjectName, name.length(), name.c_str());
}
#else
#define d3d_debug_name(...)
#endif


/*!
    Returns, whether a number is a power of two.

    \number The number to test.

    \return If the number is a power of two, the return value is true. Otherwise,
    the return value is false.
 */
inline bool is_power_of_two(int32_t number)
{
    return !((number != 1) && (number & (number - 1)));
}


/*!
    Rounds a given value to the next power of two.

    \param value The value to round up.

    \return The rounded up value.
 */
inline uint32_t ceil_power_of_two(uint32_t value)
{
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value + 1;
}


/*!
    Rounds a given value to the next power of two.

    \param value The value to round up.

    \return The rounded up value.
 */
inline int32_t ceil_power_of_two(int32_t value)
{
    return value < 0 ? 0 : ceil_power_of_two((uint32_t)value);
}


/*!
    Returns a bucket index from a slot index.

    \tparam BucketType The type of the bucket.
    \tparam SlotType The type of the slot index.

    \param slot The slot index.
 */
template<typename BucketType, typename SlotType>
inline BucketType bucket_index_from_slot(SlotType slot)
{
    return slot / (8 * sizeof(BucketType));
}

/*!
    Returns a bitmask which has a single bit set for a given index.

    \tparam BucketType The type of the bucket.
    \tparam SlotType The type of the slot index.

    \param bucketIndex The index of the bucket.
    \param slot The slot index to set.
 */
template<typename BucketType, typename SlotType>
inline BucketType bucket_bitmask_set(BucketType bucketIndex, SlotType slot)
{
    return (BucketType)1 << (slot - (bucketIndex * 8 * sizeof(BucketType)));
}


//! A structure, which represents a 128-bit integer.
struct uint128_t
{
    //! The lower 64 bits.
    uint64_t lo;
    //! The upper 64 bits.
    uint64_t hi;
};

//! 32-bit boolean value.
typedef int32_t bool32_t;
//! Signed resource identifier.
typedef int32_t rid;
//! A vector of resource identifiers.
typedef std::vector<rid> rid_vector;

/*!
    Swap-erases an element from a container.

    \param c The container.
    \param index The index to delete.
 */
template<typename Container>
void swap_erase(Container &c, size_t index)
{
    std::swap(c.begin() + index, c.end() - 1);
    c.pop_back();
}


/*!
    Swap-erases an element from a container.

    \param c The container.
    \param iterator The iterator pointing to the element to delete.
 */
template<typename Container>
void swap_erase(Container &c, typename const Container::const_iterator &iterator)
{
    std::swap(iterator, c.end() - 1);
    c.pop_back();
}


/*!
    Performs an float division followed by a ceil-operation.

    \param a The dividend.
    \param b The divisor.

    \return The ceiled quotient.
 */
inline int ceil_div(int a, int b)
{
    int c = a / b;
    return a % b == 0 ? c : c + 1;
}


/*!
    Copies a float2 value to a target buffer.

    \param target The target buffer.
    \param value The value to copy.
 */
inline void copy_float2(float *target, float x, float y)
{
    GO_ASSERT(target);

    target[0] = x;
    target[1] = y;
}


//! \overload
template<typename Vector>
void copy_float2(float *target, const Vector &value)
{
    copy_float2(target, value.x, value.y);
}


/*!
    Copies a float3 value to a target buffer.

    \param target The target buffer.
    \param value The value to copy.
 */
inline void copy_float3(float *target, float x, float y, float z)
{
    GO_ASSERT(target);

    target[0] = x;
    target[1] = y;
    target[2] = z;
}


//! \overload
template<typename Vector>
void copy_float3(float *target, const Vector &value)
{
    copy_float3(target, value.x, value.y, value.z);
}


/*!
    Copies a float3 value to a target buffer.

    \param target The target buffer.
    \param value The value to copy.
 */
inline void copy_float4(float *target, float x, float y, float z, float w)
{
    GO_ASSERT(target);

    target[0] = x;
    target[1] = y;
    target[2] = z;
    target[3] = w;
}


//! \overload
template<typename Vector>
void copy_float4(float *target, const Vector &value)
{
    copy_float4(target, value.x, value.y, value.z, value.w);
}

GO_END_NAMESPACE
