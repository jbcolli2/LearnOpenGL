//
//  Light.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/19/21.
//

#include <stdio.h>

#include "Light.hpp"


PosLight::PosLight()
{
    m_uniformScale = 0.05f;
    
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
    radiusName = "radius";
    
    
    
               
    m_box.m_transform.position = m_position;
    m_box.m_transform.scale = glm::vec3(m_uniformScale);
    
}



/*******************  draw(Shader)   ************************************
 * \brief Draw the light box.  This will used an optional passed in Shader, but by
 *      default it will use the solidShader.  Since that is the default, a uniform `color`
 *      will be set, even if not used by the passed shader.  Though it makes sense to always
 *      have the light shader use a color uniform.
 *
 * \param shader - Shader to draw light box
 **************************************************************/
void PosLight::draw(Shader shader)
{
    m_box.m_transform.position = m_position;

    shader.useProgram();
    
    
    shader.setUniform4f("color", m_color.r, m_color.g, m_color.b, 1.f);
        
    m_box.Draw(Shader::solidShader);
    
    shader.stopUseProgram();
}


void PosLight::translate(const glm::vec3& delta)
{
    m_position += delta;
}



/*******************  setAtten   ************************************
 * \brief Set the attenuation values for the light.  Also compute the radius.
 *      This is done by taking the attenuation * max light component and setting equal to
 *      some small value.  We then solve for the distance needed for the light
 *      to reach this small value after attenuation.  We assume the light is zero
 *      beyond this distance.
 **************************************************************///
void PosLight::setAtten(float constant, float lin, float quad)
{
    m_constAtten = constant;
    m_linAtten = lin;
    m_quadAtten = quad;
    
    constexpr float lightCutoffInv = 256.f/5.f;
    float maxIntensity = std::fmax(std::fmax(m_diffuse.x, m_diffuse.y), m_diffuse.z);
    if(quad != 0.f)
    {
        m_radius = -lin + std::sqrtf(lin*lin - 4.f*quad*(constant - lightCutoffInv*maxIntensity));
        m_radius = m_radius/(2.f*quad);
    }
    else
    {
        m_radius = (lightCutoffInv*maxIntensity - constant)/lin;
    }
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
    obj_Shader.setUniform3f(uniformName + posName, m_position.x, m_position.y, m_position.z);
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
    obj_Shader.setUniform3f(uniformName + dirName, m_direction.x, m_direction.y, m_direction.z);
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
    
    obj_Shader.setUniform3f(uniformName + ambName, m_ambient.x, m_ambient.y, m_ambient.z);
    obj_Shader.setUniform3f(uniformName + diffName, m_diffuse.x, m_diffuse.y, m_diffuse.z);
    obj_Shader.setUniform3f(uniformName + specName, m_specular.x, m_specular.y, m_specular.z);
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
    obj_Shader.setUniform1f(uniformName + constName, m_constAtten);
    obj_Shader.setUniform1f(uniformName + linName, m_linAtten);
    obj_Shader.setUniform1f(uniformName + quadName, m_quadAtten);
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
    obj_Shader.setUniform1f(uniformName + innerName, m_innerCutoff);
    obj_Shader.setUniform1f(uniformName + outerName, m_outerCutoff);
}


void PosLight::setUniformRadius(Shader obj_Shader, int index)
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
    obj_Shader.setUniform1f(uniformName + radiusName, m_radius);
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












