#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 7) out;

in vec3 color[];

out vec3 fcolor;

void genHouse(vec4 position)
{
    float offset = .1;
    
    fcolor = color[0];
    gl_Position = position + vec4(-offset, offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(-offset, -offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(offset, offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(offset, -offset, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
    
    
    gl_Position = position + vec4(-1.5*offset, offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(1.5*offset, offset, 0.0, 0.0);
    EmitVertex();
    
    fcolor = vec3(1.0);
    gl_Position = position + vec4(0.0, 2*offset, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
}



void main()
{
    genHouse(gl_in[0].gl_Position);
}
