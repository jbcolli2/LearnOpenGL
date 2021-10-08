//
//  Scene.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/9/21.
//

#include <stdio.h>

#include "stb_image.h"

#include "Scene.hpp"
#include "Input.hpp"










Scene* Scene::GLFWCallbackWrapper::m_scene = nullptr;

Scene::Scene(GLFWwindow* window, int width, int height, float fov,
             float nearField, float farField) : m_window(window), m_firstMouse(true), m_width(width),
    m_height(height)
{
    
    Scene::GLFWCallbackWrapper::setScene(this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, Scene::GLFWCallbackWrapper::keyCallback);
    glfwSetFramebufferSizeCallback(window, Scene::GLFWCallbackWrapper::frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, Scene::GLFWCallbackWrapper::mousePosCallback);
    glfwSetScrollCallback(window, Scene::GLFWCallbackWrapper::scrollCallback);

    
    
    
    // Load and compile the shaders
    m_objShader = Shader(SHADER_FOLDER + "MVPNormalUV.vert", SHADER_FOLDER + "Texture.frag");
    m_objShader.makeProgram();
    m_fboShader = Shader(SHADER_FOLDER + "FBOVert.vert", SHADER_FOLDER + "FBOFrag.frag");
    m_fboShader.makeProgram();
    m_debugShader = Shader(SHADER_FOLDER + "DebugVert.vert", SHADER_FOLDER + "DebugFrag.frag");
    m_debugShader.makeProgram();
    Shader::solidShader = Shader(SHADER_FOLDER + "MVPNormalUV.vert", SHADER_FOLDER + "SolidColor.frag");
    Shader::solidShader.makeProgram();
    
    
    m_selectCommands.push_back(std::make_unique<NoSelect>(this));
    m_selectCommands.push_back(std::make_unique<ShapeSelect>(this));
    m_selectCommands.push_back(std::make_unique<LightSelect>(this));
    selectCommandIndex = 0;
    
    // Create Light object
    glm::vec3 diffLight{1.f};
    m_dirLight = DirLight(SHADER_FOLDER + "MVP.vert", SHADER_FOLDER + "SolidColor.frag");
    m_dirLight.direction = glm::vec3(-1.f, -1.f, -1.f);
    m_dirLight.diffuse = glm::vec3(0.5f);
    m_dirLight.specular = glm::vec3(.5f);
    m_dirLight.ambient = glm::vec3(.15f);
    m_dirLight.structName = "dirLight";
    for(int ii = 0; ii < 4; ++ii)
    {
        PointLight temp{SHADER_FOLDER + "MVP.vert", SHADER_FOLDER + "SolidColor.frag", m_lightPos[ii]};
        m_ptLight.push_back(temp);
        m_ptLight[ii].structName = "ptLights";
        m_ptLight[ii].diffuse = glm::vec3(0.5f);
        m_ptLight[ii].ambient = m_ptLight[ii].diffuse*glm::vec3(0.05f);
        m_ptLight[ii].linAtten = 0.17f;
        m_ptLight[ii].quadAtten = 0.07f;
        
    }
    m_spotLight = SpotLight(SHADER_FOLDER + "MVP.vert", SHADER_FOLDER + "SolidColor.frag");
    m_spotLight.structName = "spotLight";
    m_spotLight.ambient = glm::vec3(.1f);
    m_spotLight.specular = glm::vec3(.8f);
    m_spotLight.diffuse = glm::vec3(.7f);
    m_spotLight.innerCutoff = glm::cos(glm::radians(45.f));
    m_spotLight.outerCutoff = glm::cos(glm::radians(48.f));
    
    
    
    
    
    //  Setup the camera
    m_cam = Camera(fov, float(m_width)/float(m_height), nearField, farField, glm::vec3(1.6f, .6f, 3.f), -10.f, -10.f);



    
    
    Material boxMat;
    boxMat.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
    boxMat.diffuse = glm::vec3(.07568f, .61424f, .07568f);
    boxMat.specular = glm::vec3(.633f, .727811f, .633f);
    boxMat.shininess = .6f*128.f;
    
    stbi_set_flip_vertically_on_load(true);
    std::vector<std::string> metalPath = {ASSET_FOLDER+"metal.png"};
    std::vector<std::string> marblePath = {ASSET_FOLDER+"marble.jpg"};
    std::vector<std::string> containerPath = {ASSET_FOLDER+"container2.png"};
    std::vector<std::string> grassPath = {ASSET_FOLDER+"grass.png"};
    std::vector<std::string> windowPath = {ASSET_FOLDER+"blending_transparent_window.png"};
    
    
    m_shapes.push_back(std::make_unique<Plane>(marblePath));
    m_shapes.back()->m_transform.position = glm::vec3(0.f, -.5f, -2.f);
    m_shapes.back()->m_transform.scale *= 10.f;
    
    m_shapes.push_back(std::make_unique<Cube>(containerPath));
    m_shapes.back()->m_transform.position = glm::vec3(-1.5f, 0.0f, -3.f);

    m_shapes.push_back(std::make_unique<Cube>(containerPath));
    m_shapes.back()->m_transform.position = glm::vec3(-1.5f, 5.05f, -3.f);

    m_shapes.push_back(std::make_unique<Plane>(windowPath));
    m_shapes.back()->m_transform.position = glm::vec3(2.f, 0.f, -4.49f);
    m_shapes.back()->m_transform.rotation.x = 90.f;

    m_shapes.push_back(std::make_unique<Plane>(windowPath));
    m_shapes.back()->m_transform.position = glm::vec3(-1.5f, 0.f, -2.49f);
    m_shapes.back()->m_transform.rotation.x = 90.f;


    m_shapes.push_back(std::make_unique<Plane>(windowPath));
    m_shapes.back()->m_transform.position = glm::vec3(0.f, 0.f, -2.5f);
    m_shapes.back()->m_transform.rotation.x = 90.f;
    m_shapes.push_back(std::make_unique<Plane>(windowPath));
    m_shapes.back()->m_transform.position = glm::vec3(0.f, 0.f, -1.5f);
    m_shapes.back()->m_transform.rotation.x = 90.f;

    
    
    
    std::vector<Vert2x2f> fbo_vert = {
        Vert2x2f(-1.f, 1.0f, 0.f, 1.f),
        Vert2x2f(-1.f, -1.f, 0.f, 0.f),
        Vert2x2f(1.f, -1.f, 1.f, 0.f),
        
        Vert2x2f(-1.f, 1.f, 0.f, 1.f),
        Vert2x2f(1.f, -1.f, 1.f, 0.f),
        Vert2x2f(1.f, 1.f, 1.f, 1.f)
    };
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    vbo = loadVBOData(fbo_vert);
    glBindVertexArray(0);
    
    

    
    
    
    
    
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    
    glEnable(GL_CULL_FACE);

    
    
    
    int fb_width, fb_height;
    glfwGetFramebufferSize(m_window, &fb_width, &fb_height);
    
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    glGenTextures(1, &tbo);
    glBindTexture(GL_TEXTURE_2D, tbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb_width, fb_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);
    
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fb_width, fb_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer NOT complete\n";
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    
}






