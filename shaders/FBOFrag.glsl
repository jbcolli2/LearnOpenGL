#version 330 core

out vec4 FragColor;
in vec2 UV;


uniform sampler2D FBOtex;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane -
    z * (far_plane - near_plane));
}

void main()
{
    float depth = texture(FBOtex, UV).r;
    depth = LinearizeDepth(depth)/far_plane;
    FragColor = vec4(vec3(depth), 1.0);
//    FragColor = texture(FBOtex,UV);
}
