/*
 * scene.cc Copyright 2024 Alwin Leerling dna.leerling@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <array>
#include <vector>

#include "load_shaders.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

unsigned int make_scene()
{
	constexpr unsigned int position_index = 0;
	constexpr unsigned int colour_index = 1;

	struct vertex {
		std::array<float, 3> position;
		std::array<float, 3> colour;
	};

	struct attribute_description {
		unsigned int index;
		int component_count;
		unsigned int component_type;
		unsigned char is_normalized;
		size_t offset;
	};

	const std::vector<attribute_description> vertex_description = {
		{ position_index, 3, GL_FLOAT, GL_FALSE, offsetof( vertex, position ) }, // position attribute
		{ colour_index, 3, GL_FLOAT, GL_FALSE, offsetof( vertex, colour ) }		 // colour attribute
	};

	std::vector<vertex> vertices = { { { -1.0F, -1.0F, 0.0F }, { 1.0F, 0.0F, 0.0F } },
									 { { 0.0F, 1.0F, 0.0F }, { 0.0F, 1.0F, 0.0F } },
									 { { 1.0F, -1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F } } };

	const unsigned int program_id = load_program( "../res/shaders/simple.glsl" );
	unsigned int vertex_buffer = -1;
	unsigned int vao;

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glGenBuffers( 1, &vertex_buffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
	glBufferData( GL_ARRAY_BUFFER, static_cast<int64_t>( vertices.size() * sizeof( vertex ) ), vertices.data(),
				  GL_STATIC_DRAW );

	for( const auto &attribute : vertex_description ) {
		glEnableVertexAttribArray( attribute.index );
		glVertexAttribPointer( attribute.index, attribute.component_count, attribute.component_type,
							   attribute.is_normalized, sizeof( vertex ),
							   /* trunk-ignore(clang-tidy/cppcoreguidelines-pro-type-reinterpret-cast) */
							   /* trunk-ignore(clang-tidy/performance-no-int-to-ptr) */
							   reinterpret_cast<const void *>( attribute.offset ) );
	}

	glUseProgram( program_id );

	glBindVertexArray( 0 );

	return vao;
}
