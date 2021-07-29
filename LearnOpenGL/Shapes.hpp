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




class Triangle
{
    unsigned int VAO;
    float verts[9];

    
public:
    Triangle(float verts[9]);
    
    void draw();
};




class Square
{
    unsigned int VAO;
    float verts[12];
    unsigned int indices[6];

    
public:
    Square(float verts[12], bool clockwise = true);
    
    void draw();
};




#endif /* Shapes_h */
