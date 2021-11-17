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

void Scene::setupShaders()
{
    ubo1Shader = Shader(SHADER_FOLDER + "UBOTestVert.glsl", SHADER_FOLDER + "UBOTestFrag1.glsl");
    ubo1Shader.makeProgram();
    ubo2Shader = Shader(SHADER_FOLDER + "UBOTestVert.glsl", SHADER_FOLDER + "UBOTestFrag2.glsl");
    ubo2Shader.makeProgram();
    ubo3Shader = Shader(SHADER_FOLDER + "UBOTestVert.glsl", SHADER_FOLDER + "UBOTestFrag3.glsl");
    ubo3Shader.makeProgram();
    ubo4Shader = Shader(SHADER_FOLDER + "UBOTestVert.glsl", SHADER_FOLDER + "UBOTestFrag4.glsl");
    ubo4Shader.makeProgram();
    m_skyboxShader = Shader(SHADER_FOLDER + "SkyboxVert.vert", SHADER_FOLDER + "SkyboxFrag.frag");
    m_skyboxShader.makeProgram();
    m_debugShader = Shader(SHADER_FOLDER + "DebugVert.vert", SHADER_FOLDER + "DebugFrag.frag");
    m_debugShader.makeProgram();
    Shader::solidShader = Shader(SHADER_FOLDER + "MVPNormalUV.vert", SHADER_FOLDER + "SolidColor.frag");
    Shader::solidShader.makeProgram();
}


void Scene::setupLights()
{
    float ambient = .15;
    
    m_dirLight = DirLight(SHADER_FOLDER + "MVP.vert", SHADER_FOLDER + "SolidColor.frag");
    m_dirLight.direction = glm::vec3(-1.f, -1.f, -1.f);
    m_dirLight.diffuse = glm::vec3(0.5f);
    m_dirLight.specular = glm::vec3(.5f);
    m_dirLight.ambient = glm::vec3(ambient);
    m_dirLight.structName = "dirLight";
    for(int ii = 0; ii < 4; ++ii)
    {
        PointLight temp{SHADER_FOLDER + "MVP.vert", SHADER_FOLDER + "SolidColor.frag", m_lightPos[ii]};
        m_ptLight.push_back(temp);
        m_ptLight[ii].structName = "ptLights";
        m_ptLight[ii].diffuse = glm::vec3(0.5f);
        m_ptLight[ii].ambient = glm::vec3(ambient);
        m_ptLight[ii].linAtten = 0.17f;
        m_ptLight[ii].quadAtten = 0.07f;
        
    }
    m_spotLight = SpotLight(SHADER_FOLDER + "MVP.vert", SHADER_FOLDER + "SolidColor.frag");
    m_spotLight.structName = "spotLight";
    m_spotLight.ambient = glm::vec3(ambient);
    m_spotLight.specular = glm::vec3(.8f);
    m_spotLight.diffuse = glm::vec3(.7f);
    m_spotLight.innerCutoff = glm::cos(glm::radians(45.f));
    m_spotLight.outerCutoff = glm::cos(glm::radians(48.f));

}








void Scene::setupShapes()
{
    stbi_set_flip_vertically_on_load(true);
    std::vector<std::string> metalPath = {ASSET_FOLDER+"metal.png"};
    std::vector<std::string> marblePath = {ASSET_FOLDER+"marble.jpg"};
    std::vector<std::string> containerPath = {ASSET_FOLDER+"container2.png"};
    std::vector<std::string> grassPath = {ASSET_FOLDER+"grass.png"};
    std::vector<std::string> windowPath = {ASSET_FOLDER+"blending_transparent_window.png"};
    std::string backpackPath = ASSET_FOLDER + "backpack/backpack.obj";
    std::string glassPath = ASSET_FOLDER + "cocktail glass/cocktail glass.obj";
    std::vector<std::string> skyboxPath = {
        ASSET_FOLDER + "skybox/right.jpg",
        ASSET_FOLDER + "skybox/left.jpg",
        ASSET_FOLDER + "skybox/top.jpg",
        ASSET_FOLDER + "skybox/bottom.jpg",
        ASSET_FOLDER + "skybox/front.jpg",
        ASSET_FOLDER + "skybox/back.jpg"
    };
    
    m_shapes.push_back(std::make_unique<Cube>(marblePath));
    m_shapes.back()->m_transform.position = glm::vec3(0.f, -2.f, -5.f);
    
    m_shapes.push_back(std::make_unique<Cube>(marblePath));
    m_shapes.back()->m_transform.position = glm::vec3(0.f, 2.f, -5.f);
    
    m_shapes.push_back(std::make_unique<Cube>(marblePath));
    m_shapes.back()->m_transform.position = glm::vec3(2.f, 0.f, -5.f);
    
    m_shapes.push_back(std::make_unique<Cube>(marblePath));
    m_shapes.back()->m_transform.position = glm::vec3(-2.f, 0.f, -5.f);
    
//    m_glass = Model(glassPath.c_str());
//    m_glass.m_transform.scale = glm::vec3(.1f);
//    m_glass.m_transform.position.z = -2.f;
    
//    m_backpack = Model(backpackPath.c_str());
//    m_backpack.m_transform.scale = glm::vec3(.2f);
//    m_backpack.m_transform.position.z = -2.f;

    stbi_set_flip_vertically_on_load(false);
    m_skybox = Skybox(skyboxPath);

}










