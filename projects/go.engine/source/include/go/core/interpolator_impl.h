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
// vector3_interpolator member functions
// ================================================================================ //

template<typename SampleData, typename TangentData>
inline auto vector3_interpolator<SampleData, TangentData>::interpolate_hermite(const SampleData &a,
    const SampleData &b, const TangentData &ta, const TangentData &tb, float t) -> SampleData
{
    auto t2 = t * t;
    auto t3 = t2 * t;

    auto h1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
    auto h2 = -2.0f * t3 + 3.0f * t2;
    auto h3 = t3 - 2.0f * t2 + t;
    auto h4 = t3 - t2;

    SampleData r;
    r.x = h1 * a.x + h2 * b.x + h3 * ta.x + h4 * tb.x;
    r.y = h1 * a.y + h2 * b.y + h3 * ta.y + h4 * tb.y;
    r.z = h1 * a.z + h2 * b.z + h3 * ta.z + h4 * tb.z;
    return r;
}


template<typename SampleData>
inline auto vector3_interpolator<SampleData, void>::interpolate_linear(const SampleData &a,
    const SampleData &b, float t) -> SampleData
{
    SampleData r;
    r.x = a.x + (b.x - a.x) * t;
    r.y = a.y + (b.y - a.y) * t;
    r.z = a.z + (b.z - a.z) * t;
    return r;
}


// ================================================================================ //
// sample_tangent_interpolator member functions
// ================================================================================ //

template<typename SampleData>
template<typename TangentData>
typename sample_tangent_interpolator<SampleData, TangentData>::function_type
sample_interpolator<SampleData>::to_tangent_interpolator(const function_type &f)
{
    return [=](const SampleData &a, const SampleData &b, float t, const TangentData &, const TangentData &)
    {
        return f(a, b, t);  
    };
}


// ================================================================================ //
// rgba_interpolator member functions
// ================================================================================ //

template<typename SampleData, typename TangentData>
inline auto rgba_interpolator<SampleData, TangentData>::cubic_hermite(
    const SampleData &a, const SampleData &b, float t, const TangentData &ta, const TangentData &tb)
    -> SampleData
{
    auto t2 = t * t;
    auto t3 = t2 * t;

    auto h1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
    auto h2 = -2.0f * t3 + 3.0f * t2;
    auto h3 = t3 - 2.0f * t2 + t;
    auto h4 = t3 - t2;

    SampleData r;
    r.r = h1 * a.r + h2 * b.r + h3 * ta.r + h4 * tb.r;
    r.g = h1 * a.g + h2 * b.g + h3 * ta.g + h4 * tb.g;
    r.b = h1 * a.b + h2 * b.b + h3 * ta.b + h4 * tb.b;
    r.a = h1 * a.a + h2 * b.a + h3 * ta.a + h4 * tb.a;
    return r;
}


// ================================================================================ //
// rgba_interpolator member functions
// ================================================================================ //

template<typename SampleData>
inline auto rgba_interpolator<SampleData, void>::lerp(
    const SampleData &a, const SampleData &b, float t) -> SampleData
{
    SampleData r;
    r.r = a.r + (b.r - a.r) * t;
    r.g = a.g + (b.g - a.g) * t;
    r.b = a.b + (b.b - a.b) * t;
    r.a = a.a + (b.a - a.a) * t;
    return r;
}


// ================================================================================ //
// step_interpolator member functions
// ================================================================================ //

template<typename SampleData, typename U>
inline auto step_interpolator<SampleData, U>::interpolate(const SampleData &a,
    const SampleData &b, float t) -> SampleData
{
    return t < 1.0f ? a : b;
}
