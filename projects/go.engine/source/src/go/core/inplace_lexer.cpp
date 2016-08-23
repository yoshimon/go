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


#include "go/core/inplace_lexer.h"


#include <locale>


// ================================================================================ //
// go::inplace_lexer::syntax_error member functions
// ================================================================================ //

go::inplace_lexer::syntax_error::syntax_error(const std::string &whatArg)
    : base_class_type(whatArg)
{
}


go::inplace_lexer::syntax_error::syntax_error(const char *whatArg)
    : base_class_type(whatArg)
{
}


// ================================================================================ //
// go::inplace_lexer::value_range_error member functions
// ================================================================================ //

go::inplace_lexer::value_range_error::value_range_error(const std::string &whatArg)
    : base_class_type(whatArg)
{
}


go::inplace_lexer::value_range_error::value_range_error(const char *whatArg)
    : base_class_type(whatArg)
{
}


// ================================================================================ //
// go::inplace_lexer member functions
// ================================================================================ //

go::inplace_lexer::inplace_lexer(std::string text, const options_t &options)
    : m_text(std::move(text)), m_options(options)
{
    GO_ASSERT(m_options.comment.singleLineComment.size() < 3);

    // Comment characters must be token separators
    for(auto c : m_options.comment.singleLineComment)
    {
        if(m_options.separators.find(c) == std::string::npos)
        {
            GO_THROW(std::exception, "Comment symbols must be part of the separator set.");
        }
    }

    m_token.info.endPos = -1;
    reset();
    m_options.separators.append(" \t\r\n");
}


go::inplace_lexer::~inplace_lexer()
{
    reset();
}


bool go::inplace_lexer::done()
{
    return m_token.info.type == token_info::type_t::eof;
}


void go::inplace_lexer::reset()
{
    move(0);
}


auto go::inplace_lexer::next_token() -> const token_t &
{
    consume();
    peek();
    return m_token;
}


auto go::inplace_lexer::next_identifier(std::string &s) -> inplace_lexer *
{
    if(next_token().info.type == token_info::type_t::identifier)
    {
        s.assign(m_token.text);
        consume();
        return this;
    }
    
    return nullptr;
}


auto go::inplace_lexer::next_list(char listBegin, std::vector<token_t> &l,
    char listEnd, const std::string &format, char delim) -> inplace_lexer *
{
    return this;
}


auto go::inplace_lexer::next_list(char listBegin, std::vector<std::string> &l,
    char listEnd, char delim) -> inplace_lexer *
{
    if(peek_match(listBegin))
    {
        next_list(l, listEnd, delim);
    }
    return this;
}


auto go::inplace_lexer::next_list(std::vector<std::string> &l, char listEnd, char delim)
    -> inplace_lexer *
{
    do
    {
        if(peek(listEnd))
        {
            break;
        }
        if(m_token.info.type != token_info::type_t::eof)
        {
            l.push_back(m_token.text);
        }
        consume();
        if(!peek_match(delim))
        {
            break;
        }
    }
    while(m_token.info.type != token_info::type_t::eof);
    match(listEnd);
    return this;
}


auto go::inplace_lexer::next_list(std::vector<token_t> &l, char listEnd,
    const std::string &format, char delim) -> inplace_lexer *
{
    return this;
}


auto go::inplace_lexer::peek() -> const token_t &
{
    while(true)
    {
        do_peek(m_options.separators, false);
        if(m_token.info.type == token_info::type_t::symbol)
        {
            consume();
        }
        else
        {
            // Comments
            if(m_token.info.type != token_info::type_t::eof)
            {
                // Single-line
                if(!m_options.comment.singleLineComment.empty() && m_token.text[0] == m_options.comment.singleLineComment[0])
                {
                    if(m_options.comment.singleLineComment.size() == 1 ||
                      (m_options.comment.singleLineComment.size() > 1 && m_oldChar == m_options.comment.singleLineComment[1]))
                    {
                        // Skip line
                        consume();
                        to_end_of_line();
                        consume();
                        peek();
                        continue;
                    }
                }
            }

            break;
        }
    }
    return m_token;
}


