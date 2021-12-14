#version 330 core

out vec4 FragColor;
in vec2 UV;
in vec4 pos;

#define PI 3.1415926538
const float offset = 1.0/300.0;

uniform sampler2D FBOtex;

void main()
{
    vec2 offsets[6] = vec2[](
        vec2(0.0, offset),     // top center
        vec2(offset, offset),  // top right
        vec2(0.0, 0.0),         // center
        vec2(offset, 0.0),      // center right
        vec2(0.0, -offset),     // bottom center
        vec2(offset, -offset)   // bottom right
    );

    float kernel[6] = float[](
        1.0, 1.0,
        4.0, 2.0,
        1.0, 1.0
    );
    

    vec3 color = vec3(0.0);
    for (int ii = 0; ii < 9; ii++)
    {
        color += (1/10.0)*kernel[ii] * vec3(texture(FBOtex, UV+offsets[ii]));
    }
    
    
//    vec4 color = texture(FBOtex, UV);
//    color = vec4(1.0);
    FragColor = vec4(vec3(color), 1.0);
}
