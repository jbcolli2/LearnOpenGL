//
//  Shapes.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 9/14/21.
//

#include <stdio.h>

#include "Shapes.hpp"






void Shape::setupMesh(const std::vector<std::string>& diffTexturePaths,
                      const std::vector<std::string>& specTexturePaths, const Material& material)
{
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    m_VBO = loadVBOData(m_verts);
    

    
    
    //Setup material for cube
    m_material = material;
    
    for(auto diffPath : diffTexturePaths)
    {
        loadTexture(diffPath, Texture::diffuseName);
    }
    for(auto specPath : specTexturePaths)
    {
        loadTexture(specPath, Texture::specName);
    }

}




void Shape::Draw(Shader shader)
{
    shader.useProgram();
    // Setup the model matrix
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, m_transform.position);
    model = glm::rotate(model, glm::radians(m_transform.rotation.x), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(m_transform.rotation.y), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(m_transform.rotation.z), glm::vec3(0.f, 0.f, 1.f));
    model = glm::scale(model, m_transform.scale);
    shader.setUniformMatrix4f("model", model);
    
    
    
    // SEtup the textures for the shape
    Texture tempTexture;
    unsigned int diffUnit=0, specUnit=0;
    for (int ii = 0; ii < m_textures.size(); ++ii)
    {
        glActiveTexture(GL_TEXTURE0 + ii);
        
        m_textures[ii].unitID = ii;
        

        tempTexture = m_textures[ii];
        if(tempTexture.typeName == Texture::diffuseName)
        {
            shader.setUniform1i(Texture::diffuseName + std::to_string(diffUnit), ii);
            diffUnit++;
        }
        if(tempTexture.typeName == Texture::specName)
        {
            shader.setUniform1i(Texture::specName + std::to_string(specUnit), ii);
            specUnit++;
        }
        
        glBindTexture(GL_TEXTURE_2D, m_textures[ii].id);
    }
    
    
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_verts.size());
    
    if(m_outlined)
    {
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        
        model = glm::scale(model, glm::vec3(1.05));
        Shader::solidShader.useProgram();
        Shader::solidShader.setUniform3f("color", 1.f, .2f, .8f);
        Shader::solidShader.setUniformMatrix4f("model", model);
        glDrawArrays(GL_TRIANGLES, 0, m_verts.size());
        
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_DEPTH_TEST);
    }
    glBindVertexArray(0);
}


Plane::Plane(const std::vector<std::string>& diffTexturePaths, const std::vector<std::string>& specTexturePaths,
           const Material& material)
{
    m_verts = fillVerts();
    
    setupMesh(diffTexturePaths, specTexturePaths, material);

}




Plane::Plane(const Material& material)
{
    
    std::vector<std::string> blank;
    
    Plane(blank, blank, material);

}





Plane::Plane(const Plane& otherPlane)
{
    m_VBO = otherPlane.m_VBO;
    m_verts = otherPlane.m_verts;
    m_material = otherPlane.m_material;
    m_textures = otherPlane.m_textures;
    
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    rglVertexAttribPointer(m_verts[0]);
}




std::vector<Vert3x3x2f> Plane::fillVerts(std::vector<Vert3x3x2f> verts)
{
    std::vector<Vert3x3x2f> temp = {
        Vert3x3x2f(-0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f), //top
        Vert3x3x2f(0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
    };
    
    
    return temp;
}








std::vector<Vert3x3x2f> Cube::fillVerts(std::vector<Vert3x3x2f> verts)
{
    std::vector<Vert3x3x2f> temp = {
        Vert3x3x2f(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // front
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
        Vert3x3x2f(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f), // back
        Vert3x3x2f(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
        Vert3x3x2f(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        Vert3x3x2f(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
        
        Vert3x3x2f(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f), //top
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f), //bottom
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
        Vert3x3x2f(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
        
        Vert3x3x2f(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
        Vert3x3x2f(0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),
        
        Vert3x3x2f(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f), //right
        Vert3x3x2f(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vert3x3x2f(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f), //left
        Vert3x3x2f(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        
        Vert3x3x2f(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vert3x3x2f(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        Vert3x3x2f(-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        
    };
    
    
    return temp;
}



Cube::Cube(const std::vector<std::string>& diffTexturePaths, const std::vector<std::string>& specTexturePaths,
           const Material& material)
{
    m_verts = fillVerts();
    
    setupMesh(diffTexturePaths, specTexturePaths, material);

}




Cube::Cube(const Material& material)
{
    
    std::vector<std::string> blank;
    
    Cube(blank, blank, material);

}





Cube::Cube(const Cube& otherCube)
{
    m_VBO = otherCube.m_VBO;
    m_verts = otherCube.m_verts;
    m_material = otherCube.m_material;
    m_textures = otherCube.m_textures;
    
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    rglVertexAttribPointer(m_verts[0]);
}




