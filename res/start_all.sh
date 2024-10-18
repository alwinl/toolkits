#!/bin/sh

cd build/src

for prog in freeglut_glew freeglut_glad glfw_glad glfw_glew;
do
	./${prog}
done
