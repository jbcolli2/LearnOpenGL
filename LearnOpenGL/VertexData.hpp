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
    
    Vert3f() : x(0.f), y(0.f), z(0.f) {};
    Vert3f(float x, float y, float z) : x(x), y(y), z(z) {};
};

struct Vert3x3f
{
    float x,y,z;
    float r,g,b;
    
    Vert3x3f() : x(0.f), y(0.f), z(0.f), r(0.f), g(0.f), b(0.f) {};
    Vert3x3f(float x, float y, float z, float r, float g, float b) :
    x(x), y(y), z(z), r(r), g(g), b(b) {};
};




struct Vert3x3x2f
{
    float x,y,z;
    float r,g,b;
    float s,t;
    
    Vert3x3x2f() : x(0.f), y(0.f), z(0.f), r(0.f), g(0.f), b(0.f), s(0.f), t(0.f) {};
    Vert3x3x2f(float x, float y, float z, float r, float g, float b, float s, float t) :
    x(x), y(y), z(z), r(r), g(g), b(b), s(s), t(t) {};
};



struct Vert3x_x_f
{
    float x,y,z;
    
    Vert3x_x_f() :x(0.f), y(0.f), z(0.f) {};
    Vert3x_x_f(float x, float y, float z) : x(x), y(y), z(z) {};
};


#endif /* VertexData_h */