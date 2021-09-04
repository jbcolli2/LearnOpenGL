//
//  Light.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/19/21.
//

#include <stdio.h>

#include "Light.hpp"


PosLight::PosLight(const glm::vec3& position, const std::string& vertFilename, const std::string& fragFilename) : position(position)
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
    
    
    m_model = glm::translate(id, position);
    m_model = glm::scale(m_model, glm::vec3(m_uniformScale));
    
    m_shader = Shader(vertFilename, fragFilename);
    m_shader.makeProgram();
    
}




void PosLight::draw(const glm::mat4& view, const glm::mat4& proj)
{
    m_model = glm::translate(id, position);
    m_model = glm::scale(m_model, glm::vec3(m_uniformScale));
    
    m_shader.useProgram();
    
    
    m_shader.setUniform3f("lightColor", color.r, color.g, color.b);
    
    m_shader.setUniformMatrix4f("model", m_model);
    m_shader.setUniformMatrix4f("view", view);
    m_shader.setUniformMatrix4f("proj", proj);
    
    m_box.draw();
    
    m_shader.stopUseProgram();
}








void PosLight::translate(const glm::vec3& delta)
{
    m_position += delta;
}













void  PointLight::setUniforms(Shader obj_Shader)
{
    PosLight::setUniforms(obj_Shader);
    obj_Shader.setUniform3f("light.ambient", ambient.r, ambient.g, ambient.b);
    obj_Shader.setUniform3f("light.diffuse", diffuse.r, diffuse.g, diffuse.b);
    obj_Shader.setUniform3f("light.specular", specular.r, specular.g, specular.b);
    
    obj_Shader.setUniform1f("light.constAtten", constAtten);
    obj_Shader.setUniform1f("light.linAtten", linAtten);
    obj_Shader.setUniform1f("light.quadAtten", quadAtten);
    
}






void SpotLight::setUniforms(Shader obj_Shader)
{
    PointLight::setUniforms(obj_Shader);
    obj_Shader.setUniform1f("light.outerCutoff", outerCutoff);
    obj_Shader.setUniform1f("light.innerCutoff", innerCutoff);
}





