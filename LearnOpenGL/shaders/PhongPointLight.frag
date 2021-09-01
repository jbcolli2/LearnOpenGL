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
    
    float attenConst;
    float attenLinear;
    float attenQuad;
    
    

};

uniform Material material;
uniform Light light;
uniform sampler2D emissive;



uniform mat4 view;


void main()
{
    float dist = length(light.position - FragPos);
    float attenuation = 1.0/(light.attenConst + dist*light.attenLinear + dist*dist*light.attenQuad);
    vec3 ambient = light.ambient*attenuation;
    vec3 diffuse = light.diffuse*attenuation;
    vec3 specular = light.specular*attenuation;
    
    vec3 ambLight = vec3(texture(material.diffuse,UV)) * ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(view*vec4(light.position,1.0))-FragPos);
    float diffAngleCoeff = max( dot(norm, lightDir), 0.0);
    vec3 diffLight = diffAngleCoeff*vec3(texture(material.diffuse,UV))*diffuse;
    
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    vec3 viewDir = normalize(-FragPos);
    float specAngleCoeff = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specLight = specAngleCoeff*vec3(texture(material.specular,UV))*specular;
    
    vec3 result = (ambLight + diffLight + specLight + vec3(texture(emissive,UV)));
    FragColor = vec4(result, 1.0);
}
