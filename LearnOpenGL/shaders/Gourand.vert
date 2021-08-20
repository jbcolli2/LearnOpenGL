#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 aNormal;

out vec3 FragValue;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    gl_Position = proj*view*model*vec4(pos, 1.0f);
    vec3 FragPos = vec3(view*model*vec4(pos, 1.0));
    mat3 invTransModel = mat3(inverse(transpose(view*model)));
    vec3 Normal = invTransModel*aNormal;
    
    float ambIntensity = 0.1;
    vec3 ambLight = ambIntensity * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(view*vec4(lightPos,1.0))-FragPos);
    float diffIntensity = max( dot(norm, lightDir), 0.0);
    vec3 diffLight = diffIntensity*lightColor;
    
    int specularity = 32;
    float specIntensity = 0.8;
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    vec3 viewDir = normalize(-FragPos);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), specularity);
    vec3 specLight = specIntensity*spec*lightColor;
    
    FragValue = (ambLight + diffLight + specLight) * objColor;
}
