//
//  Shapes.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#ifndef Shapes_h
#define Shapes_h
#define GL_SILENCE_DEPRECATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Shape
{
public:
    Shape() {};
    
    void virtual draw() = 0;
};

class Triangle : public Shape
{
    unsigned int VAO;
    float verts[9];

    
public:
    Triangle(float verts[9]);
    
    void virtual draw() override;
};




class Square : public Shape
{
    unsigned int VAO;
    float verts[12];
    unsigned int indices[6];

    
public:
    Square(float verts[12], bool clockwise = true);
    
    void virtual draw() override;
};




#endif /* Shapes_h */
