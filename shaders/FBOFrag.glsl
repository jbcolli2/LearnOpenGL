#version 330 core

out vec4 FragColor;
in vec2 UV;


uniform sampler2D FBOtex;

void main()
{
    float depth = texture(FBOtex, UV).r;
    FragColor = vec4(vec3(depth), 1.0);
//    FragColor = texture(FBOtex,UV);
}
