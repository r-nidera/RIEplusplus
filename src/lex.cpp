#include <cctype>
#include <parse.hpp>

using namespace parse;
using namespace std;

void Lexer::skim_whitespace() {
    while (peek() && isspace(static_cast<unsigned char>(peek()))) {
        advance();
    }
}
size_t Lexer::consume_identifier(void) {
    while (isalnum(static_cast<unsigned char>(peek()))) {
        advance();
    }

    return current_ - start_;
}

Token Lexer::next_token() {
    skim_whitespace();

    if (is_eof()) {
        return (Token){ "\0", 0, TokenType::Eof };
    }

    start_ = current_;

    if (isalpha(static_cast<unsigned char>(peek()))) {
        auto len = consume_identifier();
        return (Token){ start_, len, Identifier };
    }

    // TODO: CHANGE THIS TO INVALID AND HANDLE IN THE PRINT STATMENTS!!!
    return (Token){ "Unknown or invalid", 18, TokenType::Eof };
}
