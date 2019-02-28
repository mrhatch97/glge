#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;

out vec3 normal;

uniform mat4 MVP;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = MVP * vec4(in_pos.xyz, 1.0);
	normal = in_normal;
}
