#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;

out vec3 normal;
out vec3 pos;

uniform mat4 model;
uniform mat4 MVP;

void main()
{
    normal = mat3(transpose(inverse(model))) * in_norm;
    pos = vec3(model * vec4(in_pos, 1.0f));
    gl_Position = MVP * vec4(in_pos, 1.0f);
} 