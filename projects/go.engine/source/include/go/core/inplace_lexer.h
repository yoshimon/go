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
    \file core/inplace_lexer.h
    \brief Contains a lexer class for quickly parsing simple grammar.

    \author Gokhan Ozdogan
    \version 1.0
 */

#pragma once


#include "go/core/common.h"
#include "go/core/exception.h"


GO_BEGIN_NAMESPACE

//! A class, which tokenizes a given string.
class GO_API inplace_lexer
{
public:
    //! A exception class, which is thrown when a syntax error is detected.
    class GO_API syntax_error : public GO exception
    {
    public:
        //! The base class.
        typedef GO exception base_class_type;
    public:
        /*!
            Constructs the object and initializes it.

            \param whatArg Explanatory string.
            \param errorCode The HRESULT error-code.
         */
        explicit syntax_error(const std::string &whatArg);
        //! \overload
        explicit syntax_error(const char *whatArg);
    };
    //! A exception class, which is thrown when a syntax error is detected.
    class GO_API value_range_error : public GO exception
    {
    public:
        //! The base class.
        typedef GO exception base_class_type;
    public:
        /*!
            Constructs the object and initializes it.

            \param whatArg Explanatory string.
         */
        explicit value_range_error(const std::string &whatArg);
        //! \overload
        explicit value_range_error(const char *whatArg);
    };
    //! A structure, which holds the lexer parameters.
    struct options_t
    {
        /*
            A structure, which holds the lexer-comment parameters.
         */
        struct comment_t
        {
            /*!
                Single-line comment characters.

                Maximum of two characters.
             */
            std::string singleLineComment;
        };
        /*
            A list of token separators.
            
            The first character in this list is used as the string / text character.
            
            White-spaces, tabs and new-line are separators by default.
         */
        std::string separators;
        /*
            Comment options.
         */
        comment_t comment;
        /*!
            String delimiter.

            Strings will be lexed as one token.
         */
        char stringDelimiter;
    };
    //! A structure, which describes a token.
    struct token_info
    {
        //! Enumeration of token types.
        enum class type_t
        {
            /*
                The token contains a number.
             */
            number,
            /*
                The token contains a text.
             */
            string,
            //! The tokens contains an identifier.
            identifier,
            /*
                The token contains an invisible symbol (e.g. line breaks or tabs).
             */
            symbol,
            /*
                End-of-file token.
             */
            eof
        };
        /*
            The type of the token.
         */
        type_t type;
        //! End position of the token.
        int32_t startPos;
        //! Starting position of the token.
        int32_t endPos;
    };
    //! A structure, which represents a token returned by the lexer.
    struct token_t
    {
        /*
            The token information.
        */
        token_info info;
        /*
            The value or text behind the token.
        */
        const char *text;
    };
public:
    /*!
        Constructor.

        \param text The text to lex.
        \param options The lexer options.
     */
    inplace_lexer(std::string text, const options_t &options);
    //! Destructor.
    ~inplace_lexer();
public:
    /*
        Returns, whether there are any more tokens to lex.

        \return If there are no more tokens, the return value is false. Otherwise,
        the return value is true.
     */
    bool done();
    //! Resets the lexer to the beginning of the text.
    void reset();
    /*!
        Returns the next token.

        \return The next token.
     */
    const token_t &next_token();
    /*
        Returns the next identifier, if one exists.

        \param s The string to write the identifier to.

        \return Pointer to the lexer object.
     */
    inplace_lexer *next_identifier(std::string &s);
    /*
        Returns the next list, if one exists.

        Valid tokens are: numbers, strings, symbols.

        \param l The output list.
        \param format The list format.

        \return Pointer to the lexer object.
     */
    inplace_lexer *next_list(char listBegin, std::vector<token_t> &l, char listEnd, const std::string &format = "", char delim = ',');
    //! \overload
    inplace_lexer *next_list(char listBegin, std::vector<std::string> &l, char listEnd, char delim = ',');
    //! \overload
    inplace_lexer *next_list(std::vector<std::string> &l, char listEnd, char delim = ',');
    /*
        \overload
     */
    inplace_lexer *next_list(std::vector<token_t> &l, char listEnd, const std::string &format = "", char delim = ',');
    /*
        Peeks the next token.
    */
    const token_t &peek();
    /*
        \overload
    */
    inplace_lexer *peek(const char *s);
    /*
        \overload
    */
    inplace_lexer *peek(char c);
    /*
        Peeks for a given text.

        \param s The string to peek for.
    */
    inplace_lexer *peek_match(const char *s);
    //! \overload
    inplace_lexer *peek_match(char s);
    /*!
        Matches the next token to a given string.

        \param s The string to match.

        \return Pointer to the lexer object.
     */
    inplace_lexer *match(const char *s);
    /*
        \overload
     */
    inplace_lexer *match(char c);
    /*!
        Moves the lexer to a new position.

        \param position The new position.
     */
    void move(int32_t position);
    /*!
        Replaces the text within a given range.

        \param beginPos The position to replace at.
        \param endPos The position to replace to.
        \param text The text to replace with.
     */
    void replace(int32_t beginPos, int32_t endPos, const std::string &text);
    /*!
        Returns a pointer that contains the string up to the end of the current line.

        \return The string up to the end of the current line.
     */
    const token_t &to_end_of_line();
    /*!
        Returns the current character position of the lexer.

        \return Character position.
     */
    int32_t position() const;
private:
    //! Consumes the current token, that was previously peeked.
    void consume();
    /*!
        Peeks for a given set of separators.

        \param seperator The seperator set.
        \param findAll Indicates, whether to treat the separator string as one substring.
    */
    const token_t &do_peek(const std::string &separators, bool findAll);
    //! Updates the token type by examining the token value.
    void update_token_type();
private:
    //! The text to tokenize.
    std::string m_text;
    //! The lexer parameters.
    options_t m_options;
    //! The current token.
    token_t m_token;
    //! Temporary storage.
    char m_oldChar;
};

GO_END_NAMESPACE
