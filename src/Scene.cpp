//
//  Scene.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/9/21.
//

#include <stdio.h>
#include <chrono>

#include "stb_image.h"

#include "Scene.hpp"
#include "Input.hpp"

Scene* Scene::GLFWCallbackWrapper::m_scene = nullptr;

void Scene::setupShaders()
{
    m_justTexture = Shader(SHADER_FOLDER + "MVPNormalUV.vert", SHADER_FOLDER + "LightsTextures.glsl");
    m_justTexture.makeProgram();
    

    
//    m_effectShader = Shader(SHADER_FOLDER + "MVPNormalUVInstVert.glsl", SHADER_FOLDER + "Texture.frag");
//    m_effectShader.makeProgram();
//    m_skyboxShader = Shader(SHADER_FOLDER + "SkyboxVert.vert", SHADER_FOLDER + "SkyboxFrag.frag");
//    m_skyboxShader.makeProgram();
    m_fboShader = Shader(SHADER_FOLDER + "FBOVert.glsl", SHADER_FOLDER + "HDRFrag.glsl");
    m_fboShader.makeProgram();
    

    m_debugShader = Shader(SHADER_FOLDER + "ShadowDirLightVert.glsl", SHADER_FOLDER + "EmptyFrag.glsl");
    m_debugShader.makeProgram();
    
    Shader::solidShader = Shader(SHADER_FOLDER + "MVPNormalUV.vert", SHADER_FOLDER + "SolidColor.frag");
    Shader::solidShader.makeProgram();
    
    m_currentObjShader = &m_justTexture;
}


void Scene::createLights()
{
    
    PointLight temppt{glm::vec3(0.f, 0.5f, -0.5f)};
    temppt.setDiffuse(glm::vec3(200.f));
    temppt.setAmbient(.0f);
    temppt.setSpecular(0.0f);
    temppt.setAtten(0.f, 0.f, 1.f);
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
    std::vector<std::string> brick2Path = {ASSET_FOLDER+"bricks2.jpeg"};
    std::vector<std::string> brick2NormalPath = {ASSET_FOLDER+"bricks2_normal.jpeg"};
    std::vector<std::string> brick2HeightPath = {ASSET_FOLDER+"bricks2_disp.jpeg"};
    std::vector<std::string> toyBoxPath = {ASSET_FOLDER+"toy_box_diffuse.png"};
    std::vector<std::string> toyBoxNormalPath = {ASSET_FOLDER+"toy_box_normal.png"};
    std::vector<std::string> toyBoxHeightPath = {ASSET_FOLDER+"toy_box_disp.png"};
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
    
    
    
    
    m_shapes.push_back((std::make_unique<Cube>()));
    m_shapes[0]->loadTextures(woodPath, TextureType::DIFFUSE);
    m_shapes[0]->FlipNormals();
    m_shapes[0]->m_transform.position = glm::vec3(0.f, 0.f, -25.7f);
    m_shapes[0]->m_transform.scale = glm::vec3(5.f, 5.f, 2.f*28.f);
 
    
    
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
    

    glGenVertexArrays(1, &m_fboQuad.vao);
    glBindVertexArray(m_fboQuad.vao);
    m_fboQuad.vbo = loadVBOData(fbo_vert);
    glBindVertexArray(0);

    
//    m_fboQuad.skybox = Skybox(0);
    
    

}







//*********************************************
//            Scene Constructor
//*********************************************

