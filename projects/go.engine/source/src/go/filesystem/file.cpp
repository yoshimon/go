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


#include "go/stdafx.h"


#include "go/filesystem/file.h"


#include "go/core/exception.h"


// ================================================================================ //
// go::file member functions
// ================================================================================ //

go::file::file()
    : m_isBinary(true)
{
}


go::file::file(const file_path &name, open_mode mode, bool discardContent)
    : m_isBinary(true)
{
    open(name, mode, discardContent);
}


go::file::~file()
{
}


go::file::open_result go::file::open(const file_path &name, file::open_mode mode, bool discardContent)
{
    close();

    std::ios_base::openmode flags = std::ios_base::in;
    switch(mode)
    {
    case open_mode::read_write:
        {
            flags |= std::ios_base::out;
            break;
        }
    case open_mode::create:
        {
            std::ofstream createStream(name.c_str());
            createStream.flush();
            createStream.close();
            flags |= std::ios_base::out;
            break;
        }
    }

    if(m_isBinary)
    {
        flags |= std::ios_base::binary;
    }

    if(discardContent)
    {
        flags |= std::ios_base::trunc;
    }

    m_stream.open(name.c_str(), flags);
    if(!m_stream.is_open())
    {
        return (mode == open_mode::create) ? open_result::create_failed : open_result::open_failed;
    }

    move_write(0, positioning_mode::begin);
    move_read(0, positioning_mode::begin);

    return open_result::okay;
}


void go::file::close()
{
    m_stream.close();
}


uint64_t go::file::readposition() noexcept
{
    return m_stream.tellg();
}


uint64_t go::file::move_read(int64_t distance, positioning_mode mode)
{
    std::ios_base::seekdir seekDir;
    switch(mode)
    {
    case positioning_mode::begin: seekDir = std::ios_base::beg; break;
    case positioning_mode::end: seekDir = std::ios_base::end; break;
    default: seekDir = std::ios_base::cur; break;
    }

    return m_stream.seekg(distance, seekDir).tellg();
}


uint64_t go::file::writeposition() noexcept
{
    return m_stream.tellp();
}


uint64_t go::file::move_write(int64_t distance, positioning_mode mode)
{
    std::ios_base::seekdir seekDir;
    switch(mode)
    {
    case positioning_mode::begin: seekDir = std::ios_base::beg; break;
    case positioning_mode::end: seekDir = std::ios_base::end; break;
    default: seekDir = std::ios_base::cur; break;
    }

    return m_stream.seekp(distance, seekDir).tellp();
}


uint64_t go::file::size() noexcept
{
    auto oldPos = m_stream.tellg();
    auto size = m_stream.seekg(0, std::ios_base::end).tellg();
    m_stream.seekg(oldPos, std::ios_base::beg);
    return size;
}


void go::file::flush()
{
    m_stream.flush();
}


bool go::file::is_open() const noexcept
{
    return m_stream.is_open();
}


std::fstream &go::file::internal_stream() noexcept
{
    return m_stream;
}


// ================================================================================ //
// go::binary_file member functions
// ================================================================================ //

go::byte_vector go::binary_file::read(const file_path &filePath)
{
    binary_file f(filePath);
    if(!f.is_open())
    {
        _tprintf("Could not open binary file '%s'.\n", filePath.c_str());
        GO_THROW(std::invalid_argument, "Could not open binary file.");
    }

    return f.read();
}


auto go::binary_file::read_string(const file_path &filePath, std::string &out) -> io_result
{
    binary_file f(filePath);
    if(!f.is_open())
    {
        _tprintf("Could not open binary file '%s'.\n", filePath.c_str());
        GO_THROW(std::invalid_argument, "Could not open binary file.");
    }

    return f.read(out);
}


go::binary_file::binary_file()
    : file()
{
}


go::binary_file::binary_file(const file_path &name, open_mode mode, bool discardContent)
    : file(name, mode, discardContent)
{
}


auto go::binary_file::read(uint8_t *buffer, size_t size) -> io_result
{
    if(!m_stream.is_open())
    {
        return io_result::not_open;
    }

    return (m_stream.read((char *)buffer, size).bad()) ? io_result::io_error : io_result::okay;
}


auto go::binary_file::read(std::string &out, size_t size, size_t additionalCapacity) -> io_result
{
    size = std::min<size_t>(size, static_cast<size_t>(this->size()));

    out.resize(size + additionalCapacity);
    if(size > 0 && out.capacity() >= size)
    {
        return read((uint8_t *)out.c_str(), size);
    }

    return io_result::okay;
}


