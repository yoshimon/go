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
// allocator_base<Element> member functions
// ================================================================================ //

template<typename Element>
inline auto allocator_base<Element>::allocate_unique() -> std::unique_ptr<Element, deleter_type>
{
    return std::unique_ptr<Element, deleter_type>(allocate(), [this](void *p){ this->release((Element *)p); });
}


// ================================================================================ //
// allocator_base<Element->void> member functions
// ================================================================================ //

inline auto allocator_base<void>::allocate_unique(size_t size) -> std::unique_ptr<void, deleter_type>
{
    return std::unique_ptr<void, deleter_type>(allocate(size), [this](void *p){ this->release(p); });
}


// ================================================================================ //
// aligned_allocator_base<Element> member functions
// ================================================================================ //

template<typename Element>
inline auto aligned_allocator_base<Element>::allocate_aligned_unique(size_t size, size_t alignment)
    -> std::unique_ptr<Element, deleter_type>
{
    return std::unique_ptr<Element, deleter_type>(
        allocate_aligned(size, alignment),
        [this](void *p){ this->free_aligned((Element *)p); }
    );
}
