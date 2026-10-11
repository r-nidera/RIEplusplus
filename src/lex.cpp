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

#include <cctype>
#include <cstdio>
#include <cstring>
#include <parse.hpp>
#include <stdarg.h>

using namespace parse;
using namespace std;

struct Keyword {
    const char *s;
    unsigned short len;
    TokenType type;

    // Fast way to determine if something is NOT a kwd
    static constexpr char min_len = 3;
    static constexpr char max_len = 8;
};
static const Keyword KEYWORDS[] = {
    {      "let", 3,      Keyword_Let },

    {     "Void", 4,     Keyword_Void },
    {     "Byte", 4,     Keyword_Byte },
    {    "UByte", 5,    Keyword_UByte },
    {     "Char", 4,     Keyword_Char },
    {    "Short", 5,    Keyword_Short },
    {   "UShort", 6,   Keyword_UShort },
    {      "Int", 3,      Keyword_Int },
    {     "UInt", 4,     Keyword_UInt },
    {    "Float", 5,    Keyword_Float },
    {     "Long", 4,     Keyword_Long },
    {    "ULong", 5,    Keyword_ULong },
    {   "Double", 6,   Keyword_Double },
    { "unsigned", 8, Keyword_Unsigned },
    {      "val", 5,      Keyword_Val },

    {    nullptr, 0,          Unknown },
};
void Lexer::skim_whitespace() {
    while (peek() && isspace(static_cast<unsigned char>(peek()))) {
        if (peek() == '\n') {
            line_++;
        }
        advance();
    }
}
size_t Lexer::consume_identifier(void) {
    while (isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
        advance();
    }

    return current_ - start_;
}
static TokenType determine_ident_keyword(const char *s, size_t len) {
    if (len < Keyword::min_len || len > Keyword::max_len) {
        return Identifier;
    }
    for (int i = 0; KEYWORDS[i].s != nullptr; i++) {
        if (len == KEYWORDS[i].len) {
            if (memcmp(KEYWORDS[i].s, s, len) == 0) {
                return KEYWORDS[i].type;
            }
        }
    }
    return Identifier;
}
size_t Lexer::consume_digit(void) {
    while (isdigit(static_cast<unsigned char>(peek())) || peek() == '.') {
        advance();
    }
    return current_ - start_;
}
size_t Lexer::consume_string(void) {
    while (!is_eof() && peek() != '\"') {
        advance();
    }
    // PATCH: Consume the trailing quote that the lexer leaves out after we skimmed
    // the first dquote, which causes the lexer to interpret the characters after it
    // as a string, leading to a unterminated string literal for the rest of the file.
    //
    // Consume the trailing quote regardless if it's empty or not
    if (!is_eof() && peek() == '"') {
        advance();
        // Subtract 1 for the dquote
        return (size_t)(current_ - start_) - 1;
    }
    // BAD BAD BAD
    return 0;
}
static TokenType validate_digit(const char *s, size_t len) {
    bool has_dp = false;

    for (size_t i = 0; i < len; i++) {
        if (s[i] == '.') {
            if (has_dp) {
                return Malformed_Float_Literal;
            }
            has_dp = true;
        }
    }
    return (has_dp) ? Float_Literal : Integer_Literal;
}

Token Lexer::next_token() {
    skim_whitespace();

    if (is_eof()) {
        return (Token){ "\0", 0, TokenType::Eof };
    }

    start_ = current_;

    if (isalpha(static_cast<unsigned char>(peek()))) {
        auto len = consume_identifier();
        return (Token){ start_, len, determine_ident_keyword(start_, len) };
    } else if (isdigit(static_cast<unsigned char>(peek())) || peek() == '_') {
        auto len = consume_digit();
        return (Token){ start_, len, validate_digit(start_, len) };
    } else {
        switch (peek()) {
            case ':': {
                advance();
                return (Token){ start_, 1, Colon };
            }
            case ',': {
                advance();
                return (Token){ start_, 1, Comma };
            }
            case '(': {
                advance();
                return (Token){ start_, 1, Left_Paren };
            }
            case ')': {
                advance();
                return (Token){ start_, 1, Right_Paren };
            }
            case '{': {
                advance();
                return (Token){ start_, 1, Left_Brace };
            }
            case '}': {
                advance();
                return Token{ start_, 1, Right_Brace };
            }
            case '=': {
                advance();
                return Token{ start_, 1, Equal };
            }
            case '\"': {
                // Move the start of the string literal from the dquote
                // to the first char
                ++start_;

                advance();
                // FIX: Last time, we forgot to consume the trailing quote, leading to
                // the first string literal to leave the trailing quote which causes the lexer
                // to interpret the trailing dqote as an opening to a string
                auto len = consume_string();
                return Token{ start_, len, len == 0 ? Unterminated_String_Literal : String_Literal };
            }
            case '+': {
                advance();
                if (peek() == '=') {
                    advance();
                    return Token{ start_, 2, Operator_Addition_Assignment };
                }
                return Token{ start_, 1, Plus };
            }
            case '-': {
                advance();
                if (peek() == '=') {
                    advance();
                    return Token{ start_, 2, Operator_Subtraction_Assignment };
                }
                return Token{ start_, 1, Minus };
            }
            case '*': {
                if (peek() == '=') {
                    advance();
                    return Token{ start_, 2, Operator_Multiplication_Assignment };
                }
                advance();
                return Token{ start_, 1, Star };
            }
            case '/': {
                advance();
                if (peek() == '/') {
                    advance();
                    while (!is_eof()) {
                        advance();
                    }
                    return Token{ start_, (size_t)(current_ - start_), Comment };
                } else if (peek() == '=') {
                    advance();
                    return Token{ start_, 2, Operator_Multiplication_Assignment };
                }
            }
        }
    }

    // TODO: CHANGE THIS TO INVALID AND HANDLE IN THE PRINT STATMENTS!!!
    advance();
    return (Token){ start_, (size_t)(current_ - start_), TokenType::Unknown };
}

void Lexer::exception(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // TODO: ADD LINE:FILE format
    printf("Exception called: ");
    vprintf(fmt, args);
    va_end(args);
}

bool Lexer::validate_token(Token t) {
    // Catch errors
    if (t.type > 400) {
        switch (t.type) {
            case Malformed_Float_Literal: {
                exception("Malformed float literal at line %zu\n", current_line());
                return true;
            }
            case Unterminated_String_Literal: {
                exception("Unterminated string literal");
                return true;
            }
            default:
                exception("Unhandled error! Enumeration element is above 400 (which signifies have an error)\n"
                          "but is not handled in the source.\n");
                return true;
        }
    }
    return false;
}
