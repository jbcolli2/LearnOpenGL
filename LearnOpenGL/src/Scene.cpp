//
//  Scene.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/9/21.
//

#include <stdio.h>

#include "stb_image.h"

#include "Scene.hpp"


Scene::Scene(Shader shader, GLFWwindow* window) : m_shader(shader), m_window(window)
{
    m_pitch = glm::radians(90.f);
    m_yaw = 0.f;
    
    
    //        std::vector<Vert3x3f> vertsTri = {
    //            Vert3x3f(-.9f, -.5f, 0.0f, 1.0f, 0.0f, 0.0f),
    //            Vert3x3f(-.5f, -.5f, 0.0f, 1.0f, 0.0f, 0.0f),
    //            Vert3x3f(-.5f, .5f, 0.0f, 0.0f, 0.0f, 1.0f)
    //        };
    //
    //
    //        m_shapes.emplace_back(std::make_unique<Triangle<Vert3x3f>>(vertsTri));
            
            
            
    //        std::vector<Vert3x3f> vertsSq = {
    //            Vert3x3f(-.5f, -.5f, 0.0f,   1.0f, 0.0f, 0.0f),
    //            Vert3x3f(.5f, -.5f, 0.0f,    0.0f, 1.0f, 0.0f),
    //            Vert3x3f(.5f, .5f, 0.0f,     0.0f, 0.0f, 1.0f),
    //            Vert3x3f(-.5f, .5f, 0.0f,    1.0f, 1.0f, 1.0f)
    //        };
    //
    //
    //        m_shapes.emplace_back(std::make_unique<Square <Vert3x3f> >(vertsSq));
            
            
            
            
            
    //        std::vector<Vert3x3x2f> vertsTri = {
    //            Vert3x3x2f(-.9f, -.5f, 0.0f, 1.0f, 0.0f, 0.0, 0, 0),
    //            Vert3x3x2f(-.5f, -.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0, 0),
    //            Vert3x3x2f(-.5f, .5f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0)
    //        };
    //
    //
    //        m_shapes.emplace_back(std::make_unique<Triangle<Vert3x3x2f>>(vertsTri));
            
    std::vector<Vert3x3x2f> vertsSq = {
        Vert3x3x2f(-.5f, -.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f),
        Vert3x3x2f(.5f, -.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f),
        Vert3x3x2f(.5f, .5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f),
        Vert3x3x2f(-.5f, .5f, 0.0f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f)
    };


    std::string imageFolder = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/include/";

//    m_shapes.emplace_back(std::make_unique<Square <Vert3x3x2f> >(vertsSq));
//    m_shapes[0]->loadTexture(imageFolder + "container.jpeg");
//    stbi_set_flip_vertically_on_load(true);
//    m_shapes[0]->loadTextureAlpha(imageFolder + "awesomeface.png");
    
    
    float width = .5;
    float length = .5;
    float height = .5;
    std::vector<Vert3x3x2f> vertsBox = {
        Vert3x3x2f(-0.5f*width, -0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), // front
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
        Vert3x3x2f(0.5f*width, -0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), // back
        Vert3x3x2f(0.5f*width, 0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
        Vert3x3x2f(0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), //top
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), //bottom
        Vert3x3x2f(0.5f*width, -0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f*width, -0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f*width, -0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), //right
        Vert3x3x2f(0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), //left
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, -0.5f*length, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f)
    };
    m_shapes.emplace_back(std::make_unique<Box <Vert3x3x2f> >(vertsBox));
    m_shapes[0]->loadTexture(imageFolder + "container.jpeg");
    stbi_set_flip_vertically_on_load(true);
    m_shapes[0]->loadTextureAlpha(imageFolder + "awesomeface.png");
            

    
    
    glEnable(GL_DEPTH_TEST);
}






void Scene::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
//    m_yaw = (float)glfwGetTime()*glm::radians(30.f);

    
    // Camera setup
    glm::vec3 cam_p(0.f, 0.f, 0.f);
    glm::vec3 cam_d(glm::sin(m_yaw)*glm::sin(m_pitch), glm::cos(m_pitch), -glm::cos(m_yaw)*glm::sin(m_pitch));
    glm::vec3 target_p = cam_p + cam_d;
    glm::mat4 view = glm::lookAt(cam_p, target_p, glm::vec3(0.f, 1.0f, 0.f));
    
    
    view = m_cam.getViewMatrix();

    m_shader.setUniform1i("tex1", 0);
    m_shader.setUniform1i("tex2", 1);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, 0.f, glm::vec3(1.0f, 0.f, 0.0f));
    model = glm::rotate(model, 0.f, glm::vec3(0.f, 1.f, 0.f));
    
    
    glm::mat4 proj = glm::perspective(glm::radians(45.f), 800.f/600.f, 0.1f, 100.f);
    
    for(auto vec : m_positions)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, vec);
        model = glm::rotate(model, 0.f, glm::vec3(1.0f, 0.f, 0.0f));
        model = glm::rotate(model, 0.f, glm::vec3(0.f, 1.f, 0.f));
        
        

        
        m_shader.setUniformMatrix4f("model", model);
        m_shader.setUniformMatrix4f("view", view);
        m_shader.setUniformMatrix4f("proj", proj);
        
        
        m_shapes[0]->draw();
    }
    
    
    
    
//        for(int ii = 0; ii < m_shapes.size(); ++ii)
//        {
//            m_shapes[ii]->draw();
//        }
}



void Scene::processInput()
{
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(m_window, true);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_cam.turnUp(.05f);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_cam.turnDown(.05f);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_cam.turnLeft(.05f);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_cam.turnRight(.05f);
    }
}
