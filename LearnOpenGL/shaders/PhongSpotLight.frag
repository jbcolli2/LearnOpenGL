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
    float innerCutoff;
    float outerCutoff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constAtten;
    float linAtten;
    float quadAtten;
    
    

};

uniform Material material;
uniform Light light;
uniform sampler2D emissive;



uniform mat4 view;


void main()
{
    vec3 light2Frag = -normalize(vec3(view*vec4(light.position,1.0))-FragPos);
    vec3 lightDir = -light2Frag;
    
    float flashAngle = dot(light2Frag, normalize(vec3(view*vec4(light.direction, 0.0))));
    vec3 result = light.ambient * vec3(texture(material.diffuse,UV));
    vec3 ambLight = result;
    
    
    if(flashAngle > light.outerCutoff)
    {
        float dist = length(light.position - FragPos);
        float attenuation = 1.0/(light.constAtten + dist*light.linAtten + dist*dist*light.quadAtten);
        
        
        
        vec3 norm = normalize(Normal);
        
        float diffAngleCoeff = max( dot(norm, lightDir), 0.0);
        vec3 diffLight = diffAngleCoeff*vec3(texture(material.diffuse,UV))*light.diffuse;
        
        vec3 reflectDir = normalize(reflect(-lightDir, norm));
        vec3 viewDir = normalize(-FragPos);
        float specAngleCoeff = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
        vec3 specLight = specAngleCoeff*vec3(texture(material.specular,UV))*light.specular;
        
//        ambLight *= attenuation;
//        diffLight *= attenuation;
//        specLight *= attenuation;
        
        
        float fuzzyMult = clamp((flashAngle - light.outerCutoff)/(light.innerCutoff - light.outerCutoff), 0.0, 1.0);
        diffLight *= fuzzyMult;
        specLight *= fuzzyMult;
        
        result = (ambLight + diffLight + specLight + vec3(texture(emissive,UV)));
    }
    
    FragColor = vec4(result, 1.0);
}
