//
//  VertexData.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/30/21.
//

#ifndef VertexData_h
#define VertexData_h


struct Vert3f
{
    float x,y,z;
    
    Vert3f(float x, float y, float z) : x(x), y(y), z(z) {};
};

struct Vert3x3f
{
    float x,y,z;
    float r,g,b;
    
    Vert3x3f(float x, float y, float z, float r, float g, float b) :
    x(x), y(y), z(z), r(r), g(g), b(b) {};
};




struct Vert3x3x2f
{
    float x,y,z;
    float r,g,b;
    float s,t;
    
    Vert3x3x2f(float x, float y, float z, float r, float g, float b, float s, float t) :
    x(x), y(y), z(z), r(r), g(g), b(b), s(s), t(t) {};
};


#endif /* VertexData_h */
