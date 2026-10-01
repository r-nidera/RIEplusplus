#
# CRIE -- A Turing-complete byte-compiled programming language
# Copyright (C) 2026 Ronald Nidera
#
# CRIE is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CRIE is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.15)

project(crie VERSION 0.01 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(crie
  src/main.cpp
  src/process.cpp
  src/io.cpp
  src/lex.cpp
  src/parse.cpp
)

target_compile_options(crie
  PRIVATE
  -Wall
  -Wextra
  -Werror

  -fno-exceptions
  -fno-rtti
)

target_include_directories(crie
  PRIVATE
  include
)
