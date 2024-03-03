/*
 * load_shaders.cc Copyright 2024 Alwin Leerling dna.leerling@gmail.com
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

#include "load_shaders.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

std::vector<std::string> parse_source( std::string path )
{
	enum class eState { NOTHING = -1, VERTEX = 0, FRAGMENT = 1 };

	std::ifstream shader_file( path );

	std::vector<std::string> shaders( 2 );
	eState current_state = eState::NOTHING;
	std::string line;

	while( std::getline( shader_file, line ) ) {

		if( line.find( ":vertex" ) != std::string::npos ) {
			current_state = eState::VERTEX;
		} else if( line.find( ":fragment" ) != std::string::npos ) {
			current_state = eState::FRAGMENT;
		} else
			switch( current_state ) {
			case eState::VERTEX: shaders[0] += line + "\n"; break;
			case eState::FRAGMENT: shaders[1] += line + "\n"; break;
			default: break;
			}
	}

	return shaders;
}

unsigned int make_shader( unsigned int shader_type, std::string code )
{
	const unsigned int shader_id = glCreateShader( shader_type );

	char const *source = code.c_str();
	glShaderSource( shader_id, 1, &source, nullptr );

	glCompileShader( shader_id );

	int shader_compiled = GL_FALSE;
	glGetShaderiv( shader_id, GL_COMPILE_STATUS, &shader_compiled );

	if( shader_compiled == GL_FALSE ) {

		int size = 0;
		glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &size );

		if( size == 0 )
			throw std::runtime_error( "Unknown shader compile error" );

		std::vector<char> log( size + 1 );
		glGetShaderInfoLog( shader_id, size, nullptr, log.data() );

		throw std::runtime_error( std::string( log.begin(), log.end() ) );
	}

	return shader_id;
}

unsigned int make_program( std::vector<unsigned int> shader_ids )
{
	unsigned int program_id = glCreateProgram();

	auto attach_fun = [&program_id]( unsigned int shader_id ) { glAttachShader( program_id, shader_id ); };
	auto detach_fun = [&program_id]( unsigned int shader_id ) {
		glDetachShader( program_id, shader_id );
		glDeleteShader( shader_id );
	};

	std::for_each( shader_ids.begin(), shader_ids.end(), attach_fun );

	glLinkProgram( program_id );

	std::for_each( shader_ids.begin(), shader_ids.end(), detach_fun );

	int program_linked = GL_FALSE;
	glGetProgramiv( program_id, GL_LINK_STATUS, &program_linked );

	if( program_linked == GL_FALSE ) {

		int size = 0;
		glGetShaderiv( program_id, GL_INFO_LOG_LENGTH, &size );

		if( size == 0 )
			throw std::runtime_error( "Unknown program link error" );

		std::vector<char> log( size + 1 );
		glGetProgramInfoLog( program_id, size, nullptr, log.data() );

		throw std::runtime_error( std::string( log.begin(), log.end() ) );
	}

	return program_id;
}

unsigned int load_program( std::string path )
{
	std::vector<std::string> shaders = parse_source( path );

	const std::vector<unsigned int> shader_ids = { make_shader( GL_VERTEX_SHADER, shaders[0] ),
												   make_shader( GL_FRAGMENT_SHADER, shaders[1] ) };

	return make_program( shader_ids );
}