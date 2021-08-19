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




class Scene
{
    std::vector< std::unique_ptr<Shape> > m_shapes;
    Shader m_objShader, m_lightShader;
    GLFWwindow* m_window;
    
    Camera m_cam;
    
    float m_lastMousePosX, m_lastMousePosY;
    bool m_firstMouse;
    const float m_sensitivity = 0.1f;
    float m_fov = 45.f;
    
    float m_test=800.f/600.f;
    
    
    std::vector<glm::vec3> m_positions = {
        glm::vec3(0.f, 0.f, -2.f),
//        glm::vec3(-0.7f, 0.7f, -2.f),
//        glm::vec3(0.6f, -1.f, -2.f),
//        glm::vec3(0.45f, 0.f, -9.f),
//        glm::vec3(0.4f, 1.f, -3.f),
//        glm::vec3(-0.3f, -1.f, -12.f),
//        glm::vec3(-0.6f, 0.5f, -3.f)
    };
    glm::vec3 m_lightPos{0.f, 0.0f, -1.f};
    
    
public:
    Scene(GLFWwindow* window);
    
    void draw();
    
    void processInput(float deltaTime);
    
    
    
    
    class GLFWCallbackWrapper
    {
    public:
        GLFWCallbackWrapper() = delete;
        GLFWCallbackWrapper(const GLFWCallbackWrapper&) = delete;
        GLFWCallbackWrapper(GLFWCallbackWrapper&&) = delete;
        ~GLFWCallbackWrapper() = delete;
        
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
    
    
    
    
    
   
};










#endif /* BasicDraw_h */
