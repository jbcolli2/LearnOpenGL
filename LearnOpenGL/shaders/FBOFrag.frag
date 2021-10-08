#version 330 core

out vec4 FragColor;
in vec2 UV;
in vec2 pos;

#define PI 3.1415926538

uniform sampler2D FBOtex;

void main()
{
    vec4 color = texture(FBOtex, UV);
//    color = vec4(UV.y, 0, 0, 1);
    float average = (.2126*color.r + .7152*color.g + .0722*color.b);
    
    FragColor = vec4(vec3(average), 1.0);
}
