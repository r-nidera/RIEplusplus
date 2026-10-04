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

#include <cstring>
#include <iostream>
#include <ostream>
#include <process.hpp>

using namespace process;

int ArgumentHandler::parse(void) {
    if (argc_ == 1)
        return 1;

    if (strcmp(argv_[1], "build") == 0) {
        if (argc_ < 3) {
            std::cerr << "Please provide at least 1 file." << std::endl;
            return 1;
        }

        // Otherwise, put the string in.
        // And for now, we won't support multiple files because the problem with that is we aren't really done
        // with the actual compiler, bro.
        output_string_ = argv_[2];
        action_ = Compile;
    }

    else if (strcmp(argv_[1], "version") == 0) {
        std::cout << "CRIE V" << PROGRAM_VERSION << "\n"
                  << "This program comes with absolutely NO warranty" << std::endl;
    }

    else {
        std::cout << "Unknown command argument '" << argv_[1] << "'" << std::endl;
    }

    return 0;
}
