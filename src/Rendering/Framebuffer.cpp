//
//  Framebuffer.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 10/28/21.
//

#include <stdio.h>
#include <iostream>

#include "Framebuffer.hpp"


// ///////////// Framebuffer Ctor   ////////////////
/**
 \brief Store the scene pointer.  Get the dimensions of the viewport and store them.
                Create the render buffer for depth and stencil data.
 */
Framebuffer::Framebuffer(Scene* scene, GLFWwindow* window) : m_scene(scene)
{
    glfwGetFramebufferSize(window, &m_width, &m_height);
    
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);

}








// ///////////// SetupShadowMap   ////////////////
/**
 \brief Perform all the setup work to create a shadow map using the objects in the scene.  This will make the shader program.
 
 \param vertSourcePath - path for the source code of the vertex shader
 \param fragSourcePath - path for the source code of the fragment shader
 \param shadowMapWidth/shadowMapHeight - set the resolution of the shadow depth map
 

 */
void Framebuffer::SetupShadowMap(std::string vertSourcePath, std::string fragSourcePath, int shadowMapWidth, int shadowMapHeight)
{
    m_shadowShader = Shader(vertSourcePath, fragSourcePath);
    m_shadowShader.makeProgram();
    
    glGenFramebuffers(1, &m_fbo);
    m_shadowWidth = shadowMapWidth;
    m_shadowHeight = shadowMapHeight;
    glGenTextures(1, &m_tboShadow);
    
    // ********  Setup the shadow map texture and framebuffer  ********** //
    
    // Create empty texture
    glBindTexture(GL_TEXTURE_2D, m_tboShadow);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.f, 1.f, 1.f, 1.f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    // Bind texture to the depth component of framebuffer, and use no color buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_tboShadow, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Check if framebuffer complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer NOT complete\n" << "Status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n" << GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}







// ///////////// SetupShadowCubeMap   ////////////////
/**
 \brief Create the cubemap texture object.  Create the light space transformations for all 6 sides of the cube centered around the light.
    Attach cubemap to the framebuffer depth attachements and check that framebuffer is complete.
 
 \param vertSourcePath - path for the source code of the vertex shader
 \param geomSourcePath - path for the source code of the geometry shader
 \param fragSourcePath - path for the source code of the fragment shader
 \param lightPos -      Position of the light that the cubemap is centered on
 \param near\far - value of the near\far field for the projection matrix
 
 */
void Framebuffer::SetupShadowCubeMap(std::string vertSourcePath, std::string geomSourcePath, std::string fragSourcePath, const glm::vec3& lightPos, float near, float far, int shadowWidth, int shadowHeight)
{
    m_shadowWidth = shadowWidth;
    m_shadowHeight = shadowHeight;
    m_near = near;
    m_far = far;
    
    
    
    
    
    // ********  Setup Shader  ********** //
    m_shadowShader = Shader(vertSourcePath, geomSourcePath, fragSourcePath);
    m_shadowShader.makeProgram();
    
    m_shadowShader.useProgram();
    m_shadowShader.setUniform1f("farPlane", far);
    m_shadowShader.stopUseProgram();
    
    
    // ********  Generate and load data to the textures  ********** //
    
    // Create empty texture
    glGenFramebuffers(1, &m_fbo);
    glGenTextures(1, &m_tboShadow);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tboShadow);
    for(int side = 0; side < 6; ++side)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    
    //Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.f, 1.f, 1.f, 1.f};
    glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    // Bind texture to the depth component of framebuffer, and use no color buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_tboShadow, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Check if framebuffer complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer NOT complete\n" << "Status = " << std::hex << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n" << GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}





// ///////////// RenderShadowMap   ////////////////
/**
 \brief Render the scene from the lights perspective and store the results in a depth texture.  Lights View/Proj matrix is passed in from the scene and the handle
        to the depth texture is passed back to the scene.  Scene can then use this depth texture to create shadows during the actual rendering.
 
 \param lightVP - view/proj matrix for the particular light creating these shadows
 
 \returns tbo handle for the depth texture
 */
unsigned int Framebuffer::RenderShadowMap(const glm::mat4& lightVP)
{
    m_shadowShader.useProgram();
    m_shadowShader.setUniformMatrix4f("lightVP", lightVP);

    
    // ********  Render the shadow map  ********** //
    glViewport(0, 0, m_shadowWidth, m_shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    for(auto& shape : m_scene->m_shapes)
    {
        shape->Draw(m_shadowShader);
    }
    for(auto& model : m_scene->m_models)
    {
        model->Draw(m_shadowShader);
    }
    m_shadowShader.stopUseProgram();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_width, m_height);
    
    
    return m_tboShadow;
}





// ///////////// RenderShadowCubeMap   ////////////////
/**
 \brief Render the scene from the lights perspective and store the results in a depth texture.  Lights View/Proj matrix is passed in from the scene and the handle
        to the depth texture is passed back to the scene.  Scene can then use this depth texture to create shadows during the actual rendering.
 
 
 \returns tbo handle for the depth texture
 */
unsigned int Framebuffer::RenderShadowCubeMap(const glm::vec3& position)
{
    m_shadowShader.useProgram();
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.f), 1.f, m_near, m_far);
    for(int cubeSide = 0; cubeSide < 6; ++cubeSide)
    {
        glm::mat4 shadowView = glm::lookAt(position, position + m_cubemapDirs[cubeSide], m_cubemapUps[cubeSide]);
        m_lightVPCube[cubeSide] = shadowProj * shadowView;
    }

    m_shadowShader.setUniform3f("lightPos", position.x, position.y, position.z);
    for(int side = 0; side < 6; side++)
        m_shadowShader.setUniformMatrix4f("lightVPCube[" + std::to_string(side) + "]", m_lightVPCube[side]);

    
    // ********  Render the shadow map  ********** //
    glViewport(0, 0, m_shadowWidth, m_shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    for(auto& shape : m_scene->m_shapes)
    {
        shape->Draw(m_shadowShader);
    }
    for(auto& model : m_scene->m_models)
    {
        model->Draw(m_shadowShader);
    }
    m_shadowShader.stopUseProgram();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_width, m_height);
    
    
    return m_tboShadow;
}






