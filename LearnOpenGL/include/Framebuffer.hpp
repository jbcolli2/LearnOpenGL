//
//  Framebuffer.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 10/28/21.
//

#ifndef Framebuffer_h
#define Framebuffer_h


#include "OpenGLUtil.hpp"
#include "Camera.hpp"
#include "Scene.hpp"

class Scene;

class Framebuffer
{
    unsigned int m_fbo, m_rbo;
    int m_width, m_height;
    Scene* m_scene;
    
    std::vector<glm::vec3> m_cubemapDirs = {
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(-1.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.00001f),
        glm::vec3(0.f, -1.f, 0.f),
        glm::vec3(0.f, 0.f, -1.f),
        glm::vec3(0.f, 0.f, 1.f)
    };
    
public:
    Framebuffer(){};
    Framebuffer(Scene* scene, GLFWwindow* window);
    
    void RenderToTexture2D(unsigned int tbo, const glm::vec3& position, const glm::vec3& direction, const glm::mat4& proj);
    void RenderToCubemap(unsigned int tbo, const glm::vec3& position, const glm::mat4& proj);
    
};


#endif /* Framebuffer_h */
