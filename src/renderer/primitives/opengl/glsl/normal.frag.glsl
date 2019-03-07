#version 330 core

in vec3 normal;

out vec4 color;

void main()
{
    color = vec4((normal.r + 1.0) / 2.0, (normal.g + 1.0) / 2.0, (normal.b + 1.0) / 2.0, 1.0f);
}
