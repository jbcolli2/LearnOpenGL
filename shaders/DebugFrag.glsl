#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D fboTex;

void main()
{
    vec4 color = texture(fboTex, UV);

    
    FragColor = color;
}
