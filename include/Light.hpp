//
//  Light.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 8/19/21.
//

#ifndef Light_h
#define Light_h
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shapes.hpp"
#include "Shader.hpp"
#include "VertexData.hpp"



class PosLight
{
protected:
    
    
    glm::mat4 m_model;
    static constexpr glm::mat4 id = glm::mat4(1.f);
    
    Cube m_box;
    
    glm::vec3 m_ambient{.15f}, m_diffuse{.6f}, m_specular{.3f};
    glm::vec3 m_color{1.f};
    
    std::string structName;
    std::string posName, dirName, ambName, diffName, specName, constName, linName, quadName, innerName, outerName;
    
public:
    glm::vec3 m_position{0.f}, m_direction{0.f, 0.f, -1.f};
    
    float m_constAtten{1.f}, m_linAtten{.1f}, m_quadAtten{.02f};
    float m_innerCutoff{glm::cos(glm::radians(12.f))}, m_outerCutoff{glm::cos(glm::radians(17.f))};
    
    float m_uniformScale{.05f};
    
    
    
    
    PosLight();
    
    void setAmbient(float ambFactor) {m_ambient = m_diffuse*ambFactor;};
    void setDiffuse(const glm::vec3& diff) {m_diffuse = diff; m_color = diff;};
    void setSpecular(float specIntensity) {m_specular = glm::vec3(specIntensity);};
    
    void setUniformPos(Shader obj_Shader, int index = -1);
    void setUniformDir(Shader obj_Shader, int index = -1);
    void setUniformColor(Shader obj_Shader, int index = -1);
    void setUniformAtten(Shader obj_Shader, int index = -1);
    void setUniformCutoff(Shader obj_Shader, int index = -1);
    
    void setUniformDirLight(Shader obj_Shader, int index = -1);
    void setUniformPtLight(Shader obj_Shader, int index = -1);
    void setUniformSpotLight(Shader obj_Shader, int index = -1);
    
    void setOutline(bool outlineFlag) {m_box.m_outlined = outlineFlag;};
    void setColor(glm::vec3 color) {m_color = color;};
    
    void draw();
    
    void setInnerCutoff(float innerDeg) {m_innerCutoff = glm::cos(glm::radians(innerDeg));
        if(m_outerCutoff > m_innerCutoff) m_outerCutoff = m_innerCutoff - .02;
    };
    void setOuterCutoff(float outerDeg) {m_outerCutoff = glm::cos(glm::radians(outerDeg));};
    
    
    void translate(const glm::vec3& delta);
    void setScale(float uniformScale) {m_uniformScale = uniformScale;};
    
    
    
    
};


class DirLight : public PosLight
{
public:
    DirLight() = default;
    DirLight(const glm::vec3& dir)
    : PosLight()
    {
        structName = "dirLights";
        
        m_direction = dir;
    };
};



class PointLight : public PosLight
{
public:
    PointLight() {structName = "ptLights";};
    PointLight(const glm::vec3& position)
    : PosLight()
    {
        structName = "ptLights";
        
        m_position = position;
    };
};


class SpotLight : public PosLight
{
public:
    SpotLight() {structName = "spotLights";};
    SpotLight(const glm::vec3& position, const glm::vec3& direction,
               float innerCutoff = glm::cos(glm::radians(89.f)), float outerCutoff = glm::cos(glm::radians(90.f)))
    : PosLight()
    {
        SpotLight();
        
        m_position = position;
        m_direction = direction;
        m_innerCutoff = innerCutoff;
        m_outerCutoff = outerCutoff;
    };
};




//typedef PosLight DirLight, PointLight, SpotLight;



#endif /* Light_h */
