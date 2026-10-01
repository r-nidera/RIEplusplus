#pragma once

#include <cstddef>


namespace parse {

class Parser {};


enum TokenType {
    Identifier,
    Eof
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

    size_t current_line() const { return line_; }
    Token next_token(void);
};

} // namespace parse
