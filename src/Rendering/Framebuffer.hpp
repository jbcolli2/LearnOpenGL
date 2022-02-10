//
//  Framebuffer.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 10/28/21.
//

#ifndef Framebuffer_h
#define Framebuffer_h


#include "Utilities/OpenGLUtil.hpp"
#include "GameObjects/Camera.hpp"
#include "Scene.hpp"

class Scene;

class Framebuffer
{
    unsigned int m_fbo, m_rbo, m_tboShadow;
    std::vector<unsigned int> m_tbo2D{};
    std::vector<GLenum> m_colorAttachments;
    int m_width, m_height, m_shadowWidth, m_shadowHeight;
    float m_near, m_far;
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
    
    
    void SetupToTexture2D(GLint internalFormat = GL_RGBA, int numColorBuffers = 1);
    void SetupToTexture2D(const std::vector<GLint>& internalFormats, int numColorBuffers = 1);
    void RenderToTexture2D(Shader* shader, const glm::vec3& position, const glm::vec3& direction);
    void RenderToTexture2D(Shader* shader);
    void RenderToCubemap(unsigned int tbo, const glm::vec3& position, const glm::mat4& proj);
    
    
    unsigned int getColorBufferTBO(int index) {return m_tbo2D[index];};
    
};




class PingPongFilter
{
    // Dimensions of the main viewport
    int m_width{0}, m_height{0};
    
    // ID for the texture outputs of each framebuffer
    unsigned int m_tbo[2] = {0,0};
    
    // ID for the 2 framebuffers
    unsigned int m_fbo[2] = {0,0};
    
    // ID for render buffer attached as depth and stencil buffers to the framebuffers
    unsigned int m_rbo[2] = {0,0};
    
    // VAO/VBO for the quad to draw onto the full screen
    //    only need one since the vertex data is the same for both framebuffers
    unsigned int m_vao{0}, m_vbo{0};
    
    
    
    
    
public:
    PingPongFilter() = delete;
    
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
    PingPongFilter(GLFWwindow* window, unsigned int internalFormat = GL_RGBA);
    
    
    /*******************  Filter   ************************************
     * \brief Perform the ping pong filter defined by the shader.  This will be specific to a
     *      filter done in two steps only, not more.  Both steps will be done by the same shader,
     *      and there must exists a switch uniform that changes which step is done.
     *
     *      One full iteration consists of both steps in the shader.  So 5 iterations will mean
     *      10 total draw calls, 5 for each framebuffer.
     *
     * \param shader - pointer to the shader used in the filter
     * \param initialTexture - ID of texture storing the initial image to be filtered
     * \param switchUniform - string for name of uniform that controls the filter switch
     * \param initialSwitch - value of the switch initially
     * \param numIterations - the number of iterations.
     **************************************************************///
    unsigned int Filter(Shader* shader, unsigned int initialTexture, const std::string& switchUniform, bool initialSwitch = true, unsigned int numIterations = 5);
};


#endif /* Framebuffer_h */
