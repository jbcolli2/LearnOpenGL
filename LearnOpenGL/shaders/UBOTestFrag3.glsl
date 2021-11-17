#version 330 core

out vec4 FragColor;
in vec2 UV;

struct Material
{
    sampler2D diffuse0;
    sampler2D specular0;
};

uniform Material material;

layout (std140) uniform colors
{
    vec3 color1;
    vec3 color2;
    vec3 color3;
    vec3 color4;
};

void main()
{
    vec4 color = texture(material.diffuse0, UV);
    if(color.a < 0.1)
        discard;
    
    color = color*vec4(color3, 1.0);
//    color = vec4(1.0);
    
    
    
    FragColor = color;
}