void Scene::setupFBO()
{
    //*********  Plane with framebuffer texture ************//
    std::vector<Vert3x2f> fbo_vert = {
        Vert3x2f(-1.f, 1.0f, 0.0f, 0.f, 1.f),
        Vert3x2f(-1.f, -1.f, 0.0f, 0.f, 0.f),
        Vert3x2f(1.f, -1.f, 0.0f, 1.f, 0.f),

        Vert3x2f(-1.f, 1.f, 0.0f, 0.f, 1.f),
        Vert3x2f(1.f, -1.f, 0.0f, 1.f, 0.f),
        Vert3x2f(1.f, 1.f, 0.0f, 1.f, 1.f)
    };
    

    glGenVertexArrays(1, &m_fbo.vao);
    glBindVertexArray(m_fbo.vao);
    m_fbo.vbo = loadVBOData(fbo_vert);
    glBindVertexArray(0);

    
    
    
    
    int fb_width, fb_height;
    glfwGetFramebufferSize(m_window, &fb_width, &fb_height);
    
    //************  Create and setup Framebuffer **********//
    glGenFramebuffers(1, &m_fbo.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo.fbo);
    
    glGenTextures(1, &m_fbo.tbo);
    glBindTexture(GL_TEXTURE_2D, m_fbo.tbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb_width, fb_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo.tbo, 0);
    
    glGenRenderbuffers(1, &m_fbo.rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fb_width, fb_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo.rbo);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer NOT complete\n";
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}







//**********  Constructor ****************

Scene::Scene(GLFWwindow* window, int width, int height, float fov,
             float nearField, float farField) : m_window(window), m_firstMouse(true), m_width(width),
    m_height(height), m_nearField(nearField), m_farField(farField), m_fov(fov)
{
    
    Scene::GLFWCallbackWrapper::setScene(this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, Scene::GLFWCallbackWrapper::keyCallback);
    glfwSetFramebufferSizeCallback(window, Scene::GLFWCallbackWrapper::frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, Scene::GLFWCallbackWrapper::mousePosCallback);
    glfwSetScrollCallback(window, Scene::GLFWCallbackWrapper::scrollCallback);

    
    
    
    setupShaders();
    
    m_selectCommands.push_back(std::make_unique<NoSelect>(this));
    m_selectCommands.push_back(std::make_unique<ShapeSelect>(this));
    m_selectCommands.push_back(std::make_unique<LightSelect>(this));
    selectCommandIndex = 0;
    
   
    
    
    
    //  Setup the camera
    m_cam = Camera(fov, float(m_width)/float(m_height), nearField, farField);



    
    setupShapes();


    
    
    
    /*
     * Uniform buffer testing
     */
    glGenBuffers(1, &uboVP);
    glBindBuffer(GL_UNIFORM_BUFFER, uboVP);
    glBufferData(GL_UNIFORM_BUFFER, 16*8, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboVP);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glGenBuffers(1, &uboColor);
    glBindBuffer(GL_UNIFORM_BUFFER, uboColor);
    glBufferData(GL_UNIFORM_BUFFER, 4*16, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboColor);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    
    // Bind Uniform blocks in relevant programs
    ubo1Shader.bindUniformBlock("VP", 0);
    ubo1Shader.bindUniformBlock("colors", 1);
    ubo2Shader.bindUniformBlock("VP", 0);
    ubo2Shader.bindUniformBlock("colors", 1);
    ubo3Shader.bindUniformBlock("VP", 0);
    ubo3Shader.bindUniformBlock("colors", 1);
    ubo4Shader.bindUniformBlock("VP", 0);
    ubo4Shader.bindUniformBlock("colors", 1);
    
    
    glBindBuffer(GL_UNIFORM_BUFFER, uboColor);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(glm::vec3(1.f, 0.f, 0.f)));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(glm::vec3(0.f, 1.f, 0.f)));
    glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(glm::vec3(0.f, 0.f, 1.f)));
    glBufferSubData(GL_UNIFORM_BUFFER, 3*sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(glm::vec3(1.f, 0.f, 1.f)));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    /*
     * End uniform buffer testing
     */
    

    
    
    
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
      
}














