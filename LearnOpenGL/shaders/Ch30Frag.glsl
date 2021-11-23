#version 330 core

in vec3 fcolor;
out vec4 FragColor;



void main()
{
    FragColor = vec4(vec3(fcolor), 1.0);
}
