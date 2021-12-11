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





// ///////////// RenderToTexture   ////////////////
/**
 \brief Point a camera into the scene and render what it sees to a texture.  The empty tbo is generated and its parameters
    set in the calling method.
 
    The fbo will be created in this method.  The target is given for the texture, as well as the texture unit.
 
    This can be used to simply render the entire scene and then apply non-local effects.  This can also be called 6 times to create a cubemap of the scene from
        the perspecitve of a particular object.
 
 \param tbo - Empty texture object created from calling method
 \param target - Target to bind the tbo to, such as GL_TEXTURE_2D, or GL_TEXTURE_CUBE_MAP
 \param texTarget - Target used to create the texture data.  This could be GL_TEXTURE_2D, or a direction for the cubemap such as
            GL_TEXTURE_CUBE_MAP_POSITIVE_X
 
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
    
    // Attach tbo and rbo to framebuffer and check for completness
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer NOT complete\n";
    

    // Render the scene to the framebuffer
    glm::mat4 view = glm::lookAt(position, position + direction, glm::vec3(0.f, 1.f, 0.f));
    m_scene->RenderScene(view, proj);
    
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
        m_scene->RenderScene(view, proj1);
    }
    glViewport(0,0, m_width, m_height);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer NOT complete: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << " " << GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT<< std::endl;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &m_fbo);

}