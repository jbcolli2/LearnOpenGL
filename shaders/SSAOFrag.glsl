#version 330 core

in vec2 UV;

out float Occlusion;

uniform sampler2D Position;
uniform sampler2D Normal;
uniform sampler2D Rotations;

const int numSamples = 64;
uniform vec3 samples[numSamples];
uniform float radius;


uniform float width;
uniform float height;

uniform mat4 proj;


//layout (std140) uniform VP
//{
//    mat4 view;
//    mat4 proj;
//};

const vec2 rotUVscale = vec2(800.0/4.0, 600.0/4.0);
void main()
{
    Occlusion = 0.0;
    float bias = 0.025;
    vec3 fragPos = texture(Position, UV).rgb;

    //////////  Get rotation /////////////
    vec3 rotation = normalize(texture(Rotations, UV * rotUVscale).rgb);

    //////////  TBN matrix /////////////
    vec3 N = normalize(texture(Normal, UV).rgb);
    vec3 T = normalize(rotation - dot(rotation, N)*N);
    vec3 B = cross(T,N);
    mat3 TBN = mat3(T,B,N);
    
    float geometryDepth, sampleDepth;
    for(int ii = 0; ii < numSamples; ++ii)
    {
        vec3 samplesView = TBN*samples[ii];
        samplesView = fragPos + radius*samplesView;
        sampleDepth = samplesView.z;
//
        vec4 sampleUV = vec4(samplesView, 1.0);
        sampleUV = proj*sampleUV;
//        sampleUV.xyz /= sampleUV.w;
//        sampleUV.xyz = sampleUV.xyz * 0.5 + 0.5;
//
        geometryDepth = texture(Position, sampleUV.xy).z;
//

//        Occlusion += geometryDepth >= sampleDepth + bias ? 1.0 : 0.0;
//        Occlusion += clamp(100000000*(geometryDepth - (sampleDepth + bias)), 0.0, 1.0);
        Occlusion += geometryDepth - sampleDepth - bias;
        
        Occlusion += 0.5;
    }
    
    Occlusion = 1.0 - Occlusion/numSamples;
}
