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

# set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL 3.3 REQUIRED)

add_executable(freeglut_glew freeglut_glew.cc load_shaders.cc scene.cc)
add_executable(glfw_glew glfw_glew.cc load_shaders.cc scene.cc)
add_executable(freeglut_glad freeglut_glad.cc load_shaders.cc scene.cc)
add_executable(glfw_glad glfw_glad.cc load_shaders.cc scene.cc)

target_include_directories( freeglut_glew PRIVATE ${freeglut_SOURCE_DIR}/include ${glew_SOURCE_DIR}/include )
target_include_directories( freeglut_glad PRIVATE ${freeglut_SOURCE_DIR}/include)
target_include_directories( glfw_glew PRIVATE ${glfw_SOURCE_DIR}/include ${glew_SOURCE_DIR}/include )
target_include_directories( glfw_glad PRIVATE ${glfw_SOURCE_DIR}/include)

target_link_libraries( freeglut_glew freeglut_static libglew_static )
target_link_libraries( freeglut_glad freeglut_static glad_gl_core_33 )
target_link_libraries( glfw_glew ${OPENGL_LIBRARIES} glfw libglew_static ${OPENGL_gl_LIBRARY})
target_link_libraries( glfw_glad ${OPENGL_LIBRARIES} glfw glad_gl_core_33 ${OPENGL_gl_LIBRARY} )
