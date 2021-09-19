#version 330 core

out vec4 FragColor;
in vec2 UV;

struct Material
{
    sampler2D diffuse0;
    
};

uniform Material material;

void main()
{
    FragColor = texture(material.diffuse0, UV);
}
