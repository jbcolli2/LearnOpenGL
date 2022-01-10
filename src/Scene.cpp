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
    m_noNormalMap = Shader(SHADER_FOLDER + "MVPNormalUV.vert", SHADER_FOLDER + "LightsTextures.glsl");
    m_noNormalMap.makeProgram();
    m_fragNormalMap = Shader(SHADER_FOLDER + "MVP_TBN_UV_Vert.glsl", SHADER_FOLDER + "LightsTBNNormalTextures.glsl");
    m_fragNormalMap.makeProgram();
    m_vertNormalMap = Shader(SHADER_FOLDER + "MVP_invTBN_UV_Vert.glsl", SHADER_FOLDER + "LightsNormalTextures.glsl");
    m_vertNormalMap.makeProgram();
    
//    m_effectShader = Shader(SHADER_FOLDER + "MVPNormalUVInstVert.glsl", SHADER_FOLDER + "Texture.frag");
//    m_effectShader.makeProgram();
//    m_skyboxShader = Shader(SHADER_FOLDER + "SkyboxVert.vert", SHADER_FOLDER + "SkyboxFrag.frag");
//    m_skyboxShader.makeProgram();
    m_fboShader = Shader(SHADER_FOLDER + "FBOCubeVert.glsl", SHADER_FOLDER + "FBOCubeFrag.glsl");
    m_fboShader.makeProgram();
    
    m_geomNormals = Shader(SHADER_FOLDER + "MVPNormalUV.vert", SHADER_FOLDER + "Ch30NormalLinesGeom.glsl", SHADER_FOLDER + "SolidColor.frag");
    m_geomNormals.makeProgram();

    m_debugShader = Shader(SHADER_FOLDER + "ShadowDirLightVert.glsl", SHADER_FOLDER + "EmptyFrag.glsl");
    m_debugShader.makeProgram();
    Shader::solidShader = Shader(SHADER_FOLDER + "MVPNormalUV.vert", SHADER_FOLDER + "SolidColor.frag");
    Shader::solidShader.makeProgram();
    
    m_currentObjShader = &m_vertNormalMap;
}


void Scene::createLights()
{
    DirLight tempdir{glm::vec3(0.2f, 0.2f, 1.9f)};
    tempdir.setDiffuse(glm::vec3(0.8f));
    tempdir.setSpecular(0.f);
    tempdir.setAmbient(0.05f);
    m_dirLights.push_back(tempdir);
//
//    SpotLight tempspot;
//    tempspot.setAmbient(.15f);
//    tempspot.setDiffuse(glm::vec3(1.f));
//    tempspot.setSpecular(.3f);
//    tempspot.m_position =glm::vec3(-.11f, 2.f, 2.14f);
//    tempspot.m_direction = glm::vec3(0.f, -.64f, -.77f);
//    m_spotLights.push_back(tempspot);
    
    PointLight temppt{glm::vec3(0.2f, .3f, 1.f)};
    temppt.setDiffuse(glm::vec3(.85f));
    temppt.setAmbient(.05f);
    temppt.setSpecular(0.f);
    m_ptLights.push_back(temppt);

}

void Scene::setupLights()
{
    
    m_currentObjShader->useProgram();
    
    m_currentObjShader->setUniform1i("numDirLights", m_dirLights.size());
    m_currentObjShader->setUniform1i("numPtLights", m_ptLights.size());
    m_currentObjShader->setUniform1i("numSpotLights", m_spotLights.size());
    
    for(int ii = 0; ii < m_ptLights.size(); ++ii)
    {
        m_ptLights[ii].setUniformPtLight(*m_currentObjShader, ii);
    }
    for(int ii = 0; ii < m_dirLights.size(); ++ii)
    {
        m_dirLights[ii].setUniformDirLight(*m_currentObjShader, ii);
    }
    for(int ii = 0; ii < m_spotLights.size(); ++ii)
    {
        m_spotLights[ii].setUniformSpotLight(*m_currentObjShader, ii);
    }
    
    m_currentObjShader->setUniform1i("specularMap", 0);
    m_currentObjShader->setUniform1i("phong", m_phong);
    
    m_currentObjShader->stopUseProgram();
}









void Scene::setupShapes()
{
    stbi_set_flip_vertically_on_load(true);
    std::vector<std::string> metalPath = {ASSET_FOLDER+"metal.png"};
    std::vector<std::string> marblePath = {ASSET_FOLDER+"marble.jpg"};
    std::vector<std::string> brickwallPath = {ASSET_FOLDER+"brickwall.jpeg"};
    std::vector<std::string> brickwallNormalPath = {ASSET_FOLDER +"brickwall_normal.jpeg"};
    std::vector<std::string> containerPath = {ASSET_FOLDER+"container2.png"};
    std::vector<std::string> grassPath = {ASSET_FOLDER+"grass.png"};
    std::vector<std::string> woodPath = {ASSET_FOLDER+"wood.png"};
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
    
    
    
    
    m_shapes.push_back((std::make_unique<Cube>(brickwallPath, std::vector<std::string>(), brickwallNormalPath)));
    m_shapes[0]->m_transform.position = glm::vec3(0.f, 0.f, 0.0f);
    m_shapes[0]->m_transform.rotation = glm::vec3(90.f, 0.f, 0.f);
    m_shapes[0]->m_transform.scale = glm::vec3(2.f);
    
    
//    m_models.push_back(std::make_unique<Model>(planetPath.c_str()));
    
    
    

//    stbi_set_flip_vertically_on_load(false);
//    m_skybox = Skybox(skyboxPath);

}









