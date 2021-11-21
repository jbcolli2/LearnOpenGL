#version 330 core

layout (location = 0) in vec3 apos;

out vec4 pos;
out vec2 UV;



void main()
{
    gl_Position = vec4(apos, 1.0);
    gl_PointSize = 10;
}
