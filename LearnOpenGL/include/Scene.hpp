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



class SelectObjCommand
{
public:
    virtual ~SelectObjCommand(){};
    
    void virtual changeSelect() = 0;
    void virtual moveForward(float inc) = 0;
    void virtual moveBackward(float inc) = 0;
    void virtual moveLeft(float inc) = 0;
    void virtual moveRight(float inc) = 0;
    void virtual moveUp(float inc) = 0;
    void virtual moveDown(float inc) = 0;
};


class Scene
{
    std::string SHADER_FOLDER = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/shaders/";
    std::string IMAGE_FOLDER = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/include/";
    std::string ASSET_FOLDER = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/assets/";
    
    
    GLFWwindow* m_window;
    float m_deltaTime;
    
    Shader m_objShader;
    
    std::vector<std::unique_ptr<SelectObjCommand>> m_selectCommands;
    int selectCommandIndex = 0;
    
    std::vector< std::unique_ptr<Shape> > m_shapes;
    Model m_backpack;
    
    int m_selectedShape = 0;
    int m_selectedLight = 0;
    
    DirLight m_dirLight;
    std::vector<PointLight> m_ptLight;
    SpotLight m_spotLight;
    std::vector<glm::vec3> m_lightPos = {
        glm::vec3(.5f, 2.f, -2.f),
        glm::vec3(-1.f, 2.f, -4.f),
        glm::vec3(0.f, 2.f, 0.f),
        glm::vec3(-1.f, 3.f, -2.f)
    };
    Camera m_cam;
    
    
    //******* Mouse Input ***********//
    float m_lastMousePosX, m_lastMousePosY;
    bool m_firstMouse;
    const float m_mouseSensitivity = 0.1f;
    
    
    //*******  Camera/Window Variables *******//
    int m_width, m_height;
    float m_camSpeed = 1.5f;
    
    
    
    
    glm::mat4 ID4 = glm::mat4(1.f);
    glm::mat4 m_model, m_view, m_proj;
    
    
    
    
    class NoSelect : public SelectObjCommand
    {
        Scene* m_scene;
    public:
        NoSelect(Scene* scene) : m_scene(scene){};
        void virtual changeSelect() override {};
        void virtual moveForward(float inc) override {};
        void virtual moveBackward(float inc) override{};
        void virtual moveLeft(float inc) override{};
        void virtual moveRight(float inc) override{};
        void virtual moveUp(float inc) override{};
        void virtual moveDown(float inc) override{};
        
    };
    
    class ShapeSelect : public SelectObjCommand
    {
        Scene* m_scene;
    public:
        ShapeSelect(Scene* scene) : m_scene(scene){};
        void virtual changeSelect() override
            {
                m_scene->m_shapes[m_scene->m_selectedShape]->m_outlined = false;
                m_scene->m_selectedShape = (m_scene->m_selectedShape + 1) % m_scene->m_shapes.size();
                m_scene->m_shapes[m_scene->m_selectedShape]->m_outlined = true;
            };
        void virtual moveForward(float inc) override
            {m_scene->m_shapes[m_scene->m_selectedShape]->m_transform.position.z -= inc;};
        void virtual moveBackward(float inc) override
            {m_scene->m_shapes[m_scene->m_selectedShape]->m_transform.position.z += inc;};
        void virtual moveLeft(float inc) override
            {m_scene->m_shapes[m_scene->m_selectedShape]->m_transform.position.x -= inc;};
        void virtual moveRight(float inc) override
            {m_scene->m_shapes[m_scene->m_selectedShape]->m_transform.position.x += inc;};
        void virtual moveUp(float inc) override
            {m_scene->m_shapes[m_scene->m_selectedShape]->m_transform.position.y += inc;};
        void virtual moveDown(float inc) override
            {m_scene->m_shapes[m_scene->m_selectedShape]->m_transform.position.y -= inc;};
        
    };
    
    class LightSelect :public SelectObjCommand
    {
        Scene* m_scene;
    public:
        LightSelect(Scene* scene) : m_scene(scene){};
        void virtual changeSelect() override
        {
            m_scene->m_ptLight[m_scene->m_selectedLight].setOutline(false);
            m_scene->m_selectedLight = (m_scene->m_selectedLight + 1) % m_scene->m_ptLight.size();
            m_scene->m_ptLight[m_scene->m_selectedLight].setOutline(true);
        };
        void virtual moveForward(float inc) override
            {m_scene->m_ptLight[m_scene->m_selectedLight].translate(glm::vec3(0.f, 0.f, -inc));};
        void virtual moveBackward(float inc) override
            {m_scene->m_ptLight[m_scene->m_selectedLight].translate(glm::vec3(0.f, 0.f, inc));};
        void virtual moveLeft(float inc) override
            {m_scene->m_ptLight[m_scene->m_selectedLight].translate(glm::vec3(-inc, 0.f, 0.f));};
        void virtual moveRight(float inc) override
            {m_scene->m_ptLight[m_scene->m_selectedLight].translate(glm::vec3(inc, 0.f, 0.f));};
        void virtual moveUp(float inc) override
            {m_scene->m_ptLight[m_scene->m_selectedLight].translate(glm::vec3(0.f, inc, 0.f));};
        void virtual moveDown(float inc) override
            {m_scene->m_ptLight[m_scene->m_selectedLight].translate(glm::vec3(0.f, -inc, 0.f));};

    };
    
    friend class NoSelect;
    friend class ShapeSelect;
    friend class LightSelect;
    
    
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
        
        
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            GLFWCallbackWrapper::m_scene->key_callback(window, key, scancode, action, mods);
        };
        
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
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xInc, double yInc);
    
    
    
    
    
   
};










#endif /* BasicDraw_h */
