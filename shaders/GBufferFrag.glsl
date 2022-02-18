#version 330 core


in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;








void main()
{
    gAlbedo = vec4(vec3(0.95), 1.0);
    
    
    gPosition = FragPos;
    gNormal = Normal;
}
