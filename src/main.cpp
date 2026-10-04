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

#include <cstdio>
#include <io.hpp>
#include <parse.hpp>
#include <process.hpp>

#include <iostream>

int main(int argc, char **argv) {
    process::ArgumentHandler ah(argc, argv);

    // Get args
    if (ah.parse() != 0) {
        return 0;
    }

    io::FileBuffer fb;
    // If we don't provide anything, exit.
    // If opening the file failed, exit (it prints the error message).
    if (ah.action() == process::Nothing || fb.init(ah.output_string()) != 0) {
        return 1;
    }

    // Tokenize the file and print it one by one
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
