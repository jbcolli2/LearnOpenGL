//
//  BasicDraw.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#ifndef BasicDraw_h
#define BasicDraw_h

#include <stdio.h>
#include <vector>
#include <string>


#include "Shapes.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Model.hpp"




class Scene
{
    std::string SHADER_FOLDER = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/shaders/";
    std::string IMAGE_FOLDER = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/include/";
    std::string ASSET_FOLDER = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/assets/";
    
    
    GLFWwindow* m_window;
    
    Shader m_objShader, m_lightShader;
    
    std::vector< std::unique_ptr<Shape> > m_shapes;
    Model m_backpack;
    
    DirLight m_dirLight;
    PointLight m_ptLight[4];
    SpotLight m_spotLight;
    std::vector<glm::vec3> m_lightPos = {
        glm::vec3(.5f, 1.f, -2.f),
        glm::vec3(-1.f, -1.f, -4.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(-3.f, 0.f, -1.f)
    };
    Camera m_cam;
    
    
    //******* Mouse Input ***********//
    float m_lastMousePosX, m_lastMousePosY;
    bool m_firstMouse;
    const float m_mouseSensitivity = 0.1f;
    
    
    //*******  Camera/Window Variables *******//
    int m_width, m_height;
    float m_camSpeed = 1.5f;
    
    
    
    std::vector<glm::vec3> m_positions = {
        glm::vec3(0.5f, .75f, -4.f),
        glm::vec3(-0.7f, 0.7f, -4.f),
        glm::vec3(0.6f, -1.f, -3.5f),
        glm::vec3(0.9f, 0.f, -9.f),
        glm::vec3(1.f, 1.f, -3.f),
        glm::vec3(-0.7f, -1.3f, -12.f),
        glm::vec3(-0.6f, 0.75f, -3.f),
        glm::vec3(1.5f, .87f, -4.2f),
        glm::vec3(-1.2f, -1.7f, -3.6f)
    };
    
    glm::mat4 ID4 = glm::mat4(1.f);
    glm::mat4 m_model, m_view, m_proj;
    
    
    
    
    
public:
    Scene(GLFWwindow* window, int width, int height, float fov = 45.f, float nearField = .1f, float farField = 100.f);
    
    void draw();
    
    void processInput(float deltaTime);
    
    
    
    
    class GLFWCallbackWrapper
    {
    public:
        GLFWCallbackWrapper() = delete;
        GLFWCallbackWrapper(const GLFWCallbackWrapper&) = delete;
        GLFWCallbackWrapper(GLFWCallbackWrapper&&) = delete;
        ~GLFWCallbackWrapper() = delete;
        
        static void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
        {
            GLFWCallbackWrapper::m_scene->framebuffer_size_callback(window, width, height);
        };
        
        static void mousePosCallback(GLFWwindow* window, double xpos, double ypos)
        {
            GLFWCallbackWrapper::m_scene->mouse_callback(window, xpos, ypos);
        };
        
        static void scrollCallback(GLFWwindow* window, double xInc, double yInc)
        {
            GLFWCallbackWrapper::m_scene->scroll_callback(window, xInc, yInc);
        };
        
        static void setScene(Scene* scene)
        {
            GLFWCallbackWrapper::m_scene = scene;
        }
      
        
    private:
        static Scene* m_scene;
    };
    
    
    
    
    
    
private:
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xInc, double yInc);
    
    template<typename VertT>
    std::vector<VertT> genBoxVerts();
    
    
    
    
   
};










#endif /* BasicDraw_h */
