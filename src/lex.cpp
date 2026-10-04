#include <cctype>
#include <cstring>
#include <parse.hpp>

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

    {     "void", 4,     Keyword_Void },
    {     "char", 4,     Keyword_Char },
    {    "short", 5,    Keyword_Short },
    {      "int", 3,      Keyword_Int },
    {    "float", 5,    Keyword_Float },
    {     "long", 4,     Keyword_Long },
    {   "double", 6,   Keyword_Double },
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
                return parse::Malformed_Float_Literal;
            }
            has_dp = true;
        }
    }
    return (has_dp) ? parse::Float_Literal : parse::Integer_Literal;
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
                return (Token){ start_, 1, parse::Colon };
            }
            case ',': {
                advance();
                return (Token){ start_, 1, parse::Comma };
            }
            case '(': {
                advance();
                return (Token){ start_, 1, parse::Left_Paren };
            }
            case ')': {
                advance();
                return (Token){ start_, 1, parse::Right_Paren };
            }
            case '{': {
                advance();
                return (Token){ start_, 1, parse::Left_Brace };
            }
            case '}': {
                advance();
                return Token{ start_, 1, parse::Right_Brace };
            }
            case '=': {
                advance();
                return Token{ start_, 1, parse::Equal };
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
                return Token{ start_, len, len == 0 ? parse::Unterminated_String_Literal : parse::String_Literal };
            }
            case '+': {
                advance();
                return Token{ start_, 1, parse::Plus };
            }
            case '-': {
                advance();
                return Token{ start_, 1, parse::Minus };
            }
            case '*': {
                advance();
                return Token{ start_, 1, parse::Star };
            }
        }
    }

    // TODO: CHANGE THIS TO INVALID AND HANDLE IN THE PRINT STATMENTS!!!
    advance();
    return (Token){ start_, (size_t)(current_ - start_), TokenType::Unknown };
}
