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

Scene::Scene(GLFWwindow* window, int width, int height, float fov,
             float nearField, float farField) : m_window(window), m_firstMouse(true), m_width(width),
    m_height(height)
{
    
    Scene::GLFWCallbackWrapper::setScene(this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, Scene::GLFWCallbackWrapper::frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, Scene::GLFWCallbackWrapper::mousePosCallback);
    glfwSetScrollCallback(window, Scene::GLFWCallbackWrapper::scrollCallback);

    
    
    
    // Load and compile the shaders
    m_objShader = Shader(SHADER_FOLDER + "Basic.vert", SHADER_FOLDER + "Chap22.frag");
    m_objShader.makeProgram();
    
    
    
    
    
    // Create Light object
    glm::vec3 diffLight{1.f};
    m_dirLight = DirLight(SHADER_FOLDER + "lightShader.vert", SHADER_FOLDER + "lightShader.frag");
    m_dirLight.direction = glm::vec3(-1.f, -2.f, -1.f);
    m_dirLight.diffuse = glm::vec3(0.5f);
    m_dirLight.specular = glm::vec3(.5f);
    m_dirLight.ambient = glm::vec3(.15f);
    m_dirLight.structName = "dirLight";
    for(int ii = 0; ii < 4; ++ii)
    {
        m_ptLight[ii] = PointLight(SHADER_FOLDER + "lightShader.vert", SHADER_FOLDER + "lightShader.frag", m_lightPos[ii]);
        m_ptLight[ii].structName = "ptLights";
        m_ptLight[ii].diffuse = glm::vec3(0.5f);
        m_ptLight[ii].ambient = m_ptLight[ii].diffuse*glm::vec3(0.05f);
        m_ptLight[ii].linAtten = 0.17f;
        m_ptLight[ii].quadAtten = 0.07f;
        
    }
    m_spotLight = SpotLight(SHADER_FOLDER + "lightShader.vert", SHADER_FOLDER + "lightShader.frag");
    m_spotLight.structName = "spotLight";
    m_spotLight.specular = glm::vec3(.5f);
    m_spotLight.diffuse = glm::vec3(.4f);
    
    
    
    
    
    //  Setup the camera
    m_cam = Camera(fov, float(m_width)/float(m_height), nearField, farField);



    // The Box
    std::vector<Vert3x3x2f> vertsBox = genBoxVerts<Vert3x3x2f>();
    
    
    Material boxMat;
    boxMat.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
    boxMat.diffuse = glm::vec3(.07568f, .61424f, .07568f);
    boxMat.specular = glm::vec3(.633f, .727811f, .633f);
    boxMat.shininess = .6f*128.f;
    
    
    
    Box<Vert3x3x2f> box(vertsBox, boxMat);
    m_shapes.emplace_back(std::make_unique<Box <Vert3x3x2f> >(box));
    
    
    
    
    
    // The Textures
    stbi_set_flip_vertically_on_load(true);
    m_shapes[0]->loadAmbDiffTexture(IMAGE_FOLDER + "container2.png", 0);
    m_shapes[0]->loadSpecTexture(IMAGE_FOLDER + "container2_specular.png", 1);
    
    
    
    //  The light shape
    Box<Vert3x3x2f> light = box;
    m_shapes.emplace_back( std::make_unique<Box <Vert3x3x2f> >(light) );
    
    std::string backpackPath = ASSET_FOLDER + "backpack/backpack.obj";
    m_backpack = Model(backpackPath.c_str());
            

    
    
    glEnable(GL_DEPTH_TEST);
}






void Scene::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    

    m_objShader.useProgram();
    
    
    m_dirLight.setUniformDirLight(m_objShader);
    for(int ii = 0; ii < 4; ++ii)
    {
        m_ptLight[ii].setUniformPtLight(m_objShader, ii);
    }
    m_spotLight.setUniformSpotLight(m_objShader);
    
    
    
    
    
    
    
    
    m_view = m_cam.getViewMatrix();
    m_proj = m_cam.getProjMatrix();
    m_objShader.setUniformMatrix4f("view", m_view);
    m_objShader.setUniformMatrix4f("proj", m_proj);

    
    
    m_model = ID4;
    m_model = glm::translate(m_model, glm::vec3(0.f, 0.f, -2.f));
    m_model = glm::scale(m_model, glm::vec3(.2f));
    m_objShader.setUniformMatrix4f("model", m_model);
    m_backpack.Draw(m_objShader);
    
    
    for(auto light : m_ptLight)
    {
        light.draw(m_view, m_proj);
    }
    m_spotLight.position = m_cam.getPosition();
    m_spotLight.direction = m_cam.getDirection();
    
    
}











void Scene::processInput(float deltaTime)
{
    float inc = 0.85f*deltaTime;
    float camMovement = m_camSpeed * deltaTime;
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(m_window, true);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_cam.moveForward(camMovement);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_cam.moveBackward(camMovement);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_cam.moveLeft(camMovement);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_cam.moveRight(camMovement);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_cam.moveUp(camMovement);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_cam.moveDown(camMovement);
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_I) == GLFW_PRESS)
    {
        m_ptLight[0].position.z -= inc;
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_K) == GLFW_PRESS)
    {
        m_ptLight[0].position.z += inc;
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_L) == GLFW_PRESS)
    {
        m_ptLight[0].position.x += inc;
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_J) == GLFW_PRESS)
    {
        m_ptLight[0].position.x -= inc;
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_O) == GLFW_PRESS)
    {
        m_ptLight[0].position.y += inc;
    }
    
    if(glfwGetKey(m_window, GLFW_KEY_U) == GLFW_PRESS)
    {
        m_ptLight[0].position.y -= inc;
    }
}







void Scene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    m_width = width;
    m_height = height;
    m_cam.setAspectRatio(float(width)/float(height));
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
    
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;
    
    
    m_cam.turnYaw(xoffset);
    m_cam.turnPitch(yoffset);
    
}





void Scene::scroll_callback(GLFWwindow* window, double xInc, double yInc)
{
    float fov_inc = (float)yInc;
            
    m_cam.incFOV(fov_inc);
}





template <typename VertT>
std::vector<VertT> Scene::genBoxVerts()
{
    std::vector<Vert3x3x2f> vertsBox = {
        Vert3x3x2f(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // front
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
        Vert3x3x2f(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f), // back
        Vert3x3x2f(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
        Vert3x3x2f(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f), //top
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f), //bottom
        Vert3x3x2f(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f), //right
        Vert3x3x2f(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f), //left
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f)
    };
    
    return vertsBox;
}
