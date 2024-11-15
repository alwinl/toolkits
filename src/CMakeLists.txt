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

find_package(OpenGL REQUIRED)

add_executable(freeglut_glew freeglut_glew.cc scene.cc)
add_executable(glfw_glew glfw_glew.cc scene.cc)
add_executable(freeglut_glad freeglut_glad.cc scene.cc)
add_executable(glfw_glad glfw_glad.cc scene.cc)

target_compile_definitions( glfw_glad PRIVATE GLAD_LOADER )
target_compile_definitions( freeglut_glad PRIVATE GLAD_LOADER )

if(WIN32)
    target_include_directories( freeglut_glew PRIVATE ${freeglut_SOURCE_DIR}/include ${glew_SOURCE_DIR}/include )
    target_include_directories( freeglut_glad PRIVATE ${freeglut_SOURCE_DIR}/include)
    target_include_directories( glfw_glew PRIVATE ${glfw_SOURCE_DIR}/include ${glew_SOURCE_DIR}/include )
    target_include_directories( glfw_glad PRIVATE ${glfw_SOURCE_DIR}/include)

    target_link_libraries( freeglut_glew freeglut_static libglew_static OpenGL::GL)
    target_link_libraries( freeglut_glad freeglut_static glad_gl_core_33 OpenGL::GL)
    target_link_libraries( glfw_glew glfw libglew_static OpenGL::GL )
    target_link_libraries( glfw_glad glfw glad_gl_core_33 OpenGL::GL )
else()
    target_link_libraries( freeglut_glew freeglut_static libglew_static )
    target_link_libraries( freeglut_glad freeglut_static glad_gl_core_33 )
    target_link_libraries( glfw_glew glfw libglew_static OpenGL::OpenGL )
    target_link_libraries( glfw_glad glfw glad_gl_core_33 OpenGL::OpenGL )
endif()
