#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 Normal[];
in vec2 UV[];
out vec2 TexCoords;

uniform float time;

void main()
{
    float amp = .5*sin(time)*sin(time)*sin(time)*sin(time);
    vec3 normal = normalize(Normal[0]);
    gl_Position = gl_in[0].gl_Position + amp*vec4(normal, 0.0);
    TexCoords = UV[0];
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position + amp*vec4(normal, 0.0);
    TexCoords = UV[1];
    EmitVertex();
    
    gl_Position = gl_in[2].gl_Position + amp*vec4(normal, 0.0);
    TexCoords = UV[2];
    EmitVertex();
    
    
    EndPrimitive();
}
