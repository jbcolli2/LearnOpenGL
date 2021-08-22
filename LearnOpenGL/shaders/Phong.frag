#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;


struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int shininess;
};

uniform Material material;


uniform vec3 lightColor;
uniform vec3 lightPos;
uniform mat4 view;


void main()
{
    vec3 ambLight = material.ambient * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(view*vec4(lightPos,1.0))-FragPos);
    float diffAngleCoeff = max( dot(norm, lightDir), 0.0);
    vec3 diffLight = material.diffuse*diffAngleCoeff*lightColor;
    
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    vec3 viewDir = normalize(-FragPos);
    float specAngleCoeff = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specLight = material.specular*specAngleCoeff*lightColor;
    
    vec3 result = (ambLight + diffLight + specLight) * objColor;
    FragColor = vec4(result, 1.0);
}
