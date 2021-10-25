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
    vec3 I = normalize(FragPos);
    vec3 R = reflect(I, normalize(Normal));
    vec4 color = texture(skybox, normalize(R));
    if(color.a < 0.1)
        discard;
    
    
    FragColor = color;
}
