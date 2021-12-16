#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec2 UV;


uniform mat4 model;
uniform mat4 lightVP;
layout (std140) uniform VP
{
    mat4 view;
    mat4 proj;
};

void main()
{
    gl_Position = lightVP*model*vec4(pos, 1.0);

}
