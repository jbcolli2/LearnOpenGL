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
    vec4 color = texture(material.diffuse0, UV);
    if(color.a < 0.1)
        discard;
    
    
    FragColor = color;
}
