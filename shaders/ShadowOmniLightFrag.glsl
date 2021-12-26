#version 330 core

in vec4 FragPos;

out vec4 FragColor;

uniform float farPlane;
uniform vec3 lightPos;


void main()
{
    float depth = length(FragPos.xyz - lightPos);
    
    depth = depth / farPlane;
    gl_FragDepth = depth;
//    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    
}