go::byte_vector go::binary_file::read(size_t size, size_t additionalCapacity)
{
    byte_vector bytes;
    return read(bytes, size, additionalCapacity);
}


auto go::binary_file::read(byte_vector &out, size_t size, size_t additionalCapacity) -> byte_vector &
{
    size = std::min<size_t>(size, static_cast<size_t>(this->size()));

    auto oldEnd = out.size();
    out.resize(size + additionalCapacity);
    if(size > 0 && out.capacity() >= size)
    {
        read(out.data() + oldEnd, size);
    }

    return out;
}


auto go::binary_file::read_at(byte_vector &out, int64_t offset, size_t size, size_t additionalCapacity, positioning_mode mode)
    -> byte_vector &
{
    move_read(offset, mode);
    return read(out, size, additionalCapacity);
}


auto go::binary_file::read_at(int64_t offset, size_t size, size_t additionalCapacity, positioning_mode mode) -> byte_vector
{
    byte_vector out;
    return read_at(out, size, additionalCapacity);
}


go::file::io_result go::binary_file::write(const uint8_t *buffer, size_t size)
{
    if(!m_stream.is_open())
    {
        return io_result::not_open;   
    }

    return (m_stream.write((const char *)buffer, size).bad()) ? io_result::io_error : io_result::okay;
}


// ================================================================================ //
// go::text_file member functions
// ================================================================================ //

std::string go::text_file::read(const file_path &filePath)
{
    std::ifstream t(filePath);
    if(!t.is_open())
    {
        _tprintf("Could not open text file '%s'.\n", filePath.c_str());
        GO_THROW(std::invalid_argument, "Could not open text file.");
    }
    return std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}


go::text_file::text_file()
    : file()
{
    m_isBinary = false;
}


go::text_file::text_file(const file_path &name, open_mode mode /* = open_mode::read */, bool discardContent /* = false */)
    : file(name, mode, discardContent)
{
}


go::file::io_result go::text_file::write(const std::string &str, size_t strLen)
{
    if(!m_stream.is_open())
    {
        return io_result::not_open;   
    }

    return (m_stream.write(str.c_str(), (strLen == 0) ? str.length() : strLen).bad()) ? io_result::io_error : io_result::okay;
}


go::file::io_result go::text_file::read_line(std::string &out, bool removeCarriageReturn)
{
    if(!m_stream.is_open() || !m_stream.good())
    {
        return io_result::not_open;
    }

    auto rv = std::getline(m_stream, out).bad() ? io_result::io_error : io_result::okay;

    // Remove optional carriage return
    if(rv == io_result::okay && removeCarriageReturn && out.length() > 0 && *out.rbegin() == '\r')
    {
        out.erase(out.length() - 1);
    }

    return rv;
}


go::text_file &go::text_file::operator<<(bool val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(int val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(short val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(unsigned short val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(unsigned int val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(long val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(unsigned long val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(long long val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(unsigned long long val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(float val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(double val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(long double val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(void *val)
{
    m_stream << val;
    return *this;
}


go::text_file &go::text_file::operator<<(std::streambuf *sb)
{
    m_stream << sb;
    return *this;
}


go::text_file &go::text_file::operator<<(std::ostream& (*pf)(std::ostream&))
{
    m_stream << pf;
    return *this;
}


go::text_file &go::text_file::operator<<(std::ios& (*pf)(std::ios&))
{
    m_stream << pf;
    return *this;
}


go::text_file &go::text_file::operator<<(std::ios_base& (*pf)(std::ios_base&))
{
    m_stream << pf;
    return *this;
}


// ================================================================================ //
// String operators
// ================================================================================ //

go::text_file &operator<<(go::text_file &tf, char c)
{
    tf.internal_stream() << c;
    return tf;
}    


go::text_file &operator<<(go::text_file &tf, signed char c)
{
    tf.internal_stream() << c;
    return tf;
}


go::text_file &operator<<(go::text_file &tf, unsigned char c)
{
    tf.internal_stream() << c;
    return tf;
}


go::text_file &operator<<(go::text_file &tf, const char *s)
{
    tf.internal_stream() << s;
    return tf;
}


go::text_file &operator<<(go::text_file &tf, const signed char *s)
{
    tf.internal_stream() << s;
    return tf;
}


go::text_file &operator<<(go::text_file &tf, const unsigned char *s)
{
    tf.internal_stream() << s;
    return tf;
}
