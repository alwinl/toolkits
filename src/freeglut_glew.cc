/*
 * freeglut_glew.cc Copyright 2024 Alwin Leerling dna.leerling@gmail.com
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

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <array>
#include <stdexcept>
#include <vector>

#include <cstddef>

#include "load_shaders.h"

class DemoApp
{
public:
	DemoApp( int argc, char **argv );
	~DemoApp() { instance( this ); };

	DemoApp( DemoApp & ) = delete;
	DemoApp( DemoApp && ) = delete;
	DemoApp &operator=( DemoApp & ) = delete;
	DemoApp &operator=( DemoApp && ) = delete;

	int run();

private:
	void scene_setup();
	static void loop();
	void scene_render() const;

	static DemoApp *instance( DemoApp *inst = nullptr )
	{
		static DemoApp *the_inst = nullptr;

		if( the_inst == nullptr )
			the_inst = inst;
		else if( the_inst == inst )
			the_inst = nullptr;

		return the_inst;
	}

	unsigned int vao = -1;

	void process_key( unsigned char key, int mouse_x, int mouse_y );
};

DemoApp::DemoApp( int argc, char **argv )
{
	glutInit( &argc, argv );

	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize( 640, 480 );
	glutCreateWindow( "Hello from FreeGLUT and GLEW" );

	instance( this );

	glutDisplayFunc( []() { ( DemoApp::instance() )->scene_render(); } );

	if( glewInit() != GLEW_OK )
		throw std::runtime_error( "Cannot load GLEW" );

	/* trunk-ignore(clang-tidy/bugprone-easily-swappable-parameters) */
	auto key_callback = []( unsigned char key, int mouse_x, int mouse_y ) {
		( DemoApp::instance() )->process_key( key, mouse_x, mouse_y );
	};

	glutKeyboardFunc( key_callback );
}

/* trunk-ignore(clang-tidy/readability-convert-member-functions-to-static) */
void DemoApp::process_key( unsigned char key, int /*mouse_x*/, int /*mouse_y*/ )
{
	if( key == 0x1B )
		glutLeaveMainLoop();
}

void DemoApp::scene_setup()
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
}

void DemoApp::scene_render() const
{
	const int display_w = glutGet( GLUT_WINDOW_WIDTH );
	const int display_h = glutGet( GLUT_WINDOW_HEIGHT );

	glViewport( 0, 0, display_w, display_h );

	glClearColor( 0.0F, 0.0F, 0.6F, 0.0F );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glBindVertexArray( vao );

	glDrawArrays( GL_TRIANGLES, 0, 3 );

	glutSwapBuffers();
}

void DemoApp::loop()
{
	glutMainLoop();
}

int DemoApp::run()
{
	scene_setup();

	loop();

	return 0;
}

int main( int argc, char *argv[] )
{
	return DemoApp( argc, argv ).run();
}