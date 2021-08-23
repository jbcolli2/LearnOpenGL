//
//  Light.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/19/21.
//

#include <stdio.h>

#include "Light.hpp"


Light::Light(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3 diffuse,
             const glm::vec3& specular, const std::string& vertFilename, const std::string& fragFilename) : m_position(position), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
{
    m_uniformScale = 0.05f;
    
    
    std::vector<Vert3f> vertsBox = {
        Vert3f(-0.5f, -0.5f, 0.5f), // front
        Vert3f(0.5f, 0.5f, 0.5f),
        Vert3f(-0.5f, 0.5f, 0.5f),
        
        Vert3f(-0.5f, -0.5f, 0.5f),
        Vert3f(0.5f, -0.5f, 0.5f),
        Vert3f(0.5f, 0.5f, 0.5f),
        
        Vert3f(-0.5f, -0.5f, -0.5f), // back
        Vert3f(0.5f, 0.5f, -0.5f),
        Vert3f(-0.5f, 0.5f, -0.5f),
        
        Vert3f(-0.5f, -0.5f, -0.5f),
        Vert3f(0.5f, -0.5f, -0.5f),
        Vert3f(0.5f, 0.5f, -0.5f),
        
        Vert3f(-0.5f, 0.5f, 0.5f), //top
        Vert3f(0.5f, 0.5f, 0.5f),
        Vert3f(-0.5f, 0.5f, -0.5f),
        
        Vert3f(0.5f, 0.5f, 0.5f),
        Vert3f(0.5f, 0.5f, -0.5f),
        Vert3f(-0.5f, 0.5f, -0.5f),
        
        Vert3f(-0.5f, -0.5f, 0.5f), //bottom
        Vert3f(0.5f, -0.5f, 0.5f),
        Vert3f(-0.5f, -0.5f, -0.5f),
        
        Vert3f(0.5f, -0.5f, 0.5f),
        Vert3f(0.5f, -0.5f, -0.5f),
        Vert3f(-0.5f, -0.5f, -0.5f),
        
        Vert3f(0.5f, -0.5f, 0.5f), //right
        Vert3f(0.5f, -0.5f, -0.5f),
        Vert3f(0.5f, 0.5f, 0.5f),
        
        Vert3f(0.5f, -0.5f, -0.5f),
        Vert3f(0.5f, 0.5f, -0.5f),
        Vert3f(0.5f, 0.5f, 0.5f),
        
        Vert3f(-0.5f, -0.5f, 0.5f), //left
        Vert3f(-0.5f, -0.5f, -0.5f),
        Vert3f(-0.5f, 0.5f, 0.5f),
        
        Vert3f(-0.5f, -0.5f, -0.5f),
        Vert3f(-0.5f, 0.5f, -0.5f),
        Vert3f(-0.5f, 0.5f, 0.5f)
    };
               
    m_box = Box<Vert3f>(vertsBox);
    
    
    m_model = glm::translate(id, m_position);
    m_model = glm::scale(m_model, glm::vec3(m_uniformScale));
    
    m_shader = Shader(vertFilename, fragFilename);
    m_shader.makeProgram();
    
}




void Light::draw(const glm::mat4& view, const glm::mat4& proj)
{
    m_model = glm::translate(id, m_position);
    m_model = glm::scale(m_model, glm::vec3(0.05f));
    m_view = view;
    m_proj = proj;
    
    m_shader.useProgram();
    
    
    m_shader.setUniform3f("lightColor", m_diffuse.r, m_diffuse.g, m_diffuse.b);
    
    m_shader.setUniformMatrix4f("model", m_model);
    m_shader.setUniformMatrix4f("view", m_view);
    m_shader.setUniformMatrix4f("proj", m_proj);
    
    m_box.draw();
}








void Light::translate(const glm::vec3& delta)
{
    m_position += delta;
}



void Light::setPosition(const glm::vec3& position)
{
    m_position = position;
    m_model = glm::translate(id, m_position);
}