// ///////////// SetupToTexture2D   ////////////////
/**
 \brief Setup needed to use a framebuffer to render to a Texture2D.  This includes creating the FBO and TBO, as well as attaching everything to the
    framebuffer and checking for completeness.  The FBO should be totally ready for rendering after this method.

 */
void Framebuffer::SetupToTexture2D(GLint internalFormat, int numColorBuffers)
{
    m_tbo2D.clear();
    m_colorAttachments.clear();
    
    // Generate framebuffer object
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    
    // Create empty textures of correct size
    for(int ii = 0; ii < numColorBuffers; ++ii)
    {
        m_tbo2D.push_back(0);
        glGenTextures(1, &m_tbo2D[ii]);
        glBindTexture(GL_TEXTURE_2D, m_tbo2D[ii]);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ii, GL_TEXTURE_2D, m_tbo2D[ii], 0);
        
        m_colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + ii);
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer NOT complete\n";
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}




// ///////////// RenderToTexture   ////////////////
/**
 \brief Point a camera into the scene and render what it sees to a texture.
  
    This can be used to simply render the entire scene and then apply non-local effects.  This can also be called 6 times to create a cubemap of the scene from
        the perspecitve of a particular object.
 
 \param shader - Shader program used to render scene to framebuffer texture
 \param position - View position used for render
 \param direction - View direction used for render
 
 \returns The tbo of the texture rendered to through the framebuffer
  
 */
void Framebuffer::RenderToTexture2D(Shader* shader, const glm::vec3& position, const glm::vec3& direction)
{
    

    // Render the scene to the framebuffer
    glm::mat4 view = glm::lookAt(position, position + direction, glm::vec3(0.f, 1.f, 0.f));
    
    // Change the UBO to the new view matrix
    glBindBuffer(GL_UNIFORM_BUFFER, m_scene->m_uboVP);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    m_scene->RenderScene(shader);
    
    // Change the UBO back to the camera view matrix
    glBindBuffer(GL_UNIFORM_BUFFER, m_scene->m_uboVP);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_scene->m_view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}




// ///////////// RenderToTexture   ////////////////
/**
 \brief Simply render the scene using the scene camera.  We are using a framebuffer to render the scene to a texture instead of the screen.
  

 
 \param shader - Shader program used to render scene to framebuffer texture
 */
void Framebuffer::RenderToTexture2D(Shader* shader)
{
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    if(m_tbo2D.size() > 1)
    {
        glDrawBuffers(static_cast<int>(m_tbo2D.size()), &m_colorAttachments[0]);
    }
    m_scene->clearBuffers();
    m_scene->RenderScene(shader);

    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}




