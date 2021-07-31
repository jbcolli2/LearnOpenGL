//
//  OpenGLUtil.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/30/21.
//

#ifndef OpenGLUtil_h
#define OpenGLUtil_h


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexData.hpp"




void rglVertexAttribPointer(VertData v)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
};


void rglVertexAttribPointer(VertColorData v)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
};






template <class T>
void loadVBOData(std::vector<T> vec)
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vec.size()*sizeof(T), &vec[0], GL_STATIC_DRAW);
}


template <class T>
void loadEBOData(std::vector<T> vec)
{
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vec.size() * sizeof(T), &vec.front(), GL_STATIC_DRAW);
}

#endif /* OpenGLUtil_h */
