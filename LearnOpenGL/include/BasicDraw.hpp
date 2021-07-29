//
//  BasicDraw.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#ifndef BasicDraw_h
#define BasicDraw_h

#include "Shapes.hpp"


void initObjects(unsigned int& VAO)
{
    float vertsTri[] = {
        -0.9f, -.5f, 0.0f,
        -.5f,  -.5f, 0.0f,
        -.5f, .5f,  0.0f
    };
    
    Triangle triangle(vertsTri);
    
//    float vertsSq[] = {
//        .25f, 0.0f, 0.0f,
//        .75f, 0.0f, 0.0f,
//        .75f, .5f, 0.0f,
//        .25f, .5f, 0.0f
//    };
//
//    unsigned int indices[] = {
//        0, 1, 2,
//        0, 2, 3
//    };
//
//
//
//    //******* VBO/VAO   ***************
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    unsigned int VBO, EBO;
//    glGenBuffers(1, &VBO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertsTri), vertsTri, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    glGenBuffers(1, &EBO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertsSq), vertsSq, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
//    glGenBuffers(1, &EBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    //********* VAO **************

}


void draw()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
//    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    
//    glDrawArrays(GL_TRIANGLES, 0, 4);
}



//****** Basic GLFW Stuff ********//

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);
};


#endif /* BasicDraw_h */
