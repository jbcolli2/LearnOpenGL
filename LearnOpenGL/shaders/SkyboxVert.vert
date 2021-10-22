#version 330 core

layout (location = 0) in vec3 apos;

out vec3 samplerCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj*view*vec4(apos, 1.0);
    samplerCoords = apos;
}
