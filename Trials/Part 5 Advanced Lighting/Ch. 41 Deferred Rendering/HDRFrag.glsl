#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D fboTex;
uniform bool hdr;
uniform float exposure;

void main()
{
    float gamma = 2.2;
    
    vec3 hdrColor = texture(fboTex, UV).rgb;
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
    
    
    ldrColor = pow(ldrColor, vec3(1.0/gamma));

    
    FragColor = vec4(ldrColor, 1.0);
}
