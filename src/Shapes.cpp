//
//  Shapes.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 9/14/21.
//

#include <stdio.h>

#include "Shapes.hpp"


// ///////////// addTBtoVerts   ////////////////
/**
 \brief Take the vertex list from a shape and add tangent and bitangent vectors to it.
 
 \param verts A vertex list with position, normal and UV coordinates
 
 \returns A vertex list with position, normal, UV, tangent and bitangent attributes
 */
std::vector<Vert3x3x2x3x3f> addTBtoVerts(std::vector<Vert3x3x2f> verts)
{
  
    std::vector<Vert3x3x2x3x3f> ret(verts.size());
    

    //*********************************************
    //            Create T and B vectors
    //*********************************************
    // Loop through all vertices, 3 at a time, modifying the vertices in each triangle
    for(int vert0 = 0; vert0 < verts.size(); vert0 += 3)
    {
        // ********  Fill in old values  ********** //
        ret[vert0].x = verts[vert0].x;
        ret[vert0].y = verts[vert0].y;
        ret[vert0].z = verts[vert0].z;
        ret[vert0].r = verts[vert0].r;
        ret[vert0].g = verts[vert0].g;
        ret[vert0].b = verts[vert0].b;
        ret[vert0].s = verts[vert0].s;
        ret[vert0].t = verts[vert0].t;
        
        ret[vert0 + 1].x = verts[vert0 + 1].x;
        ret[vert0 + 1].y = verts[vert0 + 1].y;
        ret[vert0 + 1].z = verts[vert0 + 1].z;
        ret[vert0 + 1].r = verts[vert0 + 1].r;
        ret[vert0 + 1].g = verts[vert0 + 1].g;
        ret[vert0 + 1].b = verts[vert0 + 1].b;
        ret[vert0 + 1].s = verts[vert0 + 1].s;
        ret[vert0 + 1].t = verts[vert0 + 1].t;
        
        ret[vert0 + 2].x = verts[vert0 + 2].x;
        ret[vert0 + 2].y = verts[vert0 + 2].y;
        ret[vert0 + 2].z = verts[vert0 + 2].z;
        ret[vert0 + 2].r = verts[vert0 + 2].r;
        ret[vert0 + 2].g = verts[vert0 + 2].g;
        ret[vert0 + 2].b = verts[vert0 + 2].b;
        ret[vert0 + 2].s = verts[vert0 + 2].s;
        ret[vert0 + 2].t = verts[vert0 + 2].t;
        
        
        // ********  Setup known variables  ********** //
        // Compute edge vectors between vertex 0 -> 1...
        glm::vec3 edge01 = glm::vec3(verts[vert0 + 1].x - verts[vert0].x,
                                     verts[vert0 + 1].y - verts[vert0].y,
                                     verts[vert0 + 1].z - verts[vert0].z);
        // ... and then 1 -> 2
        glm::vec3 edge12 = glm::vec3(verts[vert0 + 2].x - verts[vert0 + 1].x,
                                     verts[vert0 + 2].y - verts[vert0 + 1].y,
                                     verts[vert0 + 2].z - verts[vert0 + 1].z);
        
        // Compute delta U/V between 0 -> 1 ...
        float deltaU01 = verts[vert0 + 1].s - verts[vert0].s;
        float deltaV01 = verts[vert0 + 1].t - verts[vert0].t;
        // ... and then 1 -> 2
        float deltaU12 = verts[vert0 + 2].s - verts[vert0 + 1].s;
        float deltaV12 = verts[vert0 + 2].t - verts[vert0 + 1].t;
        
        
        // ********  Compute T and B vectors  ********** //
        float detInv = 1.f/(deltaU01*deltaV12 - deltaU12*deltaV01);
        ret[vert0].x1 = detInv*(deltaV12*edge01.x + (-deltaV01*edge12.x));
        ret[vert0].y1 = detInv*(deltaV12*edge01.y + (-deltaV01*edge12.y));
        ret[vert0].z1 = detInv*(deltaV12*edge01.z + (-deltaV01*edge12.z));
        
        ret[vert0].x2 = detInv*((-deltaU12*edge01.x) + deltaU01*edge12.x);
        ret[vert0].y2 = detInv*((-deltaU12*edge01.y) + deltaU01*edge12.y);
        ret[vert0].z2 = detInv*((-deltaU12*edge01.z) + deltaU01*edge12.z);
        
        // ********  Fill in other verts with same T and B vectors  ********** //
        ret[vert0 + 1].x1 = ret[vert0].x1;
        ret[vert0 + 1].y1 = ret[vert0].y1;
        ret[vert0 + 1].z1 = ret[vert0].z1;
        ret[vert0 + 1].x2 = ret[vert0].x2;
        ret[vert0 + 1].y2 = ret[vert0].y2;
        ret[vert0 + 1].z2 = ret[vert0].z2;
        
        ret[vert0 + 2].x1 = ret[vert0].x1;
        ret[vert0 + 2].y1 = ret[vert0].y1;
        ret[vert0 + 2].z1 = ret[vert0].z1;
        ret[vert0 + 2].x2 = ret[vert0].x2;
        ret[vert0 + 2].y2 = ret[vert0].y2;
        ret[vert0 + 2].z2 = ret[vert0].z2;
    }
    
    return ret;
}


