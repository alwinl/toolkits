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

#include <array>
#include <stdexcept>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "load_shaders.h"
#include "scene.h"

class DemoApp
{
public:
	DemoApp( int argc, char **argv );
	~DemoApp();

	DemoApp( DemoApp & ) = delete;
	DemoApp( DemoApp && ) = delete;
	DemoApp &operator=( DemoApp & ) = delete;
	DemoApp &operator=( DemoApp && ) = delete;

	int run();

private:
	void scene_setup();
	void scene_render() const;

	unsigned int vao = -1;

	void process_key( unsigned char key, int mouse_x, int mouse_y );

	static DemoApp *instance( DemoApp *inst = nullptr );
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

DemoApp *DemoApp::instance( DemoApp *inst )
{
	static DemoApp *the_inst = nullptr;

	if( the_inst == nullptr )
		the_inst = inst;
	else if( the_inst == inst )
		the_inst = nullptr;

	return the_inst;
}

DemoApp::~DemoApp()
{
	instance( this );
}

/* trunk-ignore(clang-tidy/readability-convert-member-functions-to-static) */
void DemoApp::process_key( unsigned char key, int /*mouse_x*/, int /*mouse_y*/ )
{
	if( key == 0x1B )
		glutLeaveMainLoop();
}

void DemoApp::scene_setup()
{
	vao = make_scene();
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

int DemoApp::run()
{
	scene_setup();

	glutMainLoop();

	return 0;
}

int main( int argc, char *argv[] )
{
	return DemoApp( argc, argv ).run();
}