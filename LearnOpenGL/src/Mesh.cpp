//
//  Mesh.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 9/8/21.
//

#include <stdio.h>

#include "Mesh.hpp"

const std::string Texture::diffuseName = "material.diffuse";
const std::string Texture::specName = "material.specular";


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
m_vertices(vertices), m_indices(indices), m_textures(textures)
{
    setupMesh();
}



void Mesh::setupTextureForDraw(Shader& shader)
{
    unsigned int diffUnit = 0, specUnit = 0;
    
    for(int ii = 0; ii < m_textures.size(); ++ii)
    {
        glActiveTexture(GL_TEXTURE0 + ii);
        m_textures[ii].unitID = ii;
        
        if(m_textures[ii].typeName == Texture::diffuseName)
        {
            shader.setUniform1i(Texture::diffuseName + std::to_string(diffUnit), ii);
            diffUnit++;
        }
        else if(m_textures[ii].typeName == Texture::specName)
        {
            shader.setUniform1i(Texture::specName + std::to_string(specUnit), ii);
            specUnit++;
        }
        
        glBindTexture(GL_TEXTURE_2D, m_textures[ii].id);
    }
}


void Mesh::Draw(Shader& shader)
{
    glBindVertexArray(m_VAO);

    setupTextureForDraw(shader);
    
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void Mesh::Draw(Shader& shader, int instances)
{
    glBindVertexArray(m_VAO);

    setupTextureForDraw(shader);
    
    glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, instances);
    glBindVertexArray(0);
}











void Mesh::setupMesh()
{
    // Setup objects
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    
    
    // Load vertex data
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vertex), &m_vertices.front(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned int), &m_indices.front(), GL_STATIC_DRAW);
    
    // Set the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,position));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,textureUV));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    
}
