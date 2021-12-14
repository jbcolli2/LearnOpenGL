#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 translation;

out vec3 color;


void main()
{
    gl_Position = vec4(aPos + translation, 1.0);
    color = aColor;
}
