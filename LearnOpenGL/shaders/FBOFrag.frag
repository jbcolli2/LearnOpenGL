#version 330 core

out vec4 FragColor;
in vec2 UV;
in vec2 pos;

#define PI 3.1415926538
const float offset = 1.0/300.0;

uniform sampler2D FBOtex;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), //top left
        vec2(0.0, offset),     // top center
        vec2(offset, offset),  // top right
        vec2(-offset, 0.0),     // center left
        vec2(0.0, 0.0),         // center
        vec2(offset, 0.0),      // center right
        vec2(-offset, -offset), //bottom left
        vec2(0.0, -offset),     // bottom center
        vec2(offset, -offset)   // bottom right
    );
    
    float kernel[9] = float[](
        1.25/16, 2.25/16, 1.25/16,
        2.25/16, 2.0/16, 2.25/16,
        1.25/16, 2.25/16, 1.25/16
    );
    
    vec3 color = vec3(0.0);
    for (int ii = 0; ii < 9; ii++)
    {
        color += kernel[ii] * vec3(texture(FBOtex, UV+offsets[ii]));
    }
    
    
//    vec4 color = texture(FBOtex, UV);
    
    FragColor = vec4(color, 1.0);
}
