#version 330 core

layout (location = 0) in vec3 apos;

out vec3 samplerCoords;

layout (std140) uniform VP
{
    mat4 view;
    mat4 proj;
};

void main()
{
    vec4 pos = proj*view*vec4(apos, 1.0);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    samplerCoords = apos;
}
