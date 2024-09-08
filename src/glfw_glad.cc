/*
 * glfw_glew.cc Copyright 2024 Alwin Leerling dna.leerling@gmail.com
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

#include <glad/gl.h>
#include <GLFW/glfw3.h>

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

	void process_key( int key, int scancode, int action, int mods );

	GLFWwindow *window = nullptr;
};

DemoApp::DemoApp( int /*argc*/, char ** /*argv*/ )
{
	auto glfw_error_callback = []( int /*code*/, const char *description )
		{ throw std::runtime_error( description ); };

	glfwSetErrorCallback( glfw_error_callback );

	/* Initialize the library */
	if( glfwInit() == 0 )
		throw std::runtime_error( "Cannot initialise GLFW" );

	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow( 640, 480, "Hello from GLFW and GLAD", nullptr, nullptr );
	if( window == nullptr ) {
		glfwTerminate();
		throw std::runtime_error( "Cannot create GLFW Window" );
	}

	/* Make the window's context current */
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 );

	if( gladLoadGL( glfwGetProcAddress ) == 0 )
		throw std::runtime_error( "Cannot load GL pointers" );

	glfwSetWindowUserPointer( window, this );

	/* trunk-ignore(clang-tidy/bugprone-easily-swappable-parameters) */
	auto key_callback = []( GLFWwindow *window, int key, int scancode, int action, int mods ) {
		auto *app = static_cast<DemoApp *>( glfwGetWindowUserPointer( window ) );
		app->process_key( key, scancode, action, mods );
	};

	glfwSetKeyCallback( window, key_callback );
}

DemoApp::~DemoApp()
{
	glfwDestroyWindow( window );
	glfwTerminate();
}

void DemoApp::process_key( int key, int /*scancode*/, int action, int /*mods*/ )
{
	switch( action ) {
	case GLFW_PRESS:
		if( key == GLFW_KEY_ESCAPE )
			glfwSetWindowShouldClose( window, 1 );
		break;
	case GLFW_RELEASE: // fall through
	case GLFW_REPEAT: break;
	}
}

void DemoApp::scene_setup()
{
	vao = make_scene();
}

void DemoApp::scene_render() const
{
	glBindVertexArray( vao );

	glDrawArrays( GL_TRIANGLES, 0, 3 );
}

int DemoApp::run()
{
	scene_setup();

	/* Loop until the user closes the window */
	while( glfwWindowShouldClose( window ) == 0 ) {

		/* Poll for and process events */
		glfwPollEvents();

		/* Render here */
		int display_w = 0;
		int display_h = 0;

		glfwGetFramebufferSize( window, &display_w, &display_h );
		glViewport( 0, 0, display_w, display_h );

		glClearColor( 0.0F, 0.0F, 0.6F, 0.0F );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		scene_render();

		/* Swap front and back buffers */
		glfwSwapBuffers( window );
	}

	return 0;
}

int main( int argc, char **argv )
{
	return DemoApp( argc, argv ).run();
}