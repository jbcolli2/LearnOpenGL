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

struct PointLight
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constAtten;
    float linAtten;
    float quadAtten;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient, diffuse, specular;
};

struct SpotLight
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

#define NUM_PTLIGHTS 4
uniform Material material;
uniform PointLight ptLights[NUM_PTLIGHTS];
uniform DirLight dirLight;
uniform SpotLight spotLight;



uniform mat4 view;

vec3 computeDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 computePtLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPosition);
vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPosition);


void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = -normalize(FragPos);
    vec3 result = vec3(0.0);
    result += computeDirLight(dirLight, normal, viewDir);

    for(int ii = 0; ii < NUM_PTLIGHTS; ++ii)
    {
        result += computePtLight(ptLights[ii], normal, viewDir, FragPos);
    }
    
    result += computeSpotLight(spotLight, normal, viewDir, FragPos);
    
    FragColor = vec4(result, 1.0);
}








vec3 computeDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // The precomputed variables for the calculations
    vec3 light2Frag = normalize(vec3(view*vec4(light.direction,0.0)));
    vec3 diffuseMat = vec3(texture(material.diffuse, UV));
    vec3 specMat = vec3(texture(material.specular, UV));

    //Compute ambient component
    vec3 ambComponent = diffuseMat * light.ambient;

    //Compute the diffuse component
    float diffAngleCoeff = max( dot(normal, -light2Frag), 0.0);
    vec3 diffComponent = diffAngleCoeff * diffuseMat * light.diffuse;

    //Computer the specular component
    vec3 reflectDir = normalize(reflect(light2Frag, normal));
    float specAngleCoeff = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specComponent = specAngleCoeff * specMat * light.specular;

    return ambComponent + diffComponent + specComponent;
}





vec3 computePtLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPosition)
{
    // The precomputed variables for the calculation
    vec3 diffuseMat = vec3(texture(material.diffuse, UV));
    vec3 specMat = vec3(texture(material.specular, UV));

    vec3 light2Frag = fragPosition - vec3(view*vec4(light.position,1.0));
    float distLight2Frag = length(light2Frag);
    light2Frag = normalize(light2Frag);
    vec3 frag2Light = -light2Frag;

    float attenuation = 1.0/(light.constAtten + distLight2Frag*light.linAtten +
                             distLight2Frag*distLight2Frag*light.quadAtten);

    //Compute ambient component
    vec3 ambComponent = diffuseMat * light.ambient;

    //Computer diffuse component
    float diffAngleCoeff = max( dot(normal, frag2Light), 0.0);
    vec3 diffComponent = diffAngleCoeff * diffuseMat * light.diffuse;

    //Compute specular component
    vec3 reflectDir = normalize(reflect(light2Frag, normal));
    float specAngleCoeff = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specComponent = specAngleCoeff * specMat * light.specular;
    
    ambComponent *= attenuation;
    diffComponent *= attenuation;
    specComponent *= attenuation;

    return ambComponent + diffComponent + specComponent;
}





vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPosition)
{
    // The precomputed variables for the calculation
    vec3 diffuseMat = vec3(texture(material.diffuse, UV));
    vec3 specMat = vec3(texture(material.specular, UV));

    vec3 light2Frag = fragPosition - vec3(view*vec4(light.position,1.0));
    float distLight2Frag = length(light2Frag);
    light2Frag = normalize(light2Frag);
    vec3 frag2Light = -light2Frag;

    float attenuation = 1.0/(light.constAtten + distLight2Frag*light.linAtten +
                             distLight2Frag*distLight2Frag*light.quadAtten);
    
    vec3 spotDirection = normalize(vec3(view*vec4(light.direction, 0.0)));
    float spotAngle = dot(spotDirection, light2Frag);
    
    // Ambient light if fragment is outside of the cone of light
    vec3 result = light.ambient * vec3(texture(material.diffuse,UV));
    vec3 ambComponent = result;
    
    // Calculations of all light types if fragment is inside the light cone
    if(spotAngle > light.outerCutoff)
    {
        float attenuation = 1.0/(light.constAtten + distLight2Frag*light.linAtten +
                                 distLight2Frag*distLight2Frag*light.quadAtten);
                
        float diffAngleCoeff = max( dot(normal, frag2Light), 0.0);
        vec3 diffComponent = diffAngleCoeff * diffuseMat * light.diffuse;
        
        vec3 reflectDir = normalize(reflect(light2Frag, normal));
        float specAngleCoeff = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
        vec3 specComponent = specAngleCoeff * specMat * light.specular;
        
        ambComponent *= attenuation;
        diffComponent *= attenuation;
        specComponent *= attenuation;
        
        
        float fuzzyMult = clamp((spotAngle - light.outerCutoff)/(light.innerCutoff - light.outerCutoff), 0.0, 1.0);
        diffComponent *= fuzzyMult;
        specComponent *= fuzzyMult;
        
        result = (ambComponent + diffComponent + specComponent);
    }
    
    return result;
}
