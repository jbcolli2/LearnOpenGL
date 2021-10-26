#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

struct Material
{
    sampler2D diffuse0;
    
};

uniform Material material;
uniform vec3 camPosition;
uniform samplerCube skybox;

void main()
{
    float blendTex = .2;
    vec3 I = normalize(FragPos - camPosition);
    vec3 R = reflect(I, normalize(Normal));
    vec3 colorSkybox = texture(skybox, R).rgb;
    vec3 colorTex = texture(material.diffuse0,UV).rgb;
    vec4 color = vec4((1-blendTex)*colorSkybox + blendTex*colorTex, 1.0);
   
    
    
    FragColor = color;
}
