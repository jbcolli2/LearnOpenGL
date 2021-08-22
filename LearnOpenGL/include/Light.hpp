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
    glm::vec3 m_position;
    float m_uniformScale;
    glm::vec3 m_color;
    
    glm::mat4 m_proj, m_view, m_model;
    static constexpr glm::mat4 id = glm::mat4(1.f);
    
    Box<Vert3f> m_box;
    
    Shader m_shader;
    
    
public:
    Light() = default;
    Light(const glm::vec3& position, const glm::vec3 color, const std::string& vertFilename,
          const std::string& fragFilename);
    
    void draw(const glm::mat4& view, const glm::mat4& proj);
    
    
    void translate(const glm::vec3& delta);
    void setPosition(const glm::vec3& position);
    void setScale(float uniformScale) {m_uniformScale = uniformScale;};
    
    glm::vec3 getPosition() { return m_position;};
};

#endif /* Light_h */
