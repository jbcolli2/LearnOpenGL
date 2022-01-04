#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aT;
layout (location = 4) in vec3 aB;

out vec3 Normal;
out vec3 FragPos;
out vec2 UV;
out vec4 lightSpaceFragPos;
out mat3 TBN;



uniform mat4 model;
uniform mat4 lightVP;
layout (std140) uniform VP
{
    mat4 view;
    mat4 proj;
};


void main()
{
    gl_Position = proj*view*model*vec4(apos, 1.0f);
//    gl_Position = view*model*vec4(pos, 1.0f);

    FragPos = vec3(model*vec4(apos, 1.0));
    mat3 invTransModel = mat3(inverse(transpose(model)));
    Normal = invTransModel*aNormal;
    UV = aUV;
    lightSpaceFragPos = lightVP * vec4(FragPos, 1.0);
    vec3 T = normalize(vec3(model*vec4(aT, 0.0)));
    vec3 B = normalize(vec3(model*vec4(aB, 0.0)));
    vec3 N = normalize(vec3(model*vec4(aNormal, 0.0)));
    TBN = mat3(T, B, N);
}
