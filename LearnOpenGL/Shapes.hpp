//
//  Shapes.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#ifndef Shapes_h
#define Shapes_h
#define GL_SILENCE_DEPRECATION

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>




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





class Shape
{
public:
    Shape() {};
    
    void virtual draw() = 0;
};

class Triangle : public Shape
{
    unsigned int VAO;
    float* verts;

    
public:
    Triangle(std::vector<VertData> verts);
    Triangle(std::vector<VertColorData> verts);
    
    void virtual draw() override;
};




class Square : public Shape
{
    unsigned int VAO;
    std::vector<float> verts;
    std::vector<unsigned int> indices = {0,1,2, 0, 2,3};

    
public:
    Square(std::vector<VertData> verts, bool clockwise = true);
    
    void virtual draw() override;
};




#endif /* Shapes_h */
