#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 Normal[];



uniform float magnitude;

layout (std140) uniform VP
{
    mat4 view;
    mat4 proj;
};
void drawNormalLine(int index)
{
    vec3 normal = normalize(Normal[index]);
 
    gl_Position = proj*gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = proj*(gl_in[index].gl_Position + vec4(magnitude*normal, 0.0));
    EmitVertex();
    EndPrimitive();
}

void main()
{
    drawNormalLine(0);
    drawNormalLine(1);
    drawNormalLine(2);
    
}
