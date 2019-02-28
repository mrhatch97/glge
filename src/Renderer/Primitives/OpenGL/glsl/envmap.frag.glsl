#version 330 core
out vec4 color;

in vec3 normal;
in vec3 pos;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(pos - camPos);
    vec3 R = reflect(I, normal);
    color = vec4(texture(skybox, R).rgb, 1.0f);
}