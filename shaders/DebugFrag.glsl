#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D fboTex;

void main()
{
    float color = texture(fboTex, UV).r;

    
    FragColor = vec4( vec3(color), 1.0);
}
