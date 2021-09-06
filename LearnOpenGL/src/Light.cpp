//
//  Light.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/19/21.
//

#include <stdio.h>

#include "Light.hpp"


PosLight::PosLight(const std::string& vertFilename, const std::string& fragFilename, const glm::vec3& position,
                   const glm::vec3& dir, float ambFactor, const glm::vec3& diffuse, const glm::vec3& spec, float constAtten,
                   float linAtten, float, float innerCutoff, float outerCutoff) :
position(position), direction(dir), diffuse(diffuse), specular(spec), constAtten(constAtten),
linAtten(linAtten), quadAtten(quadAtten), innerCutoff(innerCutoff), outerCutoff(outerCutoff)
{
    ambient = ambFactor * diffuse;
    m_uniformScale = 0.05f;
    m_color = diffuse;
    
    structName = "light";
    posName = "position";
    dirName = "direction";
    ambName = "ambient";
    diffName = "diffuse";
    specName = "specular";
    constName = "constAtten";
    linName = "linAtten";
    quadName = "quadAtten";
    innerName = "innerCutoff";
    outerName = "outerCutoff";
    
    
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
    
    
    m_shader.setUniform3f("lightColor", m_color.r, m_color.g, m_color.b);
    
    m_shader.setUniformMatrix4f("model", m_model);
    m_shader.setUniformMatrix4f("view", view);
    m_shader.setUniformMatrix4f("proj", proj);
    
    m_box.draw();
    
    m_shader.stopUseProgram();
}


void PosLight::translate(const glm::vec3& delta)
{
    position += delta;
}








void PosLight::setUniformPos(Shader obj_Shader, int index)
{
    std::string uniformName;
    if(index < 0)
    {
        uniformName = structName + ".";
    }
    else
    {
        uniformName = structName + "[" + std::to_string(index) + "].";
    }
    obj_Shader.setUniform3f(uniformName + posName, position.x, position.y, position.z);
}


void PosLight::setUniformDir(Shader obj_Shader, int index)
{
    std::string uniformName;
    if(index < 0)
    {
        uniformName = structName + ".";
    }
    else
    {
        uniformName = structName + "[" + std::to_string(index) + "].";
    }
    obj_Shader.setUniform3f(uniformName + dirName, direction.x, direction.y, direction.z);
}


void PosLight::setUniformColor(Shader obj_Shader, int index)
{
    std::string uniformName;
    if(index < 0)
    {
        uniformName = structName + ".";
    }
    else
    {
        uniformName = structName + "[" + std::to_string(index) + "].";
    }
    obj_Shader.setUniform3f(uniformName + ambName, ambient.x, ambient.y, ambient.z);
    obj_Shader.setUniform3f(uniformName + diffName, diffuse.x, diffuse.y, diffuse.z);
    obj_Shader.setUniform3f(uniformName + specName, specular.x, specular.y, specular.z);
}


void PosLight::setUniformAtten(Shader obj_Shader, int index)
{
    std::string uniformName;
    if(index < 0)
    {
        uniformName = structName + ".";
    }
    else
    {
        uniformName = structName + "[" + std::to_string(index) + "].";
    }
    obj_Shader.setUniform1f(uniformName + constName, constAtten);
    obj_Shader.setUniform1f(uniformName + linName, linAtten);
    obj_Shader.setUniform1f(uniformName + quadName, quadAtten);
}


void PosLight::setUniformCutoff(Shader obj_Shader, int index)
{
    std::string uniformName;
    if(index < 0)
    {
        uniformName = structName + ".";
    }
    else
    {
        uniformName = structName + "[" + std::to_string(index) + "].";
    }
    obj_Shader.setUniform1f(uniformName + innerName, innerCutoff);
    obj_Shader.setUniform1f(uniformName + outerName, outerCutoff);
}


void PosLight::setUniformDirLight(Shader obj_Shader, int index)
{
    setUniformDir(obj_Shader, index);
    setUniformColor(obj_Shader, index);
}

void PosLight::setUniformPtLight(Shader obj_Shader, int index)
{
    setUniformPos(obj_Shader, index);
    setUniformColor(obj_Shader, index);
    setUniformAtten(obj_Shader, index);
}


void PosLight::setUniformSpotLight(Shader obj_Shader, int index)
{
    setUniformPtLight(obj_Shader, index);
    setUniformDir(obj_Shader, index);
    setUniformCutoff(obj_Shader, index);
}












