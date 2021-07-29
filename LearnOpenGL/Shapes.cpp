//
//  Shapes.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#include <stdio.h>


#include "Shapes.hpp"


Triangle::Triangle(float verts[9])
{
    for(int ii = 0; ii < 9; ii++)
    {
        this->verts[ii] = verts[ii];
    }
    
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->verts), this->verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    glEnableVertexAttribArray(0);
    //********* VAO **************
    
}



void Triangle::draw()
{
    glBindVertexArray(VAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}










Square::Square(float verts[12], bool clockwise)
{
    for(int ii = 0; ii < 12; ii++)
    {
        this->verts[ii] = verts[ii];
    }
    
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;
    
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->verts), this->verts, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    glEnableVertexAttribArray(0);
    //********* VAO **************
    
}



void Square::draw()
{
    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

