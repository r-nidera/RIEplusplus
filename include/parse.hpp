/*
 * RIE++, the compiler and tools for a general-purpose byte-compiled programming language
 * Copyright (C) 2026 Ronald Nidera
 *
 * RIE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RIE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <cstddef>


namespace parse {

class Parser {};


enum TokenType {
    Identifier = 0,
    Plus,
    Star,
    Minus,
    Slash,
    Dot,

    Left_Paren = 50,
    Right_Paren,
    Colon,
    Double_Quote,
    Left_Brace,
    Right_Brace,
    Left_Bracket,
    Right_Bracket,
    Equal,
    Comma,

    Integer_Literal = 100,
    String_Literal,
    Float_Literal,

    Operator_Equal = 110,
    Operator_Increment,
    Operator_Addition_Assignment,


    Keyword_Let = 200,

    Keyword_Void = 250,
    Keyword_Char,
    Keyword_Short,
    Keyword_Int,
    Keyword_Float,
    Keyword_Long,
    Keyword_Double,

    Keyword_Unsigned,
    Keyword_Val,

    Unknown = 400,
    Malformed_Float_Literal,
    Unterminated_String_Literal,
    Eof,
};

struct Token {
    const char *s;
    size_t len;
    TokenType type;
};
class Lexer {

    const char *start_;
    const char *current_;
    std::size_t line_;

  public:
    [[gnu::nonnull]] Lexer(const char *s) : current_(s), line_(0) {};

    char is_eof(void) const { return *current_ == '\0'; }
    char peek(void) const { return *current_; }
    char advance(void) { return *current_++; }

    void skim_whitespace(void);
    std::size_t consume_identifier(void);
    std::size_t consume_digit(void);
    std::size_t consume_string(void);

    size_t current_line() const { return line_; }
    Token next_token(void);
};

} // namespace parse
