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

#include "Shader.hpp"





struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureUV;
};



struct Texture
{
    const static std::string materialName;
    const static std::string diffuseName;
    const static std::string specName;
    const static std::string normalName;
    const static std::string dispName;
    unsigned int id;
    char unitID;
    std::string typeName;
    std::string path;
};






class Mesh
{
public:
    std::vector<Vertex>          m_vertices;
    std::vector<unsigned int>    m_indices;
    std::vector<Texture>         m_textures;
    
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    
    void Draw(Shader& shader);
    void Draw(Shader& shader, int instances);
    
    void BindVertexArray()
    {
        glBindVertexArray(m_VAO);
    };
    
    
    
private:
    unsigned int m_VAO, m_VBO, m_EBO;
    void setupMesh();
    void setupTextureForDraw(Shader& shader);
    
};



#endif /* Mesh_h */
