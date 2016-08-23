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
// Helper classes
// ================================================================================ //

template<typename Type, typename Container>
struct byte_step_view_helper
{
    Type operator()(const Container &container, size_t &readPos)
    {
        auto delta = sizeof(Type);
        auto oldReadPos = readPos;
        if(oldReadPos + delta > container.size())
        {
            return Type();
        }

        readPos += delta;

        Type t;
        memcpy(&t, container.data() + oldReadPos, delta);
        return t;
    }
};


template<typename Container>
struct byte_step_view_helper<std::string, Container>
{
    std::string operator()(const Container &container, size_t &readPos)
    {
        byte_step_view_helper<uint32_t, Container> h;
        auto strLen = h(container, readPos);
        if(strLen <= 0)
        {
            return "";
        }
        
        auto oldReadPos = readPos;
        if(oldReadPos + strLen > container.size())
        {
            return "";
        }

        readPos += strLen;

        return std::string(reinterpret_cast<char *>(container[oldReadPos]), strLen);
    }
};


// ================================================================================ //
// byte_step_view member functions
// ================================================================================ //

template<typename Container>
byte_step_view<Container>::byte_step_view(const Container &container)
    : m_container(container), m_readPos(0)
{
}


template<typename Container>
bool byte_step_view<Container>::done() const noexcept
{
    GO_ASSERT(m_readPos <= m_container.size());
    return m_readPos >= m_container.size();
}


template<typename Container>
void byte_step_view<Container>::reset() noexcept
{
    m_readPos = 0;
}


template<typename Container>
template<typename Type>
Type byte_step_view<Container>::read()
{
    if(done())
    {
        return Type();
    }

    byte_step_view_helper<Type, Container> bsvh;
    return bsvh(m_container, m_readPos);
}
