#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 UV;
in vec4 lightSpaceFragPos;

out vec4 FragColor;


struct Material
{
    sampler2D diffuse0;
    sampler2D specular0;
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

uniform bool phong;

uniform bool specularMap;
uniform vec3 cameraPos;
uniform mat4 lightVP;
uniform samplerCube shadowMap;
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





float computeShadowCoeff(vec3 frag2Light)
{
    float closestDepth = texture(shadowMap, frag2Light).r;
    closestDepth *= farPlane;
    
    float FragDepth = length(frag2Light);
    
//    float bias = max(.01 * (1.0 - dot(normal, normalize(lightDir))), .005);
    float bias = .005;
    
    if(FragDepth > 1.0)
        return 0.0;
    
    return FragDepth - bias > closestDepth ? 1.0 : 0.0;
}




//*********************************************
//           Main()
//*********************************************
void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 result = vec3(0.0);

    for(int ii = 0; ii < numPtLights; ++ii)
    {
        result += computePtLight(ptLights[0], normal, viewDir, FragPos);
    }
    for(int ii = 0; ii < 0; ii++)
    {
        
    }
    
    
    
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
    
    // Shadow Calculation
    float shadowCoeff = computeShadowCoeff(frag2Light);

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

    return ambComponent + (1.0 - shadowCoeff)*(diffComponent + specComponent);
//    return vec3(shadowCoeff);
}






