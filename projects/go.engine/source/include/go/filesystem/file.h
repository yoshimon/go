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
    \file filesystem/file.h
    \brief Contains classes for accessing files.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/path_utility.h"


GO_BEGIN_NAMESPACE

//! An abstract class, which provides a simple file interface.
class GO_API file
{
public:
    //! Enumeration of possible return values for file operations.
    enum class open_result : uint32_t
    {
        //! The operation was successfully completed.
        okay,
        //! The file could not be opened because the operating system returned an error.
        open_failed,
        //! The file could not be created because the operating system returned an error.
        create_failed
    };
    //! Enumeration of possible modes to open a file with.
    enum class open_mode : uint32_t
    {
        //! The file should be opened with read-only access.
        read,
        //! The file should be opened with read/write access.
        read_write,
        /*!
            A new file should be created and opened with read/write access. Any existing file with
            the same name will be overwritten.
         */
        create
    };
    /*!
        Enumeration of possible modes which determine the interpretation of file-pointer distance
        parameters.
     */
    enum class positioning_mode : uint32_t
    {
        //! The new position is relative to the beginning of the file.
        begin,
        //! The new position is relative to the current position.
        current,
        //! The new position is relative to the end of the file.
        end
    };
    //! Enumeration of possible return values for file I/O operations.
    enum class io_result : uint32_t
    {
        //! The operation was successfully completed.
        okay,
        //! No file has been opened for the File object.
        not_open,
        //! The operation failed because the operating system returned an error.
        io_error
    };
public:
    //! Destructor.
    virtual ~file();
public:
    /*!
        Opens a file.

        \param name The name of the file. If a path name is specified, it is interpreted relative
        to the application. Directory names preceding a file name or another directory name should
        be followed by a single forward slash character (/).
        \param mode Specifies how the file should be opened.
        \param discardContent If set to true, any previous content in the file will be discarded.

        \return See \link go::file::open_result \endlink for possible return values.
     */
    open_result open(const file_path &name, open_mode mode = open_mode::read,
        bool discardContent = false);
    /*!
        Closes the file.

        If no file was previously opened, then this function has no effect.
     */
    void close();
    /*!
        Returns the read position.

        \return The current absolute position within the file, in bytes, at which the next read or
        write operation will take place. When a file is initially opened, its read/write position
        is 0.
     */
    uint64_t readposition() noexcept;
    /*!
        Sets the read position.

        \param distance The distance, in bytes, at which the next read operation will take
        place. The interpretation of this position depends on the value of the mode parameter.
        \param mode Specifies how the position parameter is interpreted using one of the file
        positioning modes.

        \return The new absolute read position.
     */
    uint64_t move_read(int64_t distance, positioning_mode mode = positioning_mode::begin);
    /*!
        Returns the write position.

        \return The current absolute position within the file, in bytes, at which the next read or
        write operation will take place. When a file is initially opened, its read/write position
        is 0.
     */
    uint64_t writeposition() noexcept;
    /*!
        Sets the write position.

        \param distance The distance, in bytes, at which the next write operation will take
        place. The interpretation of this position depends on the value of the mode parameter.
        \param mode Specifies how the position parameter is interpreted using one of the file
        positioning modes.
        
        \return The new absolute write position.
     */
    uint64_t move_write(int64_t distance, positioning_mode mode = positioning_mode::begin);
    /*!
        Returns the size of the file.

        \return Size of the file in bytes.
     */
    uint64_t size() noexcept;
    //! Flushes any pending operations.
    void flush();
    /*!
        Returns whether the file has been opened.

        \return If the file has been opened, the return value is true. Otherwise, the return value
        is false.
     */
    bool is_open() const noexcept;
    /*!
        Returns a reference to the internal file-stream.

        \return The internal file-stream.
     */
    std::fstream &internal_stream() noexcept;
protected:
    //! Default constructor.
    file();
    /*!
        Constructs the object and opens a file.

        \param name The name of the file. If a path name is specified, it is interpreted relative
        to the application. Directory names preceding a file name or another directory name should
        be followed by a single forward slash character (/).
        \param mode Specifies how the file should be opened.
        \param discardContent If set to true, any previous content in the file will be
        discarded.
     */
    explicit file(const file_path &name, open_mode mode = open_mode::read,
        bool discardContent = false);
protected:
    //! Indicates whether the file should be treated as a binary file.
    bool m_isBinary;
    //! The file reference.
    std::fstream m_stream;
};


