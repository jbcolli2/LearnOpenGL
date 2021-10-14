#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec2 aUV;

out vec4 pos;
out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj*view*model*vec4(apos, 1.0);
    UV = aUV;

}
