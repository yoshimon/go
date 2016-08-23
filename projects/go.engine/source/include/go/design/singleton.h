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
    \file design/singleton.h
    \brief Contains classes for an implementation of the singleton pattern.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"


GO_BEGIN_NAMESPACE

/*!
    An abstract class, which implements the Singleton Pattern.
    
    \tparam Derived Derived class.
 */
template<class Derived>
class singleton
{
public:
    //! The type of the derived class.
    typedef Derived derived_type;
protected:
    /*!
        Constructor.
        
        \param[in] instance A reference to the global variable that serves as
        the pointer to the single instance of the object whose type is given by
        the TDerivedSingleton template parameter.
     */
    singleton(Derived *&instance);
    //! Destructor.
    virtual ~singleton();
private:
    //! The address of the pointer to the derived singleton instance.
    static Derived **m_instance;
};


#include "singleton_impl.h"

GO_END_NAMESPACE
