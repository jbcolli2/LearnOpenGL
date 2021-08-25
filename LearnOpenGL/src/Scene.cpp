//
//  Scene.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/9/21.
//

#include <stdio.h>

#include "stb_image.h"

#include "Scene.hpp"


Scene* Scene::GLFWCallbackWrapper::m_scene = nullptr;

Scene::Scene(GLFWwindow* window) : m_window(window), m_firstMouse(true)
{
    
    Scene::GLFWCallbackWrapper::setScene(this);
    
    
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, Scene::GLFWCallbackWrapper::mousePosCallback);
    glfwSetScrollCallback(window, Scene::GLFWCallbackWrapper::scrollCallback);

    
    
    std::string shaderFolder = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/shaders/";
    m_objShader = Shader(shaderFolder + "Phong.vert", shaderFolder + "PhongLightMap.frag");
    m_objShader.makeProgram();
    
    
    m_light = Light(m_lightPos, glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f),
                    shaderFolder + "lightShader.vert", shaderFolder + "lightShader.frag");
//    m_lightShader = Shader(shaderFolder + "lightShader.vert", shaderFolder + "lightShader.frag");
//    m_lightShader.makeProgram();
    
    
    
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
        Vert3x3x2f(-0.5f*width, -0.5f*height, 0.5f*length, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // front
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, 0.5f*length, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, 0.5f*length, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
        Vert3x3x2f(0.5f*width, -0.5f*height, 0.5f*length, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f), // back
        Vert3x3x2f(0.5f*width, 0.5f*height, -0.5f*length, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, -0.5f*length, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
        Vert3x3x2f(0.5f*width, -0.5f*height, -0.5f*length, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, -0.5f*length, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, 0.5f*height, 0.5f*length, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f), //top
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, -0.5f*length, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, -0.5f*length, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, -0.5f*length, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, 0.5f*length, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f), //bottom
        Vert3x3x2f(0.5f*width, -0.5f*height, 0.5f*length, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f*width, -0.5f*height, 0.5f*length, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, -0.5f*height, -0.5f*length, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f*width, -0.5f*height, 0.5f*length, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f), //right
        Vert3x3x2f(0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f*width, -0.5f*height, -0.5f*length, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, -0.5f*length, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(0.5f*width, 0.5f*height, 0.5f*length, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, 0.5f*length, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f), //left
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, 0.5f*length, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f*width, -0.5f*height, -0.5f*length, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, -0.5f*length, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f*width, 0.5f*height, 0.5f*length, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f)
    };
    Material boxMat;
    boxMat.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
    boxMat.diffuse = glm::vec3(.07568f, .61424f, .07568f);
    boxMat.specular = glm::vec3(.633f, .727811f, .633f);
    boxMat.shininess = .6f*128.f;
    
    TextureMaterial boxTex;
    boxTex.ambdiffID = 0;
    boxTex.specID = 1;
    boxTex.shininess = 32;
    
    Box<Vert3x3x2f> box(vertsBox, boxMat, boxTex);
    m_shapes.emplace_back(std::make_unique<Box <Vert3x3x2f> >(box));
    
    
    glActiveTexture(GL_TEXTURE0+2);
//    m_shapes[0]->loadTexture(imageFolder + "container.jpeg");
    stbi_set_flip_vertically_on_load(true);
//    m_shapes[0]->loadTextureAlpha(imageFolder + "awesomeface.png");
    m_shapes[0]->loadTextureAlpha(imageFolder + "container2.png");
    m_shapes[0]->loadTextureAlpha(imageFolder + "container2_specular.png");
    
    
    Box<Vert3x3x2f> light = box;
    m_shapes.emplace_back( std::make_unique<Box <Vert3x3x2f> >(light) );
            

    
    
    glEnable(GL_DEPTH_TEST);
}






void Scene::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    

    m_objShader.useProgram();
    
    m_objShader.setUniform3f("lightColor", 1.f, 1.f, 1.f);
    m_objShader.setUniform3f("lightPos", m_lightPos.x, m_lightPos.y, m_lightPos.z);
    m_objShader.setUniform3f("light.position", m_lightPos.x, m_lightPos.y, m_lightPos.z);
    m_objShader.setUniform3f("light.ambient", m_light.getAmbient().r, m_light.getAmbient().g, m_light.getAmbient().b);
    m_objShader.setUniform3f("light.diffuse", m_light.getDiffuse().r, m_light.getDiffuse().g, m_light.getDiffuse().b);
    m_objShader.setUniform3f("light.specular", m_light.getSpecular().r, m_light.getSpecular().g, m_light.getSpecular().b);
    
    // Material properties
    Material boxMat = m_shapes[0]->getMaterial();
    TextureMaterial boxTex = m_shapes[0]->getTexMaterial();
//    m_objShader.setUniform3f("material.ambient", boxMat.ambient.r, boxMat.ambient.g, boxMat.ambient.b);
//    m_objShader.setUniform3f("material.diffuse", boxMat.diffuse.r, boxMat.diffuse.g, boxMat.diffuse.b);
//    m_objShader.setUniform3f("material.specular", boxMat.specular.r, boxMat.specular.g, boxMat.specular.b);
    m_objShader.setUniform1f("material.shininess", boxTex.shininess);
    m_objShader.setUniform1i("material.diffuse", boxTex.ambdiffID);
    m_objShader.setUniform1i("material.specular", boxTex.specID);
    
    glm::mat4 view = m_cam.getViewMatrix();

    
    
    
    
    glm::mat4 model;
    
    glm::mat4 proj = glm::perspective(glm::radians(m_fov), 800.f/600.f, 0.1f, 100.f);
    
    for(auto vec : m_positions)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, vec);
        
        

        
        m_objShader.setUniformMatrix4f("model", model);
        m_objShader.setUniformMatrix4f("view", view);
        m_objShader.setUniformMatrix4f("proj", proj);
        
        
        m_shapes[0]->draw();
    }
    
    
    m_light.translate(m_lightPos - m_light.getPosition());
    m_light.draw(view, proj);
    
    
}











void Scene::processInput(float deltaTime)
{
    float inc = 0.85f*deltaTime;
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(m_window, true);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_cam.moveForward(inc);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_cam.moveBackward(inc);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_cam.moveLeft(inc);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_cam.moveRight(inc);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_cam.moveUp(inc);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_cam.moveDown(inc);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_I) == GLFW_PRESS)
    {
        m_lightPos.z -= inc;
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_K) == GLFW_PRESS)
    {
        m_lightPos.z += inc;
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_L) == GLFW_PRESS)
    {
        m_lightPos.x += inc;
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_J) == GLFW_PRESS)
    {
        m_lightPos.x -= inc;
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_O) == GLFW_PRESS)
    {
        m_lightPos.y += inc;
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_U) == GLFW_PRESS)
    {
        m_lightPos.y -= inc;
    }
}







void Scene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);
}



void Scene::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(m_firstMouse)
    {
        m_firstMouse = false;
        m_lastMousePosX = xpos;
        m_lastMousePosY = ypos;
    }
    
    float xoffset = xpos - m_lastMousePosX;
    float yoffset = -(ypos - m_lastMousePosY);
    m_lastMousePosX = xpos;
    m_lastMousePosY = ypos;
    
    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;
    
    
    m_cam.turnYaw(xoffset);
    m_cam.turnPitch(yoffset);
    
}





void Scene::scroll_callback(GLFWwindow* window, double xInc, double yInc)
{
    m_fov += (float)yInc;
    
    if(m_fov < 1.f)
        m_fov = 1.f;
    
    if(m_fov > 45.f)
        m_fov = 45.f;
        
}
