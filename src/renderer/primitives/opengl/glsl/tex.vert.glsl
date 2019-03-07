#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

out vec3 normal;
out vec3 frag;
out vec2 tex_coord;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 MVP;
uniform mat4 model;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = MVP * vec4(in_pos, 1.0f);
	tex_coord = in_uv;
}
