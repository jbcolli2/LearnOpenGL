#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;


uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;


void main()
{
    float ambIntensity = 0.1;
    vec3 ambLight = ambIntensity * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = lightPos-FragPos;
    float diffIntensity = max( dot(norm, lightDir), 0.0);
    
    vec3 result = (ambLight + (diffIntensity * lightColor)) * objColor;
    FragColor = vec4(result, 1.0);
}
