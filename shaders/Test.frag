#version 330 core

out vec4 FragColor;
in vec2 UV;

struct Material
{
    sampler2D diffuse0;
    
};

uniform Material material;

float near = 0.1;
float far = 100.0;

float Lin(float z)
{
    float ndc = 2.0*z - 1.0;
    return (2.0*near*far)/(near + far - ndc*(far - near));
}

void main()
{
    float depth = Lin(gl_FragCoord.z)/far;
    FragColor = vec4(vec3(depth), 1.0);
}
