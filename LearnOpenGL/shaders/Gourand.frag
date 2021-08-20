#version 330 core

in vec3 FragValue;

out vec4 FragColor;





void main()
{
    FragColor = vec4(FragValue,1.0);
}
