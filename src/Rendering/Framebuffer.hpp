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

#include <algorithm>

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
    
    void BlitDepthBuffer();
    
    
    unsigned int getColorBufferTBO(int index) {return m_tbo2D[index];};
    
};




class PingPongFilter
{
    // Dimensions of the main viewport
    int m_width{-1}, m_height{-1};
    
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






class Tex2QuadFilter
{
    // width and height of the screen, and thus the fbo
    int m_width{-1}, m_height{-1};
    
    // All the textures to be input to the shader with their uniform names
    std::vector<std::string> m_tboInputName{};
    std::vector<unsigned int> m_tboInput{};
    
    // The textures being output by the shader with their names.  Store them here so they can be gotten later.
    std::vector<std::string> m_tboOutputName{};
    std::vector<unsigned int> m_tboOutput{};
    
    // fbo and rbo for the filter
    unsigned int m_fbo{0}, m_rbo{0};
    // vao and vbo for the screen quad
    unsigned int m_vao{0}, m_vbo{0};
    
    
    
    
    /*******************  bindTexture   ************************************
     * \brief A helper method to bind a texture and set the appropriate uniform in the
     *      shader program.  It is assumed that the shader is already being used
     *
     * \param shader - the shader program that needs the uniform name
     * \param tbo - id of the texture to be bound
     * \param uniformName
     * \param texUnit - the texture unit to bind the texture to and set the uniform to
     **************************************************************///
    void bindTexture(Shader* shader, unsigned int tbo, const std::string& uniformName, int texUnit)
    {
        assert("Shader not in use" && shader->InUse());
        
        glActiveTexture(GL_TEXTURE0 + texUnit);
        glBindTexture(GL_TEXTURE_2D, tbo);
        shader->setUniform1i(uniformName, texUnit);
    };
public:
    Tex2QuadFilter() = default;
    
    /*******************  Tex2QuadFilter ctor   ************************************
     * \brief Get the width and height of the window.  Initialize the rbo to be a depth buffer.
     *      Generate the fbo and all output textures.  Attach textures and rbo to framebuffer.
     *
     * \param window - the context window to get width and height of screen
     * \param internalFormat - the format of the output textures, same for all
     * \param numOutputs - if this is 0, output will be rendered to the default framebuffer
     **************************************************************///
    Tex2QuadFilter(GLFWwindow* window, unsigned int internalFormat, int numOutputs = 1);
    
    /*******************  Tex2QuadFilter ctor   ************************************
     * \brief Same as above, only allows for different format for each output
     **************************************************************///
    Tex2QuadFilter(GLFWwindow* window, const std::vector<unsigned int>& internalFormat, int numOutputs = 1);
    
    
    /*******************  AddInputTex   ************************************
     * \brief Add the tbo of an input texture to the shader.  Also pass the uniform name for the texture.
     *
     * \param tbo - tbo id for texture
     * \param uniformName - name of the uniform
     **************************************************************///
    void AddInputTex(unsigned int tbo, const std::string& uniformName)
    {
        m_tboInput.push_back(tbo);
        m_tboInputName.push_back(uniformName);
    };
    
    /*******************  GetOutputTex   ************************************
     * \brief Returns tbo of one of the output textures.
     *
     * \param index - Index of the output texture we want.
     **************************************************************///
    unsigned int GetOutputTex(int index)
    {
        return m_tboOutput[index];
    };

    
    
    
    /*******************  Render   ************************************
     * \brief Use the shader and added input textures to render to the
     *      output textures attached to the framebuffer.
     *
     *      Note that if no output textures were set at the ctor, then this will
     *      render to the default framebuffer, that is the screen.
     *
     * \param shader - Shader program to use for render
     **************************************************************///
    void Render(Shader* shader);
};


#endif /* Framebuffer_h */
