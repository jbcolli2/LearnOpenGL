//
//  Shapes.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#include <stdio.h>


#include "Shapes.hpp"


Triangle::Triangle(std::vector<VertData> vert)
{
//    this->verts = verts;
    
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vert.size()*sizeof(VertData), &vert[0], GL_STATIC_DRAW);

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










Square::Square(std::vector<VertData> vert, bool clockwise)
{
//    this->verts = vert;
    
    
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(VertData), &vert.front(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);

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

