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

struct Inst3f
{
    float x,y,z;
    
    Inst3f() : x(0.f), y(0.f), z(0.f) {};
    Inst3f(float x, float y, float z) : x(x), y(y), z(z) {};
};

struct Inst4f
{
    float x,y,z,w;
    
    Inst4f() : x(0.f), y(0.f), z(0.f), w(0.f) {};
    Inst4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
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


struct Vert3x3x2x3x3f
{
    float x,y,z;
    float r,g,b;
    float s,t;
    float x1,y1,z1, x2, y2, z2;
    
    Vert3x3x2x3x3f() : x(0.f), y(0.f), z(0.f), r(0.f), g(0.f), b(0.f), s(0.f), t(0.f),
    x1(0.f), y1(0.f), z1(0.f), x2(0.f), y2(0.f), z2(0.f){};
    Vert3x3x2x3x3f(float x, float y, float z, float r, float g, float b, float s, float t,
               float x1, float y1, float z1, float x2, float y2, float z2) :
    x(x), y(y), z(z), r(r), g(g), b(b), s(s), t(t), x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2) {};
};



struct Vert3x_x_f
{
    float x,y,z;
    
    Vert3x_x_f() :x(0.f), y(0.f), z(0.f) {};
    Vert3x_x_f(float x, float y, float z) : x(x), y(y), z(z) {};
};


struct Vert2x2f
{
    float x,y,u,v;
    
    Vert2x2f() : x(0.f), y(0.f), u(0.f), v(0.f) {};
    Vert2x2f(float x, float y, float u, float v) : x(x), y(y), u(u), v(v) {};
};

struct Vert3x2f
{
    float x,y,z,u,v;
    
    Vert3x2f() : x(0.f), y(0.f), z(0.0), u(0.f), v(0.f) {};
    Vert3x2f(float x, float y, float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {};
};




#endif /* VertexData_h */
