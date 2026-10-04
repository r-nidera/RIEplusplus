#include <cstdio>
#include <io.hpp>
#include <parse.hpp>
#include <process.hpp>

#include <iostream>

int main(int argc, char **argv) {
    process::ArgumentHandler ah(argc, argv);

    if (ah.parse() != 0) {
        return 0;
    }

    io::FileBuffer fb;
    if (ah.action() == process::Nothing || fb.init(ah.output_string()) != 0) {
        return 1;
    }

    parse::Lexer lex(fb.contents());
    parse::Token t = lex.next_token();
    while (t.type != parse::Eof) {
        std::printf("%zu [%.*s, %zu, %d]\n", lex.current_line(), static_cast<int>(t.len), t.s, t.len, t.type);

        // Catch bugs
        if (t.type == parse::Unterminated_String_Literal) {
            std::cout << "Unterminated string literal at line " << lex.current_line() << std::endl;
        }
        t = lex.next_token();
    }

    return 0;
}
