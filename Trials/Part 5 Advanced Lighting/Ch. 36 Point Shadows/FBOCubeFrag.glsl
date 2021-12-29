#version 330 core

out vec4 FragColor;
in vec3 samplerCoords;



uniform samplerCube fboTex;

void main()
{
    vec4 color = vec4(vec3(texture(fboTex, samplerCoords).r), 1.0);
//    color = vec4(0.5);
    FragColor = color;
}
