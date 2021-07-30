//
//  main.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/25/21.
//


#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "BasicDraw.hpp"
#include "Shader.hpp"



















int main(int argc, const char * argv[]) {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create glfw window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
        
    Shader shader;
    shader.makeProgram();
    
    Scene scene;
    
//    float vertsTri[] = {
//        -0.9f, -.5f, 0.0f,
//        -.5f,  -.5f, 0.0f,
//        -.5f, .5f,  0.0f
//    };
//
//    Triangle triangle(vertsTri);
//
//
//    float vertsSq[] = {
//            .25f, 0.0f, 0.0f,
//            .75f, 0.0f, 0.0f,
//            .75f, .75f, 0.0f,
//            .25f, .75f, 0.0f
//        };
//    Square square(vertsSq);
    
    
//    unsigned int VAO;
//    initObjects(VAO);
    
    
    //Main loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        shader.useProgram();
        
        
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        scene.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    glfwTerminate();
    return 0;
    
}




