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
    \file core/interpolator.h
    \brief Contains classes for interpolating in different spaces.

    \author Gokhan Ozdogan
    \version 1.0
 */


#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

//! Type for interpolators that support tangent data.
template<typename SampleData, typename TangentData>
struct sample_tangent_interpolator
{
    //! The type of the interpolator function.
    typedef std::function<SampleData(const SampleData &, const SampleData &, float, const TangentData &, const TangentData &)> function_type;
};


//! Type for plain sample interpolators.
template<typename SampleData>
struct sample_interpolator
{
    //! The type of the interpolator function.
    typedef std::function<SampleData(const SampleData &, const SampleData &, float)> function_type;
    /*!
        Converts a sample interpolator to a sample tangent interpolator.

        \param f The interpolator function to convert.

        \return The new interpolator.
     */
    template<typename TangentData>
    static typename sample_tangent_interpolator<SampleData, TangentData>::function_type to_tangent_interpolator(const function_type &f);
};


/*!
    A structure, which is the base class for all interpolators.

    \tparam SampleData The sample data type.
    \tparam TangenData The tangent data type.
 */
template<typename SampleData, typename TangentData = void>
class interpolator
{
public:
    typedef SampleData sample_data_type;
    //! The type of the tangent data.
    typedef TangentData tangent_data_type;
private:
    //! Constructor.
    interpolator() = delete;
};


/*!
    A structure, which represents an interpolator for interpolating vector data.

    \tparam SampleData The sample data type.
    \tparam TangenData The tangent data type.
 */
template<typename SampleData, typename TangentData = void>
struct vector3_interpolator : public vector3_interpolator<SampleData>
{
    /*!
        Interpolates between two points in three-dimensional space using the specified interpolation type.

        \param a The starting point.
        \param b The target point.
        \param ta Tangent data for a.
        \param tb Tangent data for b.
        \param t The normalized interpolation offset.

        \return The interpolated sample.
     */
    static SampleData interpolate_hermite(const SampleData &a, const SampleData &b, const TangentData &ta, const TangentData &tb, float t);
};


/*!
    A structure, which represents an interpolator for interpolating three-dimensional data.

    \tparam SampleData The sample data type.
 */
template<typename SampleData>
struct vector3_interpolator<SampleData, void> : public interpolator<SampleData>
{
    /*!
        Interpolates between two points in three-dimensional space using the specified interpolation type.

        \param a The starting point.
        \param b The target point.
        \param t The normalized interpolation offset.

        \return The interpolated sample.
     */
    static SampleData interpolate_linear(const SampleData &a, const SampleData &b, float t);
};


/*!
    A structure, which represents an interpolator for interpolating RGBA color data.

    \tparam SampleData The sample data type.
 */
template<typename SampleData, typename TangentData = void>
struct rgba_interpolator : public interpolator<SampleData, void>
{
    /*!
        Interpolates between two samples using the specified interpolation type.

        \param a The starting point.
        \param b The target point.
        \param t The normalized interpolation offset.
        \param ta Tangent data for a.
        \param tb Tangent data for b.

        \return The interpolated sample.
     */
    static SampleData cubic_hermite(const SampleData &a, const SampleData &b, float t, const TangentData &ta, const TangentData &tb);
};


/*!
    A structure, which represents an interpolator for interpolating RGBA color data.

    \tparam SampleData The sample data type.
 */
template<typename SampleData>
struct rgba_interpolator<SampleData, void> : public interpolator<SampleData, void>
{
    /*!
        Interpolates between two samples using the specified interpolation type.

        \param a The starting point.
        \param b The target point.
        \param t The normalized interpolation offset.

        \return The interpolated sample.
     */
    static SampleData lerp(const SampleData &a, const SampleData &b, float t);
};


/*!
    A structure, which represents an interpolator that does not perform any blending.

    \tparam SampleData The sample data type.
 */
template<typename SampleData, typename = void>
struct step_interpolator : interpolator<SampleData, void>
{
    /*!
        Interpolates between two samples using the specified interpolation type.

        \param a The starting point.
        \param b The target point.
        \param t The normalized interpolation offset.

        \return The interpolated sample.
     */
    static SampleData interpolate(const SampleData &a, const SampleData &b, float t);
};


#include "interpolator_impl.h"

GO_END_NAMESPACE
