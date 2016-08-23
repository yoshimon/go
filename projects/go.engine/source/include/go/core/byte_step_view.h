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
    \file core/byte_step_view.h
    \brief Contains a helper class for stepping through byte vectors.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

/*!
    A class, which allows byte-vectors to be traversed with user-defined types.

    \tparam Container The container class to create a step-view of.
 */
template<typename Container>
class byte_step_view
{
public:
    //! The element type.
    typedef Container value_type;
    //! The pointer-to-element type.
    typedef Container *pointer;
    //! The pointer-to-const-element type.
    typedef const Container *const_pointer;
    //! The reference-to-element type.
    typedef Container &reference;
    //! The const-reference-to-element type.
    typedef const Container &const_reference;
public:
    /*!
        Constructor.

        \param container The container to step through.
     */
    explicit byte_step_view(const Container &container);
public:
    //! Returns whether the view has finished stepping through the container.
    bool done() const noexcept;
    //! Resets the view to its initial state.
    void reset() noexcept;
    /*!
        Reads a piece of byte data and interprets it.

        \param Type The type to read.
     */
    template<typename Type>
    Type read();
private:
    //! The container to step through.
    const Container &m_container;
    //! The current read position.
    size_t m_readPos;
};


#include "go/core/byte_step_view_impl.h"

GO_END_NAMESPACE
