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










// ///////////// RenderToTexture   ////////////////
/**
 \brief Point a camera into the scene and render what it sees to a texture.  The empty tbo is generated and its parameters
    set in the calling method.
 
    The fbo will be created in this method.  The target is given for the texture, as well as the texture unit.
 
    This can be used to simply render the entire scene and then apply non-local effects.  This can also be called 6 times to create a cubemap of the scene from
        the perspecitve of a particular object.
 
 \param tbo - Empty texture object created from calling method
  
 */
void Framebuffer::RenderToTexture2D(unsigned int tbo,
                     const glm::vec3& position, const glm::vec3& direction, const glm::mat4& proj)
{
    // Generate framebuffer object
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    
    // Create empty texture of correct size
    glBindTexture(GL_TEXTURE_2D, tbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    
    // Attach tbo and rbo to framebuffer and check for completness
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer NOT complete\n";
    

    // Render the scene to the framebuffer
    glm::mat4 view = glm::lookAt(position, position + direction, glm::vec3(0.f, 1.f, 0.f));
//    m_scene->RenderScene(view, proj);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
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