auto go::inplace_lexer::do_peek(const std::string &separators, bool findAll) -> const token_t &
{
    if(m_token.info.type != token_info::type_t::eof && m_token.info.endPos == -1)
    {
        m_token.text = &m_text[m_token.info.startPos];

        size_t i;
        if(findAll)
        {
            i = m_text.find(separators, static_cast<size_t>(m_token.info.startPos));
        }
        else
        {
            i = m_text.find_first_of(separators, static_cast<size_t>(m_token.info.startPos));
        }

        if(i != std::string::npos)
        {
            // Treat strings as one token
            if(m_text[i] == m_options.stringDelimiter)
            {
                auto j = m_text.find_first_of(m_options.stringDelimiter, i + 1);
                if(j == std::string::npos)
                {
                    GO_THROW(syntax_error, "Missing string closure.");
                }
                else
                {
                    i = j + 1;
                }
            }
            
            if(i == static_cast<size_t>(m_token.info.startPos))
            {
                ++i;
            }

            m_oldChar = m_text[i];
            m_text[i] = '\0';
            m_token.info.endPos = (int32_t)i;
            update_token_type();
        }
        else
        {
            if((size_t)m_token.info.startPos < m_text.length())
            {
                m_token.info.endPos = (int32_t)m_text.length();
                update_token_type();
            }
            else
            {
                m_token.info.type = token_info::type_t::eof;
            }
        }
    }

    return m_token;
}


void go::inplace_lexer::update_token_type()
{
    // Assume its a string
    m_token.info.type = token_info::type_t::string;

    // Numbers
    char *endPtr;
    auto i = std::strtol(m_token.text, &endPtr, 0);
    if(*endPtr != '\0')
    {
        if(i == LONG_MAX || i == LONG_MIN)
        {
            GO_THROW(value_range_error, "Value is out-of-range.");
        }
        else if((i != 0) || (i == 0 && strcmp(m_token.text, "0") == 0))
        {
            m_token.info.type = token_info::type_t::number;
            return;
        }
    }

    // string special cases
    // Symbol
    if(m_token.text[0] == ' ' || m_token.text[0] < 32 || m_token.text[0] > 126)
    {
        m_token.info.type = token_info::type_t::symbol;
        return;
    }
    else
    {
        // Identifier
        std::locale loc;
        if(std::isalpha(m_token.text[0], loc))
        {
            bool isIdentifier = true;
            for(auto j = 0; j < (m_token.info.endPos - m_token.info.startPos); ++j)
            {
                if(!std::isalnum(m_token.text[j], loc))
                {
                    isIdentifier = false;
                    break;
                }
            }

            if(isIdentifier)
            {
                m_token.info.type = token_info::type_t::identifier;
            }
        }
    }
}


auto go::inplace_lexer::peek(const char *s) -> inplace_lexer *
{
    auto &t = peek();
    return ((t.info.type != token_info::type_t::eof) && (strcmp(t.text, s) == 0)) ? this : nullptr;
}


auto go::inplace_lexer::peek(char c) -> inplace_lexer *
{
    const char s[] = { c, '\0' };
    return peek(s);
}


auto go::inplace_lexer::peek_match(const char *s) -> inplace_lexer *
{
    if(peek(s))
    {
        consume();
        return this;
    }
    else
    {
        return nullptr;
    }
}


auto go::inplace_lexer::peek_match(char c) -> inplace_lexer *
{
    const char s[] = { c, '\0' };
    return peek_match(s);
}

    
auto go::inplace_lexer::match(const char *s) -> inplace_lexer *
{
    if(!peek_match(s))
    {
        GO_THROW(syntax_error, "Syntax error.");
    }
    return this;
}


void go::inplace_lexer::consume()
{
    // Put old character back in place
    if(m_token.info.endPos > -1)
    {
        m_text[m_token.info.endPos] = m_oldChar;
        m_token.info.startPos = m_token.info.endPos;
        m_token.info.endPos = -1;
    }
}


int32_t go::inplace_lexer::position() const
{
    return m_token.info.startPos;
}


auto go::inplace_lexer::match(char c) -> inplace_lexer *
{
    const char s[] = { c, '\0' };
    return match(s);
}


void go::inplace_lexer::move(int32_t position)
{
    consume();
    m_token.info.startPos = position;
    m_token.info.endPos = -1;
    m_token.info.type = token_info::type_t::string; // Everything but eof
}


void go::inplace_lexer::replace(int32_t beginPos, int32_t endPos, const std::string &text)
{
    move(beginPos);
    m_text.replace(static_cast<size_t>(beginPos), static_cast<size_t>(endPos), text);
}


auto go::inplace_lexer::to_end_of_line() -> const token_t &
{
    return do_peek("\r\n", true);
}
