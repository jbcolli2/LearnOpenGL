#version 330 core

out vec4 FragColor;


uniform vec3 objColor;
uniform vec3 lightColor;

void main()
{
    float ambIntensity = 0.1;
    vec3 ambLight = ambIntensity * lightColor;
    vec3 result = ambLight * objColor;
    FragColor = vec4(result, 1.0);
}
