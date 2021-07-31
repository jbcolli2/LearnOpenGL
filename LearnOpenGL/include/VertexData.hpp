//
//  VertexData.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/30/21.
//

#ifndef VertexData_h
#define VertexData_h


struct VertData
{
    float x,y,z;
    
    VertData(float x, float y, float z) : x(x), y(y), z(z) {};
};

struct VertColorData
{
    float x,y,z;
    float r,g,b;
    
    VertColorData(float x, float y, float z, float r, float g, float b) :
    x(x), y(y), z(z), r(r), g(g), b(b) {};
};

#endif /* VertexData_h */
