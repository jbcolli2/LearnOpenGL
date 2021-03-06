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
    m_objShader = Shader(SHADER_FOLDER + "MVPNormalUV.vert", SHADER_FOLDER + "Texture.frag");
    m_objShader.makeProgram();
    m_effectShader = Shader(SHADER_FOLDER + "MVPNormalUVInstVert.glsl", SHADER_FOLDER + "Texture.frag");
    m_effectShader.makeProgram();
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
    std::string planetPath = ASSET_FOLDER + "planet/planet.obj";
    std::string rockPath = ASSET_FOLDER + "rock/rock.obj";
    
    std::vector<std::string> skyboxPath = {
        ASSET_FOLDER + "skybox/right.jpg",
        ASSET_FOLDER + "skybox/left.jpg",
        ASSET_FOLDER + "skybox/top.jpg",
        ASSET_FOLDER + "skybox/bottom.jpg",
        ASSET_FOLDER + "skybox/front.jpg",
        ASSET_FOLDER + "skybox/back.jpg"
    };
    
    
    
    
//    m_shapes.push_back((std::make_unique<Cube>(marblePath)));
//    m_shapes[0]->m_transform.position = glm::vec3(1.f, .5f, -4.f);
    
    m_models.push_back(std::make_unique<Model>(planetPath.c_str()));
    m_models.back()->m_transform.scale = glm::vec3(.5f);
    m_models.push_back(std::make_unique<Model>(rockPath.c_str()));
    m_models.back()->m_transform.scale = glm::vec3(asteroidScale);
    
    
    m_models[1]->BindVertexArray();
    fillInstArray();
    glBindVertexArray(0);
    
    
    for(int ii = 0; ii < numAsteroids; ++ii)
    {
        randx.push_back(glm::linearRand(-posRand, posRand));
        randy.push_back(glm::linearRand(-posRand, posRand));
        randz.push_back(glm::linearRand(-posRand, posRand));
    }
    

//    stbi_set_flip_vertically_on_load(false);
//    m_skybox = Skybox(skyboxPath);

}



void Scene::fillInstArray()
{
    std::vector<Inst4f> modelCol1(numAsteroids), modelCol2(numAsteroids),
        modelCol3(numAsteroids), modelCol4(numAsteroids);
    
    glm::mat4 id{1.f}, model;
    glm::vec3 translation;
    
    for(int ii = 0; ii < numAsteroids; ++ii)
    {
        translation = glm::vec3(asteroidRadius*glm::cos(ii*6.28/numAsteroids) + glm::linearRand(-posRand, posRand),
                                glm::linearRand(-posRand, posRand),
                                asteroidRadius*glm::sin(ii*6.28/numAsteroids) + glm::linearRand(-posRand, posRand));
        model = glm::translate(id, translation);
        model = glm::scale(model, glm::vec3(asteroidScale));
       
        modelCol1[ii] = Inst4f(model[0].x, model[0].y, model[0].z, model[0].w);
        modelCol2[ii] = Inst4f(model[1].x, model[1].y, model[1].z, model[1].w);
        modelCol3[ii] = Inst4f(model[2].x, model[2].y, model[2].z, model[2].w);
        modelCol4[ii] = Inst4f(model[3].x, model[3].y, model[3].z, model[3].w);
    }
    
    inst_vbo[0] = loadVBOData(modelCol1, 3);
    inst_vbo[1] = loadVBOData(modelCol2, 4);
    inst_vbo[2] = loadVBOData(modelCol3, 5);
    inst_vbo[3] = loadVBOData(modelCol4, 6);
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
    m_cam = Camera(fov, float(m_width)/float(m_height), nearField, farField, glm::vec3(0.f, 6.f, 23.f));



    
    setupShapes();


    
    
    
    // Uniform buffer for view/proj
    glGenBuffers(1, &m_uboVP);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboVP);
    glBufferData(GL_UNIFORM_BUFFER, 16*8, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uboVP);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    
    
    
    
   

    
    
    
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
    m_proj = m_cam.getProjMatrix();
    m_view = glm::mat4(glm::mat3(m_cam.getViewMatrix()));
    updateVP(m_skyboxShader);
    
    m_view = m_cam.getViewMatrix();
    glEnable(GL_DEPTH_TEST);
    
    clearBuffers();
    
    ImGui::Begin("Display Info");
    ImGui::Text(std::to_string(m_cam.m_camPos.x).c_str());
    if(ImGui::InputInt("Number of Asteroids", &numAsteroids))
    {
        m_models[1]->BindVertexArray();
        fillInstArray();
        glBindVertexArray(0);
    }
    if(ImGui::InputFloat("Radius", &asteroidRadius))
    {
        m_models[1]->BindVertexArray();
        fillInstArray();
        glBindVertexArray(0);
    }
    if(ImGui::InputFloat("Scale", &asteroidScale))
    {
        m_models[1]->BindVertexArray();
        fillInstArray();
        glBindVertexArray(0);
    }
    ImGui::End();
    
    // Set VP uniform buffer data
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboVP);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    

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

//    m_skyboxShader.useProgram();
//    m_skyboxShader.setUniformTex("skybox", 10);
//    m_skybox.Draw(m_skyboxShader);
    
    m_objShader.useProgram();
    for(auto& shape: m_shapes)
    {
        shape->Draw(m_objShader);
    }
    m_models[0]->Draw(m_objShader);
    
    m_effectShader.useProgram();
    m_models[1]->Draw(m_effectShader, numAsteroids);
    
    

    
    
    
    

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
        
        if(keyEvent.key == GLFW_KEY_M && keyEvent.action == GLFW_PRESS)
        {
            m_mouseCam = !m_mouseCam;
            if(m_mouseCam)
            {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else
            {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        
//        if(keyEvent.key == GLFW_KEY_SPACE && keyEvent.action == GLFW_PRESS )
//        {
//            m_shapes[m_selectedShape]->m_outlined = false;
//            m_selectedShape = (m_selectedShape + 1) % m_shapes.size();
//            m_shapes[m_selectedShape]->m_outlined = true;
//        }
        
//        if(keyEvent.key == GLFW_KEY_0 && keyEvent.action == GLFW_PRESS)
//        {
//            selectCommandIndex = 0;
//
//        }
//        if(keyEvent.key == GLFW_KEY_1 && keyEvent.action == GLFW_PRESS)
//        {
//            m_ptLight[m_selectedLight].setOutline(false);
//            selectCommandIndex = 1;
//            m_shapes[m_selectedShape]->m_outlined = true;
//        }
//        if(keyEvent.key == GLFW_KEY_2 && keyEvent.action == GLFW_PRESS)
//        {
//            m_shapes[m_selectedShape]->m_outlined = false;
//            selectCommandIndex = 2;
//            m_ptLight[m_selectedLight].setOutline(true);
//        }
//        if(keyEvent.key == GLFW_KEY_TAB && keyEvent.action == GLFW_PRESS)
//        {
//            m_selectCommands[selectCommandIndex]->changeSelect();
//        }
        
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
    
    if(m_mouseCam)
    {
        m_cam.turnYaw(xoffset);
        m_cam.turnPitch(yoffset);
    }
    
}





void Scene::scroll_callback(GLFWwindow* window, double xInc, double yInc)
{
    float fov_inc = (float)yInc;
            
    m_cam.incFOV(fov_inc);
}





