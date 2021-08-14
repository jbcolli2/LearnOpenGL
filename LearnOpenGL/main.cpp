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
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "Scene.hpp"
//#include "Shader.hpp"










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
    
    //GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    
    
    
    
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    
    
    
    std::string shaderFolder = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/shaders/";
    Shader shader(shaderFolder + "Chap8.vs", shaderFolder + "Chap8.frag");
    shader.makeProgram();
    
    Scene scene(shader, window);
    
    

    //Main loop
    while(!glfwWindowShouldClose(window))
    {
        scene.processInput();
        
        
        shader.useProgram();
        
        
        
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
