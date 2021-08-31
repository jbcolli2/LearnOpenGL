#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 UV;

out vec4 FragColor;


struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform sampler2D emissive;



uniform mat4 view;


void main()
{
    vec3 ambLight = vec3(texture(material.diffuse,UV)) * light.ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(view*vec4(light.position,1.0))-FragPos);
    float diffAngleCoeff = max( dot(norm, lightDir), 0.0);
    vec3 diffLight = diffAngleCoeff*vec3(texture(material.diffuse,UV))*light.diffuse;
    
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    vec3 viewDir = normalize(-FragPos);
    float specAngleCoeff = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specLight = specAngleCoeff*vec3(texture(material.specular,UV))*light.specular;
    
    vec3 result = (ambLight + diffLight + specLight + vec3(texture(emissive,UV)));
    FragColor = vec4(result, 1.0);
}
