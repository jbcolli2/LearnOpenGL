#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;


struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;



uniform mat4 view;


void main()
{
    vec3 ambLight = material.ambient * light.ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(view*vec4(light.position,1.0))-FragPos);
    float diffAngleCoeff = max( dot(norm, lightDir), 0.0);
    vec3 diffLight = diffAngleCoeff*material.diffuse*light.diffuse;
    
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    vec3 viewDir = normalize(-FragPos);
    float specAngleCoeff = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specLight = specAngleCoeff*material.specular*light.specular;
    
    vec3 result = (ambLight + diffLight + specLight);
    FragColor = vec4(result, 1.0);
}
