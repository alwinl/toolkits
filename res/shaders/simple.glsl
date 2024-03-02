example shaders

:vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

out vec3 a_colour;

void main()
{
  gl_Position = vec4( position, 1.0 );
  a_colour = colour;
}

:fragment
#version 330 core

in vec3 a_colour;

out vec3 color;

void main()
{
  color = a_colour;
}