//! A class for accessing binary files.
class GO_API binary_file : public file
{
public:
    /*!
        Reads the entire contents of a binary file into memory.
        If the read operation fails, the function throws an exception.

        \param filePath The file path.

        \return A byte-vector containing the file contents.
     */
    static byte_vector read(const file_path &filePath);
    /*!
        Reads the entire contents of a binary file into memory.
        If the read operation fails, the function throws an exception.

        \param filePath The file path.
        \param[out] out The output string.
     */
    static io_result read_string(const file_path &filePath, std::string &out);
public:
    //! Default constructor.
    binary_file();
    /*!
        Constructs the object and opens a file.

        \param name The name of the file. If a path name is specified, it is interpreted relative
        to the application. Directory names preceding a file name or another directory name should
        be followed by a single forward slash character (/).
        \param mode Specifies how the file should be opened.
        \param discardContent If set to true, any previous content in the file will be discarded.
     */
    explicit binary_file(const file_path &name, open_mode mode = open_mode::read,
        bool discardContent = false);
public:
    /*!
        Reads a block of data.
        This function attempts to perform a synchronous read operation for a file object.

        \param buffer A pointer to the location that will receive the data read from the file.
        \param size The number of bytes to read from the file.

        \return See \link go::file::io_result \endlink for possible return values.
     */
    io_result read(uint8_t *buffer, size_t size);
    //! \overload
    io_result read(std::string &out, size_t size = (size_t)-1, size_t additionalCapacity = 0);
    //! \overload
    byte_vector read(size_t size = (size_t)-1, size_t additionalCapacity = 0);
    //! \overload
    byte_vector &read(byte_vector &out, size_t size = (size_t)-1, size_t additionalCapacity = 0);
    //! \overload
    byte_vector &read_at(byte_vector &out, int64_t offset, size_t size, size_t additionalCapacity = 0, positioning_mode mode = positioning_mode::begin);
    //! \overload
    byte_vector read_at(int64_t offset, size_t size, size_t additionalCapacity = 0, positioning_mode mode = positioning_mode::begin);
    /*!
        Writes a block data.
        This function attempts to perform a synchronous write operation for a file object.

        \param buffer A pointer to the location from which data will be written to the file.
        \param size The number of bytes to write to the file.

        \return See \link go::file::io_result \endlink for possible return values.
     */
    io_result write(const uint8_t *buffer, size_t size);
};


//! A class for accessing text files.
class GO_API text_file : public file
{
public:
    /*!
        Reads the entire contents of a text file into memory.
        If the read operation fails, the function throws an exception.

        \param filePath The filePath.

        \return The text.
     */
    static std::string read(const file_path &filePath);
public:
    //! Default constructor.
    text_file();
    /*!
        Constructs the object and opens a file.

        \param name The name of the file. If a path name is specified, it is interpreted relative
        to the application. Directory names preceding a file name or another directory name should
        be followed by a single forward slash character (/).
        \param mode Specifies how the file should be opened.
        \param discardContent If set to true, any previous content in the file will be discarded.
     */
    explicit text_file(const file_path &name, open_mode mode = open_mode::read,
        bool discardContent = false);
public:
    /*!
        Writes a typed value to the file.

        \param val The value to write.
     */
    text_file &operator<<(bool val);
    //! \overload
    text_file &operator<<(int val);
    //! \overload
    text_file &operator<<(short val);
    //! \overload
    text_file &operator<<(unsigned short val);
    //! \overload
    text_file &operator<<(unsigned int val);
    //! \overload
    text_file &operator<<(long val);
    //! \overload
    text_file &operator<<(unsigned long val);
    //! \overload
    text_file &operator<<(long long val);
    //! \overload
    text_file &operator<<(unsigned long long val);
    //! \overload
    text_file &operator<<(float val);
    //! \overload
    text_file &operator<<(double val);
    //! \overload
    text_file &operator<<(long double val);
    //! \overload
    text_file &operator<<(void *val);
    //! \overload
    text_file &operator<<(std::streambuf *sb);
    //! \overload
    text_file &operator<<(std::ostream &(*pf)(std::ostream&));
    //! \overload
    text_file &operator<<(std::ios &(*pf)(std::ios&));
    //! \overload
    text_file &operator<<(std::ios_base &(*pf)(std::ios_base&));
public:
    /*!
        Writes a string.

        \param str The string to write.
        \param strLen Length of the string in characters.

        \return See \link go::file::io_result \endlink for possible return values.
     */
    io_result write(const std::string &str, size_t strLen = 0);
    /*!
        Reads the next line.

        \param out The string buffer to write the line into.
        \param removeCarriageReturn If set to true, a trailing carriage-return will be removed.

        \return See \link go::file::io_result \endlink for possible return values.
     */
    io_result read_line(std::string &out, bool removeCarriageReturn = true);
};

GO_END_NAMESPACE


/*!
    Writes an typed value to a text-file.

    \param tf The text-file to write to.
    \param c The value to write.
 */
GO_API GO text_file &operator<<(GO text_file &tf, char c);
//! \overload
GO_API GO text_file &operator<<(GO text_file &tf, signed char c);
//! \overload
GO_API GO text_file &operator<<(GO text_file &tf, unsigned char c);
//! \overload
GO_API GO text_file &operator<<(GO text_file &tf, const char *s);
//! \overload
GO_API GO text_file &operator<<(GO text_file &tf, const signed char *s);
//! \overload
GO_API GO text_file &operator<<(GO text_file &tf, const unsigned char *s);
//! \overload
template<class Char, class Traits, class T>
GO text_file &operator<<(GO text_file &tf, const T &val);


#include "file_impl.h"
