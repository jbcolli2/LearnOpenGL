#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoords;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
    FragColor = mix(texture(tex1,ourTexCoords), texture(tex2,ourTexCoords), .4);
//    FragColor = vec4(ourColor, 1.0);
}
