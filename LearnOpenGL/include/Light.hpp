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



class Light
{
private:
    glm::vec3 m_position, m_ambient, m_diffuse, m_specular;
    float m_uniformScale;
    glm::vec3 m_color;
    
    glm::mat4 m_proj, m_view, m_model;
    static constexpr glm::mat4 id = glm::mat4(1.f);
    
    Box<Vert3f> m_box;
    
    Shader m_shader;
    
    
public:
    Light() = default;
    Light(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3 diffuse,
          const glm::vec3& specular, const std::string& vertFilename, const std::string& fragFilename);
    
    void draw(const glm::mat4& view, const glm::mat4& proj);
    
    
    void translate(const glm::vec3& delta);
    void setPosition(const glm::vec3& position);
    void setScale(float uniformScale) {m_uniformScale = uniformScale;};
    
    void setAmbientDiffuse(const glm::vec3& color, float ambientFactor)
    {
        m_ambient = ambientFactor * color;
        m_diffuse = color;
    };
    void setSpecular(const glm::vec3& color)
    {
        m_specular = color;
    };
    
    glm::vec3 getPosition() { return m_position;};
    glm::vec3 getAmbient() {return m_ambient;};
    glm::vec3 getDiffuse() {return m_diffuse;};
    glm::vec3 getSpecular() {return m_specular;};
};




struct DirectionLight
{
    glm::vec3 direction = glm::vec3(0.f, -1.f, 0.f);
    glm::vec3 ambient = glm::vec3(.1f);
    glm::vec3 diffuse = glm::vec3(1.f);
    glm::vec3 specular = glm::vec3(1.f);
};


struct Flashlight
{
    glm::vec3 position = glm::vec3(1.f, 1.f, -2.f);
    glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
    float cutoffAngle = glm::cos(glm::radians(45.f));
    
    glm::vec3 ambient = glm::vec3(.2f);
    glm::vec3 diffuse = glm::vec3(1.f);
    glm::vec3 specular = glm::vec3(1.f);
};

#endif /* Light_h */
