//
//  Skybox.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 10/20/21.
//

#ifndef Skybox_h
#define Skybox_h

#include <vector>
#include <string>
#include <iostream>

#include <stb_image.h>

#include "Utilities/OpenGLUtil.hpp"
#include "Rendering/Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// ///////////// Skybox Class   ////////////////


class Skybox
{
    unsigned int m_tbo, m_vao, m_vbo;
    float m_yaw = 0;

    glm::mat4 m_model;
    std::vector<Vert3f> m_box = {
        Vert3f(-0.5f, -0.5f, 0.5f), // front
        Vert3f(0.5f, 0.5f, 0.5f),
        Vert3f(-0.5f, 0.5f, 0.5f),
      
        Vert3f(-0.5f, -0.5f, 0.5f),
        Vert3f(0.5f, -0.5f, 0.5f),
        Vert3f(0.5f, 0.5f, 0.5f),
        
        Vert3f(-0.5f, -0.5f, -0.5f), // back
        Vert3f(-0.5f, 0.5f, -0.5f),
        Vert3f(0.5f, 0.5f, -0.5f),
        
        Vert3f(-0.5f, -0.5f, -0.5f),
        Vert3f(0.5f, 0.5f, -0.5f),
        Vert3f(0.5f, -0.5f, -0.5f),
        
        Vert3f(-0.5f, 0.5f, 0.5f), //top
        Vert3f(0.5f, 0.5f, 0.5f),
        Vert3f(-0.5f, 0.5f, -0.5f),
        
        Vert3f(0.5f, 0.5f, 0.5f),
        Vert3f(0.5f, 0.5f, -0.5f),
        Vert3f(-0.5f, 0.5f, -0.5f),
        
        Vert3f(-0.5f, -0.5f, 0.5f), //bottom
        Vert3f(-0.5f, -0.5f, -0.5f),
        Vert3f(0.5f, -0.5f, 0.5f),
        
        Vert3f(0.5f, -0.5f, 0.5f),
        Vert3f(-0.5f, -0.5f, -0.5f),
        Vert3f(0.5f, -0.5f, -0.5f),
        
        Vert3f(0.5f, -0.5f, 0.5f), //right
        Vert3f(0.5f, -0.5f, -0.5f),
        Vert3f(0.5f, 0.5f, 0.5f),
        
        Vert3f(0.5f, -0.5f, -0.5f),
        Vert3f(0.5f, 0.5f, -0.5f),
        Vert3f(0.5f, 0.5f, 0.5f),
        
        Vert3f(-0.5f, -0.5f, 0.5f), //left
        Vert3f(-0.5f, 0.5f, 0.5f),
        Vert3f(-0.5f, -0.5f, -0.5f),
        
        Vert3f(-0.5f, -0.5f, -0.5f),
        Vert3f(-0.5f, 0.5f, 0.5f),
        Vert3f(-0.5f, 0.5f, -0.5f)
    };
    
    
public:
    Skybox() = default;
    Skybox(unsigned int tbo);
    Skybox(std::vector<std::string> texFilenames);
    
    void Draw(Shader shader);
    
    void RotateRight(float inc) {m_yaw += inc;};
    void RotateLeft(float inc) {m_yaw -= inc;};
    
    void SetCubemap(unsigned int tbo) {m_tbo = tbo;};
};






#endif /* Skybox_h */