void Scene::draw()
{


    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    
    m_view = m_cam.getViewMatrix();
    m_proj = m_cam.getProjMatrix();




    Shader::solidShader.useProgram();
    Shader::solidShader.setUniformMatrix4f("view", m_view);
    Shader::solidShader.setUniformMatrix4f("proj", m_proj);

    m_objShader.useProgram();
    m_objShader.setUniformMatrix4f("view", m_view);
    m_objShader.setUniformMatrix4f("proj", m_proj);

    m_dirLight.setUniformDirLight(m_objShader);
//    for(int ii = 0; ii < 4; ++ii)
//    {
//        m_ptLight[ii].setUniformPtLight(m_objShader, ii);
//    }
    m_spotLight.setUniformSpotLight(m_objShader);
    
    
    
    for(auto& shape: m_shapes)
    {
        shape->Draw(m_objShader);
    }
    
//    m_debugShader.useProgram();
//    std::vector<Vert3f> debugVert = {
//        Vert3f(-1.f, -1.f, 0.f),
//        Vert3f(1.f, -1.f, 0.f),
//        Vert3f(0.f, 1.f, 0.f)
//    };
//
//    unsigned int debugvao;
//    glGenVertexArrays(1, &debugvao);
//    glBindVertexArray(debugvao);
//    loadVBOData(debugVert);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glBindVertexArray(0);
    
    
    
    
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_fboShader.useProgram();
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tbo);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    
    

    
    
