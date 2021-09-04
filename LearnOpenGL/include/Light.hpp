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
    float m_uniformScale;
    glm::vec3 m_color;
    
    glm::mat4 m_model;
    static constexpr glm::mat4 id = glm::mat4(1.f);
    
    Box<Vert3f> m_box;
    
    Shader m_shader;
    
    
public:
    glm::vec3 position, color;
    
    PosLight() = default;
    PosLight(const glm::vec3& position, const std::string& vertFilename, const std::string& fragFilename);
    
    void virtual setUniforms(Shader obj_Shader)
    {
        obj_Shader.setUniform3f("light.position", position.x, position.y, position.z);
    };
    void draw(const glm::mat4& view, const glm::mat4& proj);
    
    
    void translate(const glm::vec3& delta);
    void setScale(float uniformScale) {m_uniformScale = uniformScale;};
    
    
    
    
};




class PointLight : public PosLight
{
public:
    glm::vec3 ambient, diffuse, specular;
    float constAtten, linAtten, quadAtten;
    
    PointLight(const glm::vec3 position, const std::string& vertFilename, const std::string& fragFilename,
               float ambFactor = .1f, const glm::vec3& diffuse = glm::vec3(1.f, 1.f, 1.f),
               const glm::vec3& spec = glm::vec3(1.f, 1.f, 1.f), float constAtten = 1.f,
               float linAtten = 0.f, float quadAtten = 0.f) :
    PosLight(position, vertFilename, fragFilename), diffuse(diffuse), specular(spec), constAtten(constAtten),
    linAtten(linAtten), quadAtten(quadAtten)
    {
        ambient = ambFactor*diffuse;
    }
    
    void virtual setUniforms(Shader obj_Shader) override;
    
    void setAmbientDiffuse(const glm::vec3& color, float ambientFactor)
    {
        ambient = ambientFactor * color;
        diffuse = color;
    };
};



class SpotLight : public PointLight
{
    float innerCutoff, outerCutoff;
    
    SpotLight(const glm::vec3 position, const std::string& vertFilename, const std::string& fragFilename,
               float ambFactor = .1f, const glm::vec3& diffuse = glm::vec3(1.f, 1.f, 1.f),
               const glm::vec3& spec = glm::vec3(1.f, 1.f, 1.f), float constAtten = 1.f,
               float linAtten = 0.f, float quadAtten = 0.f, float innerCutoff = glm::radians(45.f),
              float outerCutoff = glm::radians(45.f)) :
    PointLight(position, vertFilename, fragFilename, ambFactor, diffuse, spec, constAtten, linAtten,
               quadAtten), innerCutoff(innerCutoff), outerCutoff(outerCutoff)  {};
    
    
    void virtual setUniforms(Shader obj_Shader) override;
    
};




struct DirectionLight
{
    glm::vec3 direction = glm::vec3(0.f, -1.f, 0.f);
    glm::vec3 ambient = glm::vec3(.2f);
    glm::vec3 diffuse = glm::vec3(1.f);
    glm::vec3 specular = glm::vec3(1.f);
    
    DirectionLight(const glm::vec3& dir, float ambFactor = .2f, const glm::vec3& diffuse = glm::vec3(1.f),
                   const glm::vec3& spec = glm::vec3(1.f)) :
    direction(dir), diffuse(diffuse), specular(spec)
    {
        ambient = diffuse*ambFactor;
    };
    
    void setAmbientDiffuse(const glm::vec3& color, float ambientFactor)
    {
        ambient = ambientFactor * color;
        diffuse = color;
    };
};


struct Flashlight
{
    glm::vec3 position = glm::vec3(1.f, 1.f, -2.f);
    glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
    float innerAngle = glm::cos(glm::radians(45.f));
    float outerAngle = glm::cos(glm::radians(47.f));
    
    glm::vec3 ambient = glm::vec3(.2f);
    glm::vec3 diffuse = glm::vec3(1.f);
    glm::vec3 specular = glm::vec3(1.f);
};

#endif /* Light_h */
