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
#include <algorithm>

#include "Scene.hpp"










void framebuffer_size_callback(GLFWwindow* window, int width, int height);









int main(int argc, const char * argv[]) {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    
    int width = 800;
    int height = 600;
    
    
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create glfw window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    
    
    
    glUniformBlockBinding( 1,1,1);
    
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    
    
    
    
    
    Scene scene(window, width, height);
    
    float currentFrame = 0.f;
    float lastFrame = 0.f;
    float deltaTime = 0.f;

    //Main loop
    while(!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        scene.processInput(deltaTime);
        
        
        
        
        
        scene.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    glfwTerminate();
    return 0;
    
}










void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);
};