void Framebuffer::RenderToCubemap(unsigned int tbo, const glm::vec3 &position, const glm::mat4 &proj)
{
    glm::mat4 proj1 = glm::perspective(glm::radians(89.0), 1.0, 0.1, 1000.0);
    glm::mat4 view;
    // Generate framebuffer object
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    
    
    // Create 6 empty textures of correct size bound to the 6 faces of the cube
    glBindTexture(GL_TEXTURE_CUBE_MAP, tbo);
    for(int ii = 0; ii < 6; ++ii)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + ii, 0, GL_RGB, 1000, 1000, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }
    glViewport(0,0, 1000, 1000);
    for(int ii = 0; ii < 6; ++ii)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + ii, tbo, 0);
        view = glm::lookAt(position, position + m_cubemapDirs[ii], glm::vec3(0.f, 1.f, 0.f));
//        m_scene->RenderScene(view, proj1);
    }
    glViewport(0,0, m_width, m_height);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer NOT complete: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << " " << GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT<< std::endl;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &m_fbo);

}








//***************************************************************
//            PingPongFilter Class
//***************************************************************

/*******************  PingPongFilter ctor   ************************************
 * \brief Generate all buffers(frame/texture/render) needed for the ping pong operation.
 *      We will need two of everything, as there will be two framebuffers passing the picture
 *      back and forth between them.
 *
 *      Attach colorbuffer and renderbuffer to each framebuffer.
 *
 *      Generate and fill vao/vbo and define vertex attributes.
 *
 * \param internalFormat - the format of the textures.
 **************************************************************/
PingPongFilter::PingPongFilter(GLFWwindow* window, unsigned int internalFormat)
{
    glfwGetFramebufferSize(window, &m_width, &m_height);
    
    // *************  Generate all buffers  ************** //
    glGenFramebuffers(2, m_fbo);
    glGenTextures(2, m_tbo);
    glGenRenderbuffers(2, m_rbo);
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    
    
    // *************  Initialize texture and render buffers  ************** //
    for(int ii = 0; ii < 2; ++ii)
    {
        glBindTexture(GL_TEXTURE_2D, m_tbo[ii]);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        
//        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo[ii]);
//        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
//        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    
    
    // *************  Attach buffers to framebuffers  ************** //
    for(int ii = 0; ii < 2; ++ii)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[ii]);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tbo[ii], 0);
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo[ii]);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer NOT complete\n";
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    
    // *************  Initialize VAO/VBO for screen quad  ************** //
    std::vector<Vert3x2f> screenVerts = {
        Vert3x2f(-1.f, 1.0f, 0.0f, 0.f, 1.f),
        Vert3x2f(-1.f, -1.f, 0.0f, 0.f, 0.f),
        Vert3x2f(1.f, -1.f, 0.0f, 1.f, 0.f),

        Vert3x2f(-1.f, 1.f, 0.0f, 0.f, 1.f),
        Vert3x2f(1.f, -1.f, 0.0f, 1.f, 0.f),
        Vert3x2f(1.f, 1.f, 0.0f, 1.f, 1.f)
        };
    
    glBindVertexArray(m_vao);
    m_vbo = loadVBOData(screenVerts);
    glBindVertexArray(0);
}





/*******************  Filter   ************************************
 * \brief Perform a filter on an imaged using a ping pong shader.  This shader does two
 *      operations on an image, i.e. horz and vertical gaussian blur.  This two-stage filtering
 *      is done using 2 framebuffers.  Output of one framebuffer is the texture input to the other.
 *      This goes back and forth iteratively until filter is complete.
 *
 * \param shader - pointer to the shader used in the filter
 * \param initialTexture - ID of texture storing the initial image to be filtered
 * \param switchUniform - string for name of uniform that controls the filter switch
 * \param initialSwitch - value of the switch initially
 * \param numIterations - the number of iterations.
 **************************************************************///
unsigned int PingPongFilter::Filter(Shader* shader, unsigned int initialTexture, const std::string& switchUniform, bool initialSwitch, unsigned int numIterations)
{
    bool firstIteration = true;
    bool switchValue = !initialSwitch;
    unsigned int currentTBO{0};
    
    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    
    for(int iter = 0; iter < numIterations; ++iter)
    {
        // *************  Render to fbo 0  ************** //
        currentTBO = firstIteration ? initialTexture : m_tbo[1];
        switchValue = !switchValue;
        firstIteration = false;
        
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[0]);
        
        
        glBindTexture(GL_TEXTURE_2D, currentTBO);
        shader->setUniform1ui(switchUniform, switchValue);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        
        // *************  Render to fbo 1  ************** //
        currentTBO = m_tbo[0];
        switchValue = !switchValue;

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[1]);

        glBindTexture(GL_TEXTURE_2D, currentTBO);
        shader->setUniform1ui(switchUniform, switchValue);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return m_tbo[1];
}
