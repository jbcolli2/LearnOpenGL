#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

uniform float horzMove;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(pos.x + horzMove, pos.y, pos.z, 1.0f);
    ourColor = color;
}
