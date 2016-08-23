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
    \file core/optional.h
    \brief Contains a class, which can be used for optional parameters.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

/*!
    A class, which simulates an optional variable (optionally set).

    \tparam Type The type to store.
 */
template<typename Type>
class optional
{
public:
    //! The element type.
    typedef Type value_type;
    //! The pointer-to-element type.
    typedef Type *pointer;
    //! The pointer-to-const-element type.
    typedef const Type *const_pointer;
    //! The reference-to-element type.
    typedef Type &reference;
    //! The const-reference-to-element type.
    typedef const Type &const_reference;
public:
    //! Default constructor.
    optional() noexcept;
    /*!
        Copy constructor.

        \param other The object to copy.
     */
    optional(Type &other);
    /*!
        Copy constructor.

        \param other The object to copy.
     */
    optional(const Type &other);
    /*!
        Move constructor.

        \param other The object to move.
     */
    optional(Type &&other);
public:
    /*!
        Copy-assignment operator.

        \param other The object to copy.

        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    Type &operator=(Type &other);
    /*!
        Copy-assignment operator.

        \param other The object to copy.

        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    Type &operator=(const Type &other);
    /*!
        Move-assignment operator.

        \param other The object to move.
        
        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    Type &operator=(Type &&other);
    /*!
        Equal-comparison operator.

        \param other The object to move.
        
        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    bool operator==(Type &other) const;
    /*!
        Greater-than-comparison operator.

        \param other The object to move.
        
        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    bool operator>(const Type &other) const;
    /*!
        Less-than-comparison operator.

        \param other The object to move.
        
        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    bool operator<(const Type &other) const;
    /*!
        Greater-equal-than-comparison operator.

        \param other The object to move.
        
        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    bool operator>=(const Type &other) const;
    /*!
        Less-equal-than-comparison operator.

        \param other The object to move.
        
        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    bool operator<=(const Type &other) const;
    /*!
        Cast operator.

        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    operator const Type &() const noexcept;
    /*!
        Cast operator.

        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    operator Type &() const noexcept;
    /*!
        Implicit cast operator.

        \return If the optional has been initialized, the return value is true.
        Otherwise, the return value is false.
     */
    operator bool() const noexcept;
    /*!
        Address-of operator.

        \return The return value of this operator depends on the operator implementation of the
        class denoted by <i>Type</i>.
     */
    Type *operator &() noexcept;
public:
    /*!
        Returns whether the optional has been initialized.

        \return If the optional has been initialized, the return value is true.
        Otherwise, the return value is false.
     */
    bool is_initialized() const noexcept;
    //! Resets the optional to an uninitialized state.
    void reset() noexcept;
private:
    //! An instance of the optional variable.
    Type m_instance;
    //! A state-flag for the optional variable.
    bool m_isInitialized;
};


#include "optional_impl.h"

GO_END_NAMESPACE
