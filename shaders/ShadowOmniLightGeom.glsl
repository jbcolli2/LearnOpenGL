#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

out vec4 FragPos;

uniform mat4 lightVPCube[6];


void main()
{
    for(int side = 0; side < 6; side++)
    {
        gl_Layer = side;
        for(int vert = 0; vert < 3; vert++)
        {
            FragPos = gl_in[vert].gl_Position;
            gl_Position = lightVPCube[side] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
    
}
