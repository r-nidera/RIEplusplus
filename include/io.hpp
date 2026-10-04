/*
 * rie++, the compiler and tools for a general-purpose byte-compiled programming language
 * copyright (c) 2026 ronald nidera
 *
 * rie++ is free software: you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation, either version 3 of the license, or
 * (at your option) any later version.
 *
 * rie++ is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * gnu general public license for more details.
 *
 * you should have received a copy of the gnu general public license
 * along with this program.  if not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <memory>

namespace io {

class FileBuffer {
    std::unique_ptr<char[]> buffer_;

  public:
    int init(const char *filepath);
    const char *contents() const { return buffer_.get(); };
};


} // namespace io
