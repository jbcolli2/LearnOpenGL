#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 Normal;
out vec3 FragPos;
out vec2 UV;
out vec4 lightSpaceFragPos;


uniform mat4 model;
uniform mat4 lightVP;
layout (std140) uniform VP
{
    mat4 view;
    mat4 proj;
};


void main()
{
    gl_Position = proj*view*model*vec4(pos, 1.0f);
//    gl_Position = view*model*vec4(pos, 1.0f);

    FragPos = vec3(model*vec4(pos, 1.0));
    mat3 invTransModel = mat3(inverse(transpose(model)));
    Normal = invTransModel*aNormal;
    UV = aUV;
    lightSpaceFragPos = lightVP * vec4(FragPos, 1.0);
}
