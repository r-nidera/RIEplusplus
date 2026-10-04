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

#define PROGRAM_VERSION "0.0.1"

namespace process {

enum ArgActionType {
    Nothing,
    Compile,
};

class ArgumentHandler {
    int argc_ = 0;
    char **argv_ = nullptr;

    ArgActionType action_ = Nothing;
    const char *output_string_ = nullptr;

  public:
    ArgumentHandler(int argc, char **argv) : argc_(argc), argv_(argv) {}
    int parse(void);

    ArgActionType action() const { return action_; }
    const char *output_string() const { return output_string_; }
};

} // namespace process
