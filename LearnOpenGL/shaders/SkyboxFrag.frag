#version 330 core

out vec4 FragColor;
in vec3 samplerCoords;



uniform samplerCube skybox;

void main()
{
    vec4 color = texture(skybox, samplerCoords);
//    color = vec4(1.0);
    FragColor = color;
}
