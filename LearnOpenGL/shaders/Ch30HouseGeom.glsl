#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 9) out;



void genHouse(vec4 position)
{
    float offset = .1;
    
    gl_Position = position + vec4(-offset, offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(-offset, -offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(offset, offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(offset, offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(offset, -offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(-offset, -offset, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
    
    gl_Position = position + vec4(-1.5*offset, offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(1.5*offset, offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = position + vec4(0.0, 1.6*offset, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
}



void main()
{
    genHouse(gl_in[0].gl_Position);
}
