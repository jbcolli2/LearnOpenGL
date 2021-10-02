#version 330 core

out vec4 FragColor;


uniform vec4 color;
uniform mat4 model;

void main()
{
    FragColor = color;
}
