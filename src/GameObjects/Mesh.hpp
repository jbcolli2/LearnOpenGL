//
//  Mesh.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 9/7/21.
//

#ifndef Mesh_h
#define Mesh_h

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Rendering/Shader.hpp"
#include "Utilities/OpenGLUtil.hpp"





struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureUV;
};









class Mesh
{
public:
    std::vector<Vertex>          m_vertices;
    std::vector<unsigned int>    m_indices;
    std::vector<Texture>         m_textures;
    
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, bool specMap = false);
    
    void Draw(Shader& shader);
    void Draw(Shader& shader, int instances);
    
    void BindVertexArray()
    {
        glBindVertexArray(m_VAO);
    };
    
    
    
private:
    unsigned int m_VAO, m_VBO, m_EBO;
    bool m_isSpecMap{false};
    void setupMesh();
    void setupTextureForDraw(Shader& shader);
    
};



#endif /* Mesh_h */