void Shape::setupMesh(const std::vector<std::string>& diffTexturePaths,
                      const std::vector<std::string>& specTexturePaths,
                      const std::vector<std::string>& normalTexturePaths,
                      const std::vector<std::string>& dispTexturePaths,
                      const Material& material)
{
    
    m_numVerts = m_verts.size();
        
        
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    // If there is a normal map, add TB to the attributes
    if(normalTexturePaths.size() > 0)
    {
        m_VBO = loadVBOData(addTBtoVerts(m_verts));
    }
    // otherwise, just send the 3x3x2 vert data to the VBO
    else
    {
        m_VBO = loadVBOData(m_verts);
    }
        
    
    

    
    
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
    for(auto normalPath : normalTexturePaths)
    {
        loadTexture(normalPath, Texture::normalName);
    }
    for(auto dispPath : dispTexturePaths)
    {
        loadTexture(dispPath, Texture::dispName);
    }

}




void Shape::Draw(Shader shader, int instances)
{
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
    unsigned int diffUnit=0, specUnit=0, normalUnit = 0, dispUnit = 0;;
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
        if(tempTexture.typeName == Texture::normalName)
        {
            shader.setUniform1i(Texture::normalName + std::to_string(normalUnit), ii);
            normalUnit++;
        }
        if(tempTexture.typeName == Texture::dispName)
        {
            shader.setUniform1i(Texture::dispName + std::to_string(dispUnit), ii);
            dispUnit++;
        }
        
        glBindTexture(GL_TEXTURE_2D, m_textures[ii].id);
    }
    shader.setUniform1f(Texture::materialName + ".shininess", m_material.shininess);
    
    
    glBindVertexArray(m_VAO);
    if(instances == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
    }
    else
    {
        glDrawArraysInstanced(GL_TRIANGLES, 0, m_numVerts, instances);
    }
    
    
    if(m_outlined)
    {
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        
        model = glm::scale(model, glm::vec3(1.05));
        Shader::solidShader.useProgram();
        Shader::solidShader.setUniform3f("color", 1.f, .2f, .8f);
        Shader::solidShader.setUniformMatrix4f("model", model);
        glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
        
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_DEPTH_TEST);
    }
    glBindVertexArray(0);
}





//*********************************************
//            Points
//*********************************************


Points::Points(std::vector<Vert3x3f> positions) : m_positions(positions)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    m_VBO = loadVBOData(m_positions);
}


void Points::Draw(Shader shader, int instances)
{
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_POINTS, 0, m_positions.size());
}







//*********************************************
//            Line class
//*********************************************
Line::Line(std::vector<Vert3x3f> positions) : m_positions(positions)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    m_VBO = loadVBOData(m_positions);
}


void Line::Draw(Shader shader, int instances)
{
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINE_STRIP, 0, m_positions.size());
}




//*********************************************
//            Plane class
//*********************************************


Plane::Plane(const std::vector<std::string>& diffTexturePaths, const std::vector<std::string>& specTexturePaths,
             const std::vector<std::string>& normalTexturePaths, const std::vector<std::string>& dispTexturePaths,
             float UVCorner, const Material& material) : m_UVCorner(UVCorner)
{
    m_verts = fillVerts();
    
    
    setupMesh(diffTexturePaths, specTexturePaths, normalTexturePaths, dispTexturePaths, material);

}




Plane::Plane(const Material& material)
{
    
    std::vector<std::string> blank;
    
    Plane(blank, blank, blank, blank, 1.f, material);

}





Plane::Plane(const Plane& otherPlane)
{
    m_VBO = otherPlane.m_VBO;
    m_verts = otherPlane.m_verts;
    m_numVerts = otherPlane.m_numVerts;
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
        Vert3x3x2f(0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, m_UVCorner, 0.0f),
        Vert3x3x2f(-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, m_UVCorner),
        
        Vert3x3x2f(0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, m_UVCorner, 0.0f),
        Vert3x3x2f(0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, m_UVCorner, m_UVCorner),
        Vert3x3x2f(-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, m_UVCorner),
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
           const std::vector<std::string>& normalTexturePaths, const std::vector<std::string>& dispTexturePaths,
           const Material& material)
{
    m_verts = fillVerts();
    
    setupMesh(diffTexturePaths, specTexturePaths, normalTexturePaths, dispTexturePaths, material);

}




Cube::Cube(const Material& material)
{
    
    std::vector<std::string> blank;
    
    Cube(blank, blank, blank, blank, material);

}





Cube::Cube(const Cube& otherCube)
{
    m_VBO = otherCube.m_VBO;
    m_verts = otherCube.m_verts;
    m_numVerts = otherCube.m_numVerts;
    m_material = otherCube.m_material;
    m_textures = otherCube.m_textures;
    
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    rglVertexAttribPointer(m_verts[0]);
}




