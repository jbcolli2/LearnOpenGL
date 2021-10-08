#version 330 core

out vec4 FragColor;
in vec2 UV;
in vec2 pos;



uniform sampler2D FBOtex;

void main()
{
    vec4 color = texture(FBOtex, UV);
//    color = vec4(UV.y, 0, 0, 1);
    
    FragColor = color;
}
