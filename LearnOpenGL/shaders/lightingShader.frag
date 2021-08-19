#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;


uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform mat4 view;


void main()
{
    float ambIntensity = 0.1;
    vec3 ambLight = ambIntensity * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(view*vec4(lightPos,1.0))-FragPos);
    float diffIntensity = max( dot(norm, lightDir), 0.0);
    vec3 diffLight = diffIntensity*lightColor;
    
    int specularity = 128;
    float specIntensity = 0.8;
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    vec3 viewDir = normalize(-FragPos);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), specularity);
    vec3 specLight = specIntensity*spec*lightColor;
    
    vec3 result = (ambLight + diffLight + specLight) * objColor;
    FragColor = vec4(result, 1.0);
}
