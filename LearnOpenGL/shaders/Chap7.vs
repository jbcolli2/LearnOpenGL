#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoords;

out vec3 ourColor;
out vec2 ourTexCoords;

void main()
{
    gl_Position = vec4(pos, 1.0f);
    ourColor = color;
    ourTexCoords = texCoords;
}
