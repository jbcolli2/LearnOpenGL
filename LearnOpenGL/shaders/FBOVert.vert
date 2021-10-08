#version 330 core

layout (location = 0) in vec2 apos;
layout (location = 1) in vec2 aUV;

out vec2 pos;
out vec2 UV;



void main()
{
    pos = apos;
    UV = aUV;
    gl_Position = vec4(apos.x, apos.y, 0.0, 1.0);

}