void Scene::SetupFBORender()
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

    
    m_fbo.skybox = Skybox(0);
    
    

}







//*********************************************
//            Scene Constructor
//*********************************************

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
    selectCommandIndex = 2;
    
    createLights();
    setupLights();
    
    
    
    //  Setup the camera
    m_cam = Camera(fov, float(m_width)/float(m_height), nearField, farField, glm::vec3(-.11f, .5f, 2.14f), -10.f);
    
    
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
void Scene::draw(float deltaTime)
{
    m_proj = m_cam.getProjMatrix();
    m_view = glm::mat4(glm::mat3(m_cam.getViewMatrix()));
//    updateVP(m_skyboxShader);
    
    m_view = m_cam.getViewMatrix();
    glEnable(GL_DEPTH_TEST);
    
    clearBuffers();
    
    SetupImGui();
    
    // ********  Uniform buffer for VP matrices  ********** //
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboVP);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    
    
    
    updateLights();

    
    
    // ********  Draw objects and models  ********** //
    m_currentObjShader->useProgram();
    updateLightUniforms();
//    m_shapes[0]->m_transform.rotation.y += 15.f*deltaTime;
    m_shapes[0]->m_transform.rotation.x += m_rotSpeedMult*deltaTime;
    drawObjects(*m_currentObjShader);
    m_currentObjShader->stopUseProgram();
    
    
    
    
    

   
    
}






// ///////////// drawObjects   ////////////////
/**
 \brief Helper subroutine to draw all objects in the scene.  There is no shader calls here, just draw calls.
 */
void Scene::drawObjects(Shader shader)
{

//    m_skyboxShader.useProgram();
//    m_skyboxShader.setUniformTex("skybox", 10);
//    m_skybox.Draw(m_skyboxShader);
    
    for(auto& shape: m_shapes)
    {
        shape->Draw(shader);
    }
}





void Scene::SetupImGui()
{
    ImGui::Begin("Display Info");
    ImGui::Text("Cam Position: (%4.2f, %4.2f, %4.2f)", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
    glm::vec3 dir = m_cam.getDirection();
    ImGui::Text("Cam Direction: (%4.2f, %4.2f, %4.2f)", dir.x, dir.y, dir.z);
    if(ImGui::BeginCombo(("Light " + std::to_string(m_selectedLight)).c_str(), "Light 0", ImGuiComboFlags_NoPreview))
    {
        for(int ii = 0; ii < m_ptLights.size(); ++ii)
        {
            if(ImGui::Selectable(("Light " + std::to_string(ii)).c_str()))
            {
                m_selectCommands[2]->selectIndex(ii);
            }
        }
        
        ImGui::EndCombo();
        
    }
    
    if(ImGui::Checkbox("Phong Lighting", &m_phong))
    {
        m_currentObjShader->useProgram();
        m_currentObjShader->setUniform1i("phong", m_phong);
        m_currentObjShader->stopUseProgram();
    }
    
    if(ImGui::RadioButton("No normal map", m_currentObjShader == &m_noNormalMap))
    {
        m_currentObjShader = &m_noNormalMap;
        setupLights();
    }
    ImGui::SameLine();
    if(ImGui::RadioButton("Normal map(Frag)", m_currentObjShader == &m_fragNormalMap))
    {
        m_currentObjShader = &m_fragNormalMap;
        setupLights();
    }
    ImGui::SameLine();
    if(ImGui::RadioButton("Normal Map(Vert)", m_currentObjShader == &m_vertNormalMap))
    {
        m_currentObjShader = &m_vertNormalMap;
        setupLights();
    }
    
    ImGui::End();
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
    m_currentObjShader->setUniform3f("cameraPos", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
    
    for (int ii = 0; ii < m_dirLights.size(); ++ii)
    {
       m_dirLights[ii].setUniformDir(*m_currentObjShader, ii);
    }
    for (int ii = 0; ii < m_ptLights.size(); ++ii)
    {
       m_ptLights[ii].setUniformPos(*m_currentObjShader, ii);
    }

    for (int ii = 0; ii < m_spotLights.size(); ++ii)
    {
        m_spotLights[ii].setUniformPos(*m_currentObjShader, ii);
        m_spotLights[ii].setUniformDir(*m_currentObjShader, ii);
    }
}








void Scene::RenderFBO(float nearPlane, float farPlane)
{
    m_fboShader.useProgram();
//    glBindVertexArray(m_fbo.vao);
    glActiveTexture(GL_TEXTURE0 + 7);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_fbo.tbo);
    m_fboShader.setUniform1i("fboTex", 7);
    m_fboShader.setUniformMatrix4f("skyboxView", glm::mat4(glm::mat3(m_cam.getViewMatrix())));
    m_fbo.skybox.Draw(m_fboShader);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//    glBindVertexArray(0);
}



void Scene::updateLights()
{
    for(int ii = 0; ii < m_ptLights.size(); ++ii)
    {
        m_ptLights[ii].draw();
    }
    
    
    

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
        if(keyEvent.key == GLFW_KEY_SPACE && keyEvent.action == GLFW_PRESS)
        {
            m_rotSpeedMult += 10;
            m_rotSpeedMult = m_rotSpeedMult % 30;
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





