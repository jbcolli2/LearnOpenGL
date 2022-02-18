#version 330 core


in vec2 UV;

out vec4 FragColor;

uniform sampler2D FragPos;
uniform sampler2D Normal;
uniform sampler2D DiffSpec;


struct PointLight
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constAtten;
    float linAtten;
    float quadAtten;
    
    float radius;
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
uniform bool hdr;
uniform float exposure;
uniform bool specularMap;
uniform vec3 cameraPos;



#define MAX_LIGHTS 32
uniform int numDirLights, numPtLights, numSpotLights;

//*********************************************
//            Material and light uniforms
//*********************************************
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




vec3 toneMap(vec3 hdrColor);



//*********************************************
//           Main()
//*********************************************
void main()
{
    vec3 normal = normalize(texture(Normal,UV).rgb);
    vec3 Pos = vec3(texture(FragPos, UV));
    vec3 viewDir = normalize(cameraPos - Pos);
    vec3 result = vec3(0.0);
    if(numDirLights > 0)
    {
        result += computeDirLight(dirLights[0], normal, viewDir);
    }

    for(int ii = 0; ii < numPtLights; ++ii)
    {
        float distToLight = length(Pos - ptLights[ii].position);
//        if(distToLight < ptLights[ii].radius)
        {
            result += computePtLight(ptLights[ii], normal, viewDir, Pos);
        }
        
    }
    
    if(numSpotLights > 0)
    {
        result += computeSpotLight(spotLights[0], normal, viewDir, Pos);
    }
    
    result = toneMap(result);
    FragColor = vec4(result, 1.0);
    
//    FragColor = vec4(vec3(texture(DiffSpec, UV)), 1.0);
//    FragColor = vec4(1.0);
    

}


vec3 toneMap(vec3 hdrColor)
{
    float gamma = 2.2;
    
    vec3 ldrColor;
    
    if(hdr)
    {
//        ldrColor = hdrColor/(hdrColor + vec3(1.0));
        
        //Exposure mapping
        ldrColor = vec3(1.0) - exp(-hdrColor*exposure);
    }
    else
    {
        ldrColor = hdrColor;
    }
    
    
    return pow(ldrColor, vec3(1.0/gamma));

}



//*********************************************
//           Helper functions
//*********************************************
float computeDiffCoeff(vec3 light2Frag, vec3 normal)
{
    return max( dot(normal, -light2Frag), 0.0);
}


float computeSpecCoeff(vec3 light2Frag, vec3 normal, vec3 viewDir)
{
    if(phong)
    {
        vec3 reflectDir = normalize(reflect(light2Frag, normal));
        return pow(max(dot(reflectDir, viewDir), 0.0), 16.0);
    }
    else
    {
        vec3 halfwayDir = normalize(-light2Frag + viewDir);
        return pow(max(dot(halfwayDir, normal), 0.0), 16.0);
    }
}








//*********************************************
//           Direction Light computation
//*********************************************
vec3 computeDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // The precomputed variables for the calculations
    vec3 light2Frag = normalize(light.direction);
    vec3 diffuseMat = vec3(texture(DiffSpec, UV));
    vec3 specMat;
    // If not specular map, assume constant specularity of 1.0 across texture
    specMat = vec3(texture(DiffSpec, UV).a);
    
    
    //////////  Compute Amb/Diff/Spec contributions /////////////

    //Compute ambient component
    vec3 ambComponent = diffuseMat * light.ambient;

    //Compute the diffuse component
    float diffAngleCoeff = computeDiffCoeff(light2Frag, normal);
    vec3 diffComponent = diffAngleCoeff * diffuseMat * light.diffuse;

    //Compute the specular component
    float specAngleCoeff = computeSpecCoeff(light2Frag, normal, viewDir);
    vec3 specComponent = specAngleCoeff * specMat * light.specular;

    return ambComponent + diffComponent + specComponent;
}







//*********************************************
//           Point Light Computation
//*********************************************
vec3 computePtLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPosition)
{
    // The precomputed variables for the calculation
    vec3 diffuseMat = vec3(texture(DiffSpec, UV));
    vec3 specMat;
    specMat = vec3(texture(DiffSpec, UV).a);

    
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
    
//    ambComponent *= attenuation;
//    diffComponent *= attenuation;
//    specComponent *= attenuation;

    return ambComponent + diffComponent + specComponent;
}






//*********************************************
//           SpotLight Computation
//*********************************************
vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPosition)
{
    // The precomputed variables for the calculation
    vec3 diffuseMat = vec3(texture(DiffSpec, UV));
    vec3 specMat;
    specMat = vec3(texture(DiffSpec, UV).a);
    
    
    vec3 light2Frag = fragPosition - light.position;
    float distLight2Frag = length(light2Frag);
    light2Frag = normalize(light2Frag);
    vec3 frag2Light = -light2Frag;

    float attenuation = 1.0/(light.constAtten + distLight2Frag*light.linAtten +
                             distLight2Frag*distLight2Frag*light.quadAtten);
    
    vec3 spotDirection = normalize(vec3(vec4(light.direction, 0.0)));
    float spotAngle = dot(spotDirection, light2Frag);
    
    // Ambient light if fragment is outside of the cone of light
    vec3 result = light.ambient * diffuseMat;
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
        
        result = (ambComponent + diffComponent + specComponent);
    }
    
    return result;
}
