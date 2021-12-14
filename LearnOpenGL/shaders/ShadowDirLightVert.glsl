#version 330 core

layout (location = 0) in vec3 pos;

out vec2 UV;


uniform mat4 model;
uniform mat4 lightVP;

void main()
{
    gl_Position = lightVP*model*vec4(pos, 1.0f);

}