//    for(int ii = 0; ii < m_ptLight.size(); ++ii)
//    {
//        m_ptLight[ii].draw(m_view, m_proj);
//    }
    m_spotLight.position = m_cam.getPosition();
    m_spotLight.direction = m_cam.getDirection();
    
}








void Scene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Input::instance().m_unhandledKeys.emplace(key, action, mods);
        
}


void Scene::processInput(float deltaTime)
{
    m_deltaTime = deltaTime;
    float inc = 0.85f*deltaTime;
    float camMovement = m_camSpeed * deltaTime;
    
    Input* inputHandler = &Input::instance();
    auto unhandledKeys = inputHandler->m_unhandledKeys;
    
    //Handle just key down/up events
    while(!inputHandler->m_unhandledKeys.empty())
    {
        KeyEvent keyEvent = inputHandler->m_unhandledKeys.front();
        inputHandler->m_unhandledKeys.pop();
        
        if(keyEvent.key == GLFW_KEY_SPACE && keyEvent.action == GLFW_PRESS )
        {
            m_shapes[m_selectedShape]->m_outlined = false;
            m_selectedShape = (m_selectedShape + 1) % m_shapes.size();
            m_shapes[m_selectedShape]->m_outlined = true;
        }
        
        if(keyEvent.key == GLFW_KEY_0 && keyEvent.action == GLFW_PRESS)
        {
            selectCommandIndex = 0;
            
        }
        if(keyEvent.key == GLFW_KEY_1 && keyEvent.action == GLFW_PRESS)
        {
            m_ptLight[m_selectedLight].setOutline(false);
            selectCommandIndex = 1;
            m_shapes[m_selectedShape]->m_outlined = true;
        }
        if(keyEvent.key == GLFW_KEY_2 && keyEvent.action == GLFW_PRESS)
        {
            m_shapes[m_selectedShape]->m_outlined = false;
            selectCommandIndex = 2;
            m_ptLight[m_selectedLight].setOutline(true);
        }
        if(keyEvent.key == GLFW_KEY_TAB && keyEvent.action == GLFW_PRESS)
        {
            m_selectCommands[selectCommandIndex]->changeSelect();
        }
        
        Input::instance().m_keyPress[keyEvent.key] = keyEvent.action == GLFW_PRESS || keyEvent.action == GLFW_REPEAT;
        
       
        
    }
    
    
    
    if(inputHandler->m_keyPress[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(m_window, true);
    if(inputHandler->m_keyPress[GLFW_KEY_W])
    {
        m_cam.moveForward(camMovement);
    }
    if(inputHandler->m_keyPress[GLFW_KEY_S])
    {
        m_cam.moveBackward(camMovement);
    }
    if(inputHandler->m_keyPress[GLFW_KEY_A])
    {
        m_cam.moveLeft(camMovement);
    }
    if(inputHandler->m_keyPress[GLFW_KEY_D])
    {
        m_cam.moveRight(camMovement);
    }
    if(inputHandler->m_keyPress[GLFW_KEY_E])
    {
        m_cam.moveUp(camMovement);
    }
    if(inputHandler->m_keyPress[GLFW_KEY_Q])
    {
        m_cam.moveDown(camMovement);
    }
    
    
    
    
    if(inputHandler->m_keyPress[GLFW_KEY_I])
    {
        m_selectCommands[selectCommandIndex]->moveForward(inc);
    }
    if(inputHandler->m_keyPress[GLFW_KEY_K])
    {
        m_selectCommands[selectCommandIndex]->moveBackward(inc);
    }
    if(inputHandler->m_keyPress[GLFW_KEY_J])
    {
        m_selectCommands[selectCommandIndex]->moveLeft(inc);
    }
    if(inputHandler->m_keyPress[GLFW_KEY_L])
    {
        m_selectCommands[selectCommandIndex]->moveRight(inc);
    }
    if(inputHandler->m_keyPress[GLFW_KEY_U])
    {
        m_selectCommands[selectCommandIndex]->moveDown(inc);
    }
    if(inputHandler->m_keyPress[GLFW_KEY_O])
    {
        m_selectCommands[selectCommandIndex]->moveUp(inc);
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





