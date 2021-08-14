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
    Shader m_shader;
    GLFWwindow* m_window;
    
    Camera m_cam;
    
    
    float m_pitch, m_yaw, m_test=800.f/600.f;
    
    std::vector<glm::vec3> m_positions = {
        glm::vec3(0.f, 0.f, -2.f),
        glm::vec3(-0.2f, 0.7f, -4.f),
//        glm::vec3(0.6f, -1.f, -2.f),
//        glm::vec3(0.45f, 0.f, -9.f),
//        glm::vec3(0.4f, 1.f, -3.f),
//        glm::vec3(-0.3f, -1.f, -12.f),
//        glm::vec3(-0.6f, 0.5f, -3.f)
    };
    
    
public:
    Scene(Shader m_shader, GLFWwindow* window);
    
    void draw();
    
    void processInput();
    
    
    
    
    
   
};










#endif /* BasicDraw_h */
