#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 6) out;





void main()
{
    float width = .02;
    vec4 normal = vec4(gl_in[0].gl_Position.y - gl_in[1].gl_Position.y,
                  gl_in[1].gl_Position.x - gl_in[0].gl_Position.x, 0.0, 0.0);
    normal = normalize(normal);
    
    gl_Position = gl_in[0].gl_Position + 0.5*width*normal;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position - 0.5*width*normal;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position - 0.5*width*normal;
    EmitVertex();
    EndPrimitive();
    
    gl_Position = gl_in[1].gl_Position + 0.5*width*normal;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + 0.5*width*normal;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position - 0.5*width*normal;
    EmitVertex();
    EndPrimitive();
    
    
}
