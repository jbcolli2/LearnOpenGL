#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 UV;
in vec4 lightSpaceFragPos;
in mat3 TBN;

out vec4 FragColor;


struct Material
{
    sampler2D diffuse0, diffuse1;
    sampler2D specular0;
    sampler2D normal0;
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

uniform vec3 cameraPos;
uniform bool phong;

uniform bool specularMap;

uniform float farPlane;



#define MAX_LIGHTS 5
uniform int numDirLights, numPtLights, numSpotLights;

//*********************************************
//            Material and light uniforms
//*********************************************
uniform Material material;
uniform PointLight ptLights[MAX_LIGHTS];
uniform DirLight dirLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];


//*********************************************
//            View and Projection Uniform buffer
//*********************************************
layout (std140) uniform VP
{
    mat4 view;
    mat4 proj;
};




vec3 computeDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 computePtLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPosition);
vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPosition);









//*********************************************
//           Main()
//*********************************************
void main()
{
    vec3 normal = normalize(vec3(texture(material.normal0, UV)));
    normal = 2.0*normal - 1.0;
    normal = TBN*normal;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 result = vec3(0.0);
    
    result += computeDirLight(dirLights[0], normal, viewDir);

    for(int ii = 0; ii < numPtLights; ++ii)
    {
        result += computePtLight(ptLights[ii], normal, viewDir, FragPos);
    }
    
    result += computeSpotLight(spotLights[0], normal, viewDir, FragPos);
    
    FragColor = vec4(result, 1.0);
}









float computeDiffCoeff(vec3 light2Frag, vec3 normal)
{
    return max( dot(normal, -light2Frag), 0.0);
}


float computeSpecCoeff(vec3 light2Frag, vec3 normal, vec3 viewDir)
{
    if(phong)
    {
        vec3 reflectDir = normalize(reflect(light2Frag, normal));
        return pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    }
    else
    {
        vec3 halfwayDir = normalize(-light2Frag + viewDir);
        return pow(max(dot(halfwayDir, normal), 0.0), material.shininess);
    }
}




//*********************************************
//           Direction Light computation
//*********************************************
vec3 computeDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // Compute the shadow coefficient
    
    
    // The precomputed variables for the calculations
    vec3 light2Frag = normalize(light.direction);
    vec3 diffuseMat = vec3(texture(material.diffuse0, UV));
    vec3 specMat;
    // If not specular map, assume constant specularity of 1.0 across texture
    if(specularMap)
    {
        specMat = vec3(texture(material.specular0, UV));
    }
    else
    {
        specMat = vec3(1.0);
    }
    
    
    //////////  Compute Amb/Diff/Spec contributions /////////////

    //Compute ambient component
    vec3 ambComponent = diffuseMat * light.ambient;

    //Compute the diffuse component
    float diffAngleCoeff = computeDiffCoeff(light2Frag, normal);
    vec3 diffComponent = diffAngleCoeff * diffuseMat * light.diffuse;

    //Compute the specular component
    float specAngleCoeff = computeSpecCoeff(light2Frag, normal, viewDir);
    vec3 specComponent = specAngleCoeff * specMat * light.specular;

    return ambComponent + (diffComponent + specComponent);
}







//*********************************************
//           Point Light Computation
//*********************************************
vec3 computePtLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPosition)
{
    // The precomputed variables for the calculation
    vec3 diffuseMat = vec3(texture(material.diffuse0, UV));
    vec3 specMat;
    if(specularMap)
    {
        specMat = vec3(texture(material.specular0, UV));
    }
    else
    {
        specMat = vec3(1.0);
    }
    
    vec3 light2Frag = fragPosition - light.position;
    float distLight2Frag = length(light2Frag);
    light2Frag = normalize(light2Frag);
    vec3 frag2Light = -light2Frag;
    

    float attenuation = 1.0/(light.constAtten + distLight2Frag*light.linAtten +
                             distLight2Frag*distLight2Frag*light.quadAtten);

    //Compute ambient component
    vec3 ambComponent = diffuseMat * light.ambient;

    //Compute diffuse component
    float diffAngleCoeff = computeDiffCoeff(light2Frag, normal);
    vec3 diffComponent = diffAngleCoeff * diffuseMat * light.diffuse;

    //Compute specular component
    float specAngleCoeff = computeSpecCoeff(light2Frag, normal, viewDir);
    vec3 specComponent = specAngleCoeff * specMat * light.specular;
    
    ambComponent *= attenuation;
    diffComponent *= attenuation;
    specComponent *= attenuation;

    return ambComponent + diffComponent + specComponent;
}






//*********************************************
//           SpotLight Computation
//*********************************************
vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPosition)
{
    // The precomputed variables for the calculation
    vec3 diffuseMat = vec3(texture(material.diffuse0, UV));
    vec3 specMat;
    if(specularMap)
    {
        specMat = vec3(texture(material.specular0, UV));
    }
    else
    {
        specMat = vec3(1.0);
    }
    
    
    vec3 light2Frag = fragPosition - light.position;
    float distLight2Frag = length(light2Frag);
    light2Frag = normalize(light2Frag);
    vec3 frag2Light = -light2Frag;
    

    float attenuation = 1.0/(light.constAtten + distLight2Frag*light.linAtten +
                             distLight2Frag*distLight2Frag*light.quadAtten);
    
    vec3 spotDirection = normalize(vec3(vec4(light.direction, 0.0)));
    float spotAngle = dot(spotDirection, light2Frag);
    
    // Ambient light if fragment is outside of the cone of light
    vec3 result = light.ambient * vec3(texture(material.diffuse0,UV));
    vec3 ambComponent = result;
    
    // Calculations of all light types if fragment is inside the outer edge of the light cone
    if(spotAngle > light.outerCutoff)
    {
        float attenuation = 1.0/(light.constAtten + distLight2Frag*light.linAtten +
                                 distLight2Frag*distLight2Frag*light.quadAtten);
                
        //Compute diffuse contribution
        float diffAngleCoeff = computeDiffCoeff(light2Frag, normal);
        vec3 diffComponent = diffAngleCoeff * diffuseMat * light.diffuse;
        
        // Compute specular contribution
        float specAngleCoeff = computeSpecCoeff(light2Frag, normal, viewDir);
        vec3 specComponent = specAngleCoeff * specMat * light.specular;
        
        ambComponent *= attenuation;
        diffComponent *= attenuation;
        specComponent *= attenuation;
        
        
        float fuzzyMult = clamp((spotAngle - light.outerCutoff)/(light.innerCutoff - light.outerCutoff), 0.0, 1.0);
        diffComponent *= fuzzyMult;
        specComponent *= fuzzyMult;
        
        result = ambComponent + (diffComponent + specComponent);
    }
    
    return result;
}
