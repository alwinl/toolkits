#
# CMakeLists.txt Copyright 2024 Alwin Leerling dna.leerling@gmail.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
#

include(FetchContent)

FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG        origin/master
)

FetchContent_GetProperties(glew)

if(NOT glew_POPULATED)
  FetchContent_Populate(glew)

  add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(glewinfo ${glew_SOURCE_DIR}/src/glewinfo.c)
target_link_libraries(glewinfo PRIVATE libglew_static)