Scene::Scene(GLFWwindow* window, int width, int height, float fov,
             float nearField, float farField) : m_window(window), m_firstMouse(true), m_width(width),
    m_height(height), m_nearField(nearField), m_farField(farField), m_fov(fov)
{
    
    
    Scene::GLFWCallbackWrapper::setScene(this);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, Scene::GLFWCallbackWrapper::keyCallback);
    glfwSetFramebufferSizeCallback(window, Scene::GLFWCallbackWrapper::frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, Scene::GLFWCallbackWrapper::mousePosCallback);
    glfwSetScrollCallback(window, Scene::GLFWCallbackWrapper::scrollCallback);
    glfwSetMouseButtonCallback(window, Scene::GLFWCallbackWrapper::mouseButtonCallback);

    
    // *************  Shaders setup  ************** //
    setupShaders();
    
    // *************  Select object setup  ************** //
    m_selectCommands.push_back(std::make_unique<NoSelect>(this));
    m_selectCommands.push_back(std::make_unique<ShapeSelect>(this));
    m_selectCommands.push_back(std::make_unique<LightSelect>(this));
    selectCommandIndex = 2;
    
    
    
    //Use this as flag to choose whether to create objects through JSON or
    // by hard-coding.  Don't want objects created both ways.
    // TODO: This is only used for debuging purposes, when done debugging remove this flag
    bool serializeObjects = true;
    
    if(serializeObjects)
    {
        DeserializeObjects(JSON_FILE);
    }
    else
    {
        createLights();
        m_cam = Camera(fov, float(m_width)/float(m_height), nearField, farField, glm::vec3(0.0f, 0.f, 1.0f), 0.f, 5.f);
    //    setupShapes();
    }
    


    
    
    
    // *************  Uniform buffer for view/proj  ************** //
    glGenBuffers(1, &m_uboVP);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboVP);
    glBufferData(GL_UNIFORM_BUFFER, 16*8, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uboVP);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    
    
    //*********************************************
    //            Demo Begin
    //*********************************************
    m_gaussianFilter = new PingPongFilter(window, GL_RGB16F);
    
    SetupFBORender();
    m_fbo = new Framebuffer(this, m_window);
    m_fbo->SetupToTexture2D(GL_RGBA16F, 2);
    m_fboQuad.tbo = m_fbo->getColorBufferTBO(0);
    m_fboQuad.tbo_aux = m_fbo->getColorBufferTBO(1);
    m_doHDR = true;
    m_fboShader.useProgram();
    m_fboShader.setUniform1ui("hdr", m_doHDR);
    m_fboShader.setUniform1f("exposure", m_exposure);
    m_fboShader.stopUseProgram();
    
    //*********************************************
    //            Demo End
    //*********************************************
    
    
    
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
    // ********  Uniform buffer for VP matrices  ********** //
    m_proj = m_cam.getProjMatrix();
    m_view = glm::mat4(glm::mat3(m_cam.getViewMatrix()));
//    updateVP(m_skyboxShader);
    
    m_view = m_cam.getViewMatrix();
    
    // Fill the UBO with view and proj
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboVP);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    
    
    
    
    // ********  The actual rendering onto the screen  ********** //
    
    // Reset screen for a new render
    glEnable(GL_DEPTH_TEST);
    clearBuffers();
    
    // GUI stuff
    SetupImGui();
    
    // Render scene to a texture
    m_fbo->RenderToTexture2D(m_currentObjShader);
    
    
    RenderFBO(m_fboQuad.tbo);
    
    
}





// ///////////// RenderScene   ////////////////
/**
 \brief All the calls needed to render the scene.  No setup for the render is in this method, just the draw calls and seting the uniforms.
    Can be done with any shader.  Main use for this is so the framebuffer can render the scene from its class.
 
 \param shader - The shader program used to render the scene.
 */
void Scene::RenderScene(Shader* shader)
{    
    
    
    
    shader->useProgram();
    updateLightUniforms();
    drawObjects(*m_currentObjShader);
    shader->stopUseProgram();
    
    // This will draw the point lights onto the scene
    //TODO: This call still has shader program being started and stoped within it.  Should move all shader program logic out of a draw call I think.
    updateLights();

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
    
    for(int ii = 0; ii < m_shapes.size(); ++ii)
    {
        m_shapes[ii]->Draw(shader);
    }
    
    for(int ii = 0; ii < m_models.size(); ++ii)
    {
        m_models[ii]->Draw(shader);
    }
}





