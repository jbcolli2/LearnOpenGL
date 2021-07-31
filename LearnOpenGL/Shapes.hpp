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

#include "OpenGLUtil.hpp"









class Shape
{
public:
    Shape() {};
    
    void virtual draw() = 0;
};



template <class T>
class Triangle : public Shape
{
    unsigned int VAO;
    std::vector<T> verts;

    
public:
    Triangle(std::vector<T> vert);
    
//    Triangle(std::vector<VertColorData> verts);
    
    void virtual draw() override;
};



template <class T>
class Square : public Shape
{
    unsigned int VAO;
    std::vector<T> verts;
    std::vector<unsigned int> indices = {0,1,2, 0, 2,3};

    
public:
    Square(std::vector<T> verts, bool clockwise = true);
    
    void virtual draw() override;
};






















template <class T>
Triangle<T>::Triangle(std::vector<T> vert)
{
    verts = vert;
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    loadVBOData(verts);

    rglVertexAttribPointer(vert[0]);
    
    
    glEnableVertexAttribArray(0);
    //********* VAO **************
}





template <class T>
void Triangle<T>::draw()
{
    glBindVertexArray(VAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}






template <class T>
Square<T>::Square(std::vector<T> vert, bool clockwise)
{
    this->verts = vert;
    
    
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    loadVBOData(verts);
    
    loadEBOData(indices);

    rglVertexAttribPointer(vert[0]);
    
    
    
    //********* VAO **************
    
}



template <class T>
void Square<T>::draw()
{
    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}




#endif /* Shapes_h */
