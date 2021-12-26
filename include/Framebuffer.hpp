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
    unsigned int m_fbo, m_rbo, m_tboShadow;
    int m_width, m_height, m_shadowWidth, m_shadowHeight;
    Shader m_shadowShader;
    Scene* m_scene;
    
    std::vector<glm::vec3> m_cubemapDirs = {
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(-1.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(0.f, -1.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f),
        glm::vec3(0.f, 0.f, -1.f)
    };
    std::vector<glm::vec3> m_cubemapUps = {
        glm::vec3(0.f, -1.f, 0.f),
        glm::vec3(0.f, -1.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f),
        glm::vec3(0.f, 0.f, -1.f),
        glm::vec3(0.f, -1.f, 0.f),
        glm::vec3(0.f, -1.f, 0.f)
    };
    
    glm::mat4 m_lightVPCube[6];
    
public:
    Framebuffer(){};
    Framebuffer(Scene* scene, GLFWwindow* window);
    
    void SetupShadowMap(std::string vertSource, std::string fragSource, int width, int height);
    void SetupShadowCubeMap(std::string vertSource, std::string geomSource, std::string fragSource, const glm::vec3& lightPos, float near, float far, int width, int height);
    
    unsigned int RenderShadowMap(const glm::mat4& lightVP);
    unsigned int RenderShadowCubeMap(const glm::vec3& position);
    void RenderToTexture2D(unsigned int tbo, const glm::vec3& position, const glm::vec3& direction, const glm::mat4& proj);
    void RenderToCubemap(unsigned int tbo, const glm::vec3& position, const glm::mat4& proj);
    
};


#endif /* Framebuffer_h */
