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
// short_string member functions
// ================================================================================ //

template<typename Char, size_t Length, typename Traits>
short_string<Char, Length, Traits>::short_string() noexcept
    : m_length(0)
{
    m_string[0] = 0;
    m_string[Length] = 0; // We can already stick this in, not going to change
}


template<typename Char, size_t Length, typename Traits>
short_string<Char, Length, Traits>::short_string(const short_string &rhs) noexcept
{
    *this = rhs;
}


template<typename Char, size_t Length, typename Traits>
short_string<Char, Length, Traits>::short_string(const char_type *str) noexcept
{
    *this = str;
}


template<typename Char, size_t Length, typename Traits>
short_string<Char, Length, Traits>::short_string(const std::string &str) noexcept
{
    *this = str.c_str();
}


template<typename Char, size_t Length, typename Traits>
size_t short_string<Char, Length, Traits>::capacity() const noexcept
{
    return Length;
}


template<typename Char, size_t Length, typename Traits>
size_t short_string<Char, Length, Traits>::length() const noexcept
{
    return m_length;
}


template<typename Char, size_t Length, typename Traits>
bool short_string<Char, Length, Traits>::is_empty() const noexcept
{
    return m_length == 0;
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::c_str() const noexcept -> const char_type *
{
    return &m_string[0];
}


template<typename Char, size_t Length, typename Traits>
size_t short_string<Char, Length, Traits>::hash() noexcept
{
    auto h = m_hash;
    if(h == 0)
    {
        // Lazy hashing
        h = hash_c_str(m_string);
        m_hash = h;
    }

    return h;
}


template<typename Char, size_t Length, typename Traits>
void short_string<Char, Length, Traits>::clear() noexcept
{
    m_string[0] = 0;
    m_length = 0;
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::operator=(const short_string &rhs) noexcept -> short_string &
{
    strcpy_s(m_string, rhs.m_string);
    m_hash = rhs.m_hash;
    m_length = rhs.m_length;
    return *this;
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::operator=(const char_type *rhs) noexcept -> short_string &
{
    if(!rhs)
    {
        clear();
    }
    else
    {
        strncpy_s(m_string, sizeof(m_string), rhs, Length);
        m_hash = 0;
        m_length = strnlen_s(rhs, sizeof(m_string));
    }

    return *this;
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::operator=(const std::string &rhs) noexcept -> short_string &
{
    *this = rhs.c_str();
    return *this;
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::operator+=(const short_string &rhs) noexcept -> short_string &
{
    *this += rhs.c_str();
    return *this;
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::operator+=(const char_type *rhs) noexcept -> short_string &
{
    if(rhs)
    {
        auto oldLen = m_length;
        auto rhsLen = strnlen_s(rhs, sizeof(m_string));
        auto newLen = oldLen + rhsLen;
        if(newLen < Length)
        {
            m_hash = 0;
            m_length = newLen;
            auto concatBegin = &m_string[oldLen];
            strcat_s(concatBegin, sizeof(m_string) - oldLen, rhs);
        }
    }

    return *this;
}


template<typename Char, size_t Length, typename Traits>
void short_string<Char, Length, Traits>::internal_update_length_and_hash()
{
    m_hash = 0;
    m_length = strlen(m_string);
}


template<typename Char, size_t Length, typename Traits>
bool short_string<Char, Length, Traits>::operator<(const short_string &rhs) noexcept
{
    return compare(rhs) < 0;
}


template<typename Char, size_t Length, typename Traits>
bool short_string<Char, Length, Traits>::operator>(const short_string &rhs) noexcept
{
    return compare(rhs) > 0;
}


template<typename Char, size_t Length, typename Traits>
bool short_string<Char, Length, Traits>::operator>=(const short_string &rhs) noexcept
{
    return compare(rhs) >= 0;
}


template<typename Char, size_t Length, typename Traits>
bool short_string<Char, Length, Traits>::operator<=(const short_string &rhs) noexcept
{
    return compare(rhs) <= 0;
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::back() const -> const char_type &
{
    GO_MSGASSERT(m_length > 0, "invalid string length");
    return m_string[m_length - 1];
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::back() -> char_type &
{
    GO_MSGASSERT(m_length > 0, "invalid string length");
    return m_string[m_length - 1];
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::front() const -> const char_type &
{
    GO_MSGASSERT(m_length > 0, "invalid string length");
    return m_string[0];
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::front() -> char_type &
{
    GO_MSGASSERT(m_length > 0, "invalid string length");
    return m_string[0];
}


template<typename Char, size_t Length, typename Traits>
auto short_string<Char, Length, Traits>::operator[](size_t index) const -> char_type
{
    return m_string[index];
}


template<typename Char, size_t Length, typename Traits>
short_string<Char, Length, Traits>::operator std::string() const noexcept
{
    return m_string;
}


template<typename Char, size_t Length, typename Traits>
int short_string<Char, Length, Traits>::compare(const char_type *other) const noexcept
{
    return Traits::compare(m_string, other, m_length);
}


template<typename Char, size_t Length, typename Traits>
int short_string<Char, Length, Traits>::compare(const short_string &other) const noexcept
{
    return compare(other.c_str());
}


template<typename Char, size_t Length, typename Traits>
size_t short_string<Char, Length, Traits>::find_first_of(char_type c, size_t pos) const noexcept
{
    GO_MSGASSERT(pos <= Length, "invalid string offset");
    return Traits::find(&m_string[pos], m_length - pos, c) - m_string;
}
