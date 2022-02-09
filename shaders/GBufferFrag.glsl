#version 330 core


in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffSpec;


struct Material
{
    sampler2D diffuse0;
    sampler2D specular0;
};

uniform Material material;





void main()
{
    gDiffSpec.rgb = texture(material.diffuse0, UV).rgb;
    gDiffSpec.rgb = 
}
