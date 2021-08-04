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

#include "BasicDraw.hpp"
#include "Shader.hpp"










void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);









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
    
    
    
    
    
    
    
    
    
    
    
    std::string shaderFolder = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/shaders/";
    Shader shader(shaderFolder + "Chap7.vs", shaderFolder + "Chap7_Ex1.frag");
    shader.makeProgram();
    
    Scene scene;
    
    float mixValue = 0.0f;
    
    //Main loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        if(glfwGetKey(window, GLFW_KEY_UP))
        {
            mixValue += .01;
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN))
        {
            mixValue -= .01;
        }
        
        mixValue = std::clamp(mixValue, 0.0f, 1.0f);
        shader.useProgram();
        
        shader.setUniform1i("tex1", 0);
        shader.setUniform1i("tex2", 1);
        shader.setUniform1f("mixValue", mixValue);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        scene.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    glfwTerminate();
    return 0;
    
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
