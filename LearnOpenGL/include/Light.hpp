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
    
    Cube m_box;
    
    Shader m_shader;
    
    
public:
    glm::vec3 position, direction;
    glm::vec3 ambient, diffuse, specular;
    float constAtten, linAtten, quadAtten;
    float innerCutoff, outerCutoff;
    
    std::string structName;
    std::string posName, dirName, ambName, diffName, specName, constName, linName, quadName, innerName, outerName;
    
    PosLight() = default;
    PosLight(const std::string& vertFilename, const std::string& fragFilename, const glm::vec3& position = glm::vec3(0.f),
             const glm::vec3& dir = glm::vec3(0.f, 0.f, -1.f), float ambFactor = .3f,
             const glm::vec3& diffuse = glm::vec3(1.f), const glm::vec3& spec = glm::vec3(1.f), float constAtten = 1.f,
             float linAtten = 0.1f, float quadAtten = 0.02f, float innerCutoff = glm::cos(glm::radians(12.f)),
            float outerCutoff = glm::cos(glm::radians(17.f)));
    
    
    
    void setUniformPos(Shader obj_Shader, int index = -1);
    void setUniformDir(Shader obj_Shader, int index = -1);
    void setUniformColor(Shader obj_Shader, int index = -1);
    void setUniformAtten(Shader obj_Shader, int index = -1);
    void setUniformCutoff(Shader obj_Shader, int index = -1);
    
    void setUniformDirLight(Shader obj_Shader, int index = -1);
    void setUniformPtLight(Shader obj_Shader, int index = -1);
    void setUniformSpotLight(Shader obj_Shader, int index = -1);
    
    void draw(const glm::mat4& view, const glm::mat4& proj);
    
    
    void translate(const glm::vec3& delta);
    void setScale(float uniformScale) {m_uniformScale = uniformScale;};
    
    
    
    
};

typedef PosLight DirLight, PointLight, SpotLight;



#endif /* Light_h */
