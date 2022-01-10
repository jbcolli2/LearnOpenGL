#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aT;
layout (location = 4) in vec3 aB;



struct PointLight
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constAtten;
    float linAtten;
    float quadAtten;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient, diffuse, specular;
};




out VS_OUT
{
    vec3 Normal;
    vec2 UV;
} vs_out;

out InvTBN
{
    vec3 viewDir;
    vec3 FragPos;
    vec3 ptLightPos;
    vec3 dirLightDir;
} invTBN_out;



uniform mat4 model;
uniform vec3 cameraPos;
uniform PointLight ptLights[5];
uniform DirLight dirLights[5];


layout (std140) uniform VP
{
    mat4 view;
    mat4 proj;
};


void main()
{
    gl_Position = proj*view*model*vec4(apos, 1.0f);
//    gl_Position = view*model*vec4(pos, 1.0f);

    // Transformal normal to world space
    mat3 invTransModel = mat3(inverse(transpose(model)));
    vs_out.Normal = invTransModel*aNormal;
    
    vs_out.UV = aUV;
    
    // Creating world --> tangent space matrix
    vec3 T = normalize(vec3(model*vec4(aT, 0.0)));
    vec3 B = normalize(vec3(model*vec4(aB, 0.0)));
    vec3 N = normalize(vec3(model*vec4(aNormal, 0.0)));
    mat3 invTBN = transpose(mat3(T, B, N));
    
    // Converting lighting variables to tangent space
    vec3 FragPos = vec3(model*vec4(apos, 1.0));
    invTBN_out.viewDir = normalize(invTBN*(cameraPos - FragPos));
    invTBN_out.FragPos = invTBN*(FragPos);
    invTBN_out.ptLightPos = invTBN*ptLights[0].position;
    invTBN_out.dirLightDir = normalize(invTBN*dirLights[0].direction);
}