//*******************************************
//********  draw()  *******************
//********************************************//
void Scene::draw()
{
    m_view = glm::mat4(glm::mat3(m_cam.getViewMatrix()));
    m_proj = m_cam.getProjMatrix();
    
    
    updateVP(m_skyboxShader);
    m_skyboxShader.setUniform1i("skybox", 10);
    m_view = m_cam.getViewMatrix();
    
    /*
     * Uniform buffer testing
     */
    glBindBuffer(GL_UNIFORM_BUFFER, uboVP);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    /*
     * End uniform buffer testing
     */

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    clearBuffers();
    
    
    
    updateVP(Shader::solidShader);
//    updateVP(m_objShader);
    m_objShader.setUniform1i("skybox", 10);
    m_objShader.setUniform3f("camPosition", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);

//    updateLightUniforms();
    drawObjects();
    
    
    

    updateLights();
   
    
}















void Scene::clearBuffers()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void Scene::updateVP(Shader shader)
{
    shader.useProgram();
    shader.setUniformMatrix4f("view", m_view);
    shader.setUniformMatrix4f("proj", m_proj);
}




void Scene::updateLightUniforms()
{
    m_dirLight.setUniformDirLight(m_objShader);
//    for(int ii = 0; ii < 4; ++ii)
//    {
//        m_ptLight[ii].setUniformPtLight(m_objShader, ii);
//    }
    m_spotLight.setUniformSpotLight(m_objShader);
}




void Scene::drawObjects()
{

    m_skyboxShader.useProgram();
    m_skybox.Draw(m_skyboxShader);
    
//    m_objShader.useProgram();
//    m_glass.Draw(m_objShader);
//
//    for(auto& shape: m_shapes)
//    {
//        shape->Draw(m_objShader);
//    }
    
    /*
     * Uniform buffer test
     */
    ubo1Shader.useProgram();
    m_shapes[0]->Draw(ubo1Shader);
    ubo1Shader.useProgram();
    m_shapes[1]->Draw(ubo2Shader);
    ubo1Shader.useProgram();
    m_shapes[2]->Draw(ubo3Shader);
    ubo1Shader.useProgram();
    m_shapes[3]->Draw(ubo4Shader);
    
    
    /*
     * End uniform buffer test
     */
    

}




void Scene::drawFBOQuad()
{
    m_fboShader.useProgram();
    glm::mat4 model = glm::mat4(1.0f);
    m_fboShader.setUniformMatrix4f("model", model);
    m_fboShader.setUniformMatrix4f("view", glm::mat4(1.f));
    m_fboShader.setUniformMatrix4f("proj", glm::mat4(1.f));
    glBindVertexArray(m_fbo.vao);
    glBindTexture(GL_TEXTURE_2D, m_fbo.tbo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



void Scene::updateLights()
{
    //    for(int ii = 0; ii < m_ptLight.size(); ++ii)
    //    {
    //        m_ptLight[ii].draw(m_view, m_proj);
    //    }
        m_spotLight.position = m_cam.getPosition();
        m_spotLight.direction = m_cam.getDirection();

}









//*****************************************
/*            Input
 ********************************************/



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
        m_skybox.RotateLeft(camMovement*.5f);

    }
    if(inputHandler->m_keyPress[GLFW_KEY_D])
    {
        m_cam.moveRight(camMovement);
        m_skybox.RotateRight(camMovement*.5f);
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