void Scene::SetupImGui()
{
    ImGui::Begin("Display Info");
    ImGui::Text("Cam Position: (%4.2f, %4.2f, %4.2f)", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
    glm::vec3 dir = m_cam.getDirection();
    ImGui::Text("Cam Direction: (%4.2f, %4.2f, %4.2f)", dir.x, dir.y, dir.z);
    
    if(m_ptLights.size() > 0)
    {
        static float tempLightBright = m_ptLights[m_selectedLight].getDiffBrightness();

        if(ImGui::BeginCombo(("Light " + std::to_string(m_selectedLight)).c_str(), "Light 0", ImGuiComboFlags_NoPreview))
        {
            for(int ii = 0; ii < m_ptLights.size(); ++ii)
            {
                if(ImGui::Selectable(("Light " + std::to_string(ii)).c_str()))
                {
                    m_selectCommands[2]->selectIndex(ii);
                    tempLightBright = m_ptLights[m_selectedLight].getDiffBrightness();
                }
            }
            
            ImGui::EndCombo();
            
            
        }
        
        
        if(ImGui::DragFloat("Brightness", &tempLightBright, 0.1f, 0.1f, 500.f, "%.3f"))
        {
            m_ptLights[m_selectedLight].setDiffBrightness(tempLightBright);
        }

    }
    
    if(ImGui::Checkbox("Phong Lighting", &m_phong))
    {
        m_currentObjShader->useProgram();
        m_currentObjShader->setUniform1i("phong", m_phong);
        m_currentObjShader->stopUseProgram();
    }
    
    if(ImGui::Checkbox("HDR", &m_doHDR))
    {
        m_fboShader.useProgram();
        m_fboShader.setUniform1ui("hdr", m_doHDR);
        m_fboShader.stopUseProgram();
    }
    
    
    if(ImGui::DragFloat("Exposure", &m_exposure, 0.01f, 0.f, 10.f, "%.3f"))
    {
        m_fboShader.useProgram();
        m_fboShader.setUniform1f("exposure", m_exposure);
        m_fboShader.stopUseProgram();
    }
    
//    if(ImGui::RadioButton("Just texture", m_currentObjShader == &m_justTexture))
//    {
//        m_currentObjShader = &m_justTexture;
//        setupLights();
//    }
//    ImGui::SameLine();
//    if(ImGui::RadioButton("Normal map", m_currentObjShader == &m_normalMap))
//    {
//        m_currentObjShader = &m_normalMap;
//        setupLights();
//    }
    
    if(m_jsonParseError)
    {
        
        ImGui::OpenPopup("JSON Failure");
        if(ImGui::BeginPopupModal("JSON Failure", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Failure to parse json file");
            if(ImGui::Button("OK")) {ImGui::CloseCurrentPopup();m_jsonParseError = false;}
            ImGui::EndPopup();
        }
    }
    ImGui::End();
    
    
    
}







void Scene::clearBuffers()
{
    glClearColor(0.f, 0.f, 0.f, 1.0f);
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
        m_ptLights[ii].setUniformColor(*m_currentObjShader, ii);
    }

    for (int ii = 0; ii < m_spotLights.size(); ++ii)
    {
        m_spotLights[ii].setUniformPos(*m_currentObjShader, ii);
        m_spotLights[ii].setUniformDir(*m_currentObjShader, ii);
    }
}








void Scene::RenderFBO(unsigned int tbo, unsigned int tbo_aux)
{
    m_fboShader.useProgram();
    glBindVertexArray(m_fboQuad.vao);
    glActiveTexture(GL_TEXTURE0 + 7);
    glBindTexture(GL_TEXTURE_2D, tbo);
    m_fboShader.setUniform1i("fboTex", 7);
    
    glActiveTexture(GL_TEXTURE0 + 8);
    glBindTexture(GL_TEXTURE_2D, tbo_aux);
    m_fboShader.setUniform1i("brightTex", 8);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



void Scene::updateLights()
{
    for(int ii = 0; ii < m_ptLights.size(); ++ii)
    {
        m_ptLights[ii].draw(Shader::solidShader);
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
        
        
        // Use CTRL Z/X for save/load so that it doesn't interfere with the moving buttons
        //
        // TODO: Allow use of S and L by skipping movement code when this code is run
        if(keyEvent.key == GLFW_KEY_Z && keyEvent.mods == GLFW_MOD_SUPER && keyEvent.action == GLFW_PRESS)
        {
            SerializeObjects(JSON_FILE);
        }
        if(keyEvent.key == GLFW_KEY_X && keyEvent.mods == GLFW_MOD_SUPER && keyEvent.action == GLFW_PRESS)
        {
            DeserializeObjects(JSON_FILE);
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
    
    if(m_mouseIsCam)
    {
        m_cam.turnYaw(xoffset);
        m_cam.turnPitch(yoffset);
    }
    
}



void Scene::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        m_mouseIsCam = true;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        m_mouseIsCam = false;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}





void Scene::scroll_callback(GLFWwindow* window, double xInc, double yInc)
{
    float fov_inc = (float)yInc;
            
    m_cam.incFOV(fov_inc);
}




// ///////////// SerializeObjects   ////////////////
/**
 \brief Initialize a blank json array and store in member m_gameObjectJson.  Loop through each vector of objects and add them to the json array.  Then
    write json object to file.
 
 \param jsonPath - Path to the json file
 
 \returns
 */
void Scene::SerializeObjects(const std::string& jsonPath)
{
    m_gameObjectJson = json::array();
    m_gameObjectJson.push_back(m_cam);
    for (const auto& light : m_ptLights)
    {
        m_gameObjectJson.push_back(light);
    }
    for (const auto& light : m_dirLights)
    {
        m_gameObjectJson.push_back(light);
    }
    for (const auto& light : m_spotLights)
    {
        m_gameObjectJson.push_back(light);
    }
    for (const auto& shape : m_shapes)
    {
        /**
         *  Dynamic casting of shape to particular shape.
         *
         *  *shape - convert unique_ptr<Shape> --> Shape object (no idea what a Shape object even is since it's abstract)
         *  &*shape - convert Shape --> raw pointer to Shape
         *  dynamic_cast<Cube*>(&*shape) - convert Shape* --> Cube*
         *  *dynamice_cast<Cube*>(&*shape) - convert Cube* --> Cube
         *
         *  TODO: This is ugly and I would have no idea what it means without these comments.  Find a better way to cast from unique_ptr<Shape> to Cube.
         */
        switch(shape->m_shapeType)
        {
            case GameObject::CUBE:
                m_gameObjectJson.push_back(*dynamic_cast<Cube*>(&*shape));
                break;
                
            case GameObject::PLANE:
                m_gameObjectJson.push_back(*dynamic_cast<Plane*>(&*shape));
                break;
                
            default:
                break;
        }
    }
    
    for (const auto& model : m_models)
    {
        m_gameObjectJson.push_back(*model);
    }
    
    JsonToFile(m_gameObjectJson, JSON_FILE);
}






// ///////////// DeserializeObjects   ////////////////
/**
 \brief Loop through all objects in a json array and fill all object vectors (shapes, ptLights, ...) with info
    from the json array.  This will start everything over, so whatever is in those vectors before this method
    will be completely overridden.
 
 \param jsonFilePath - The json file with the json information
 */
void Scene::DeserializeObjects(const std::string& jsonFilePath)
{
    // This flag is set if a light is deserialized to make sure that `setupLights()` is called at the end
    bool addLight = false;
    
    // *************  JSON file parsing  ************** //
    std::ifstream file;
    file.open(jsonFilePath);
//    file >> m_gameObjectJson;
    json j;
    try
    {
        j = json::parse(file, nullptr, true, true);
    }
    catch (json::parse_error& e)
    {
        std::cout << e.what() << std::endl;
        m_jsonParseError = true;
        return;
    }
    
    m_gameObjectJson = j;
    
    // Want to start vector of objects from scatch, so that the loading will not double up
    m_shapes.erase(m_shapes.begin(), m_shapes.end());
    m_ptLights.erase(m_ptLights.begin(), m_ptLights.end());
    m_dirLights.erase(m_dirLights.begin(), m_dirLights.end());
    m_spotLights.erase(m_spotLights.begin(), m_spotLights.end());
    
    
    for(const auto& j : m_gameObjectJson)
    {
        switch(j.value("type", GameObject::INVALID))
        {
                
                // ********  Shapes  ********** //
            case GameObject::CUBE:
                m_shapes.push_back(std::make_unique<Cube>(Cube(j.get<Cube>())));
                break;
            case GameObject::PLANE:
                m_shapes.push_back(std::make_unique<Plane>(Plane(j.get<Plane>())));
                break;
                
                
                
                // *************  Models  ************** //
            case GameObject::MODEL:
                m_models.push_back(std::make_unique<Model>(Model(j.get<Model>())));
                break;
                
                
                
                // ********  Lights  ********** //
            case GameObject::PTLIGHT:
                m_ptLights.push_back(j.get<PointLight>());
                addLight = true;
                break;
            case GameObject::DIRLIGHT:
                m_dirLights.push_back(j);
                addLight = true;
                break;
            case GameObject::SPOTLIGHT:
                m_spotLights.push_back(j);
                addLight = true;
                break;
                
                
                
                
                // ********  Camera  ********** //
            case GameObject::CAMERA:
                m_cam = j;
                break;
            default:
                break;
        }
        
        
        if(addLight)
            setupLights();
        
        
    }
    
}


Scene::~Scene()
{
    delete m_fbo;
}





