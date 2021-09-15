//
//  Shapes.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#ifndef Shapes_h
#define Shapes_h
#define GL_SILENCE_DEPRECATION

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include "OpenGLUtil.hpp"
#include "Mesh.hpp"



struct Material
{
    glm::vec3 ambient = glm::vec3(1.f);
    glm::vec3 diffuse = glm::vec3(1.f);
    glm::vec3 specular = glm::vec3(1.f);
    
    Texture diffuseTexture;
    Texture specTexture;
    float shininess = 1;
};






class Shape
{
    
protected:
    unsigned int m_VAO, m_VBO;
    std::vector<unsigned int> m_textures;
    
    Material m_material;

    
public:
    Shape() {};
    virtual ~Shape() {};
    
    Material getMaterial() {return m_material;};
    void virtual draw() = 0;
  
    
    
    unsigned int loadTexture(std::string filename)
    {
        unsigned int texID = 0;
        int width, height, nrChannels;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
        unsigned int rgbFlag;
        if(nrChannels == 3)
        {
            rgbFlag = GL_RGB;
        }
        else if(nrChannels == 4)
        {
            rgbFlag = GL_RGBA;
        }
        else
        {
            std::cout << "Number of channels in image is not 3 or 4\n";
            return 0;
        }
        if(data)
        {
            

            
            glGenTextures(1, &texID);
            glActiveTexture(GL_TEXTURE0 + texUnit);
            glBindTexture(GL_TEXTURE_2D, texID);
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rgbFlag, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            stbi_image_free(data);
            
            m_textures.push_back(texID);
        }
        else
        {
            std::cout << "Failure to load texture " << filename << std::endl;
        }
        
        return texID;
    }
    
    
    
    
    void loadAmbDiffTexture(std::string filename, unsigned int texUnit)
    {
        unsigned int texID = loadTexture(filename, texUnit);
        if(texID == 0)
        {
            std::cout << "Failed to load Ambient/Diffuse texture\n";
            return;
        }
        
        m_material.ambdiffTexID = texID;
        m_material.ambdiffTexUnit = texUnit;
    }
    
    
    
    void loadSpecTexture(std::string filename, unsigned int texUnit)
    {
        unsigned int texID = loadTexture(filename, texUnit);
        if(texID == 0)
        {
            std::cout << "Failed to load Specular texture\n";
            return;
        }
        
        m_material.specTexID = texID;
        m_material.specTexUnit = texUnit;
    }
    
    void setAmbDiffTexUnit(unsigned int texUnit)
    {
        if(m_material.ambdiffTexID == 0)
        {
            std::cout << "Trying to set Amb/Diff tex unit before it is loaded\n";
        }
        else
        {
            m_material.ambdiffTexUnit = texUnit;
            glActiveTexture(GL_TEXTURE0 + texUnit);
            glBindTexture(GL_TEXTURE_2D, m_material.ambdiffTexID);
        }
  
    }
    
    
    
    void setSpecTexUnit(unsigned int texUnit)
    {
        if(m_material.specTexID == 0)
        {
            std::cout << "Trying to set Amb/Diff tex unit before it is loaded\n";
        }
        else
        {
            m_material.specTexUnit = texUnit;
            glActiveTexture(GL_TEXTURE0 + texUnit);
            glBindTexture(GL_TEXTURE_2D, m_material.specTexID);
        }
  
    }
    
    
    void bindTexture(int i = 0, GLenum target = GL_TEXTURE_2D)
    {
        if(m_textures.size() > i)
        {
            glBindTexture(target, m_textures[i]);
        }
        else
        {
            std::cout << "Error binding texture, "<< i <<  "is bigger than number of textures\n";
        }
    }
};








class Triangle : public Shape
{

    std::vector<Vert3x3x2f> verts;
public:
    Triangle(std::vector<Vert3x3x2f> vert);
    
    void virtual draw() override;
};




class Plane : public Shape
{
    std::vector<Vert3x3x2f> verts;
    std::vector<unsigned int> indices = {0,1,2, 0, 2,3};

    
public:
    Plane(std::vector<Vert3x3x2f> vert, bool clockwise = true);
    
    
    void virtual draw() override;
};




class Cube : public Shape
{
    float m_width, m_length, m_height;
    std::vector<Vert3x3x2f> m_verts;
    
    
    
public:
    Cube();
//    Box(std::vector<VertT> verts);
    Cube(std::vector<Vert3x3x2f> verts, Material material = Material());
    Cube(const Cube& otherBox);
    
    void virtual draw() override;
};







































template <class VertT>
Triangle<VertT>::Triangle(std::vector<VertT> vert)
{
    verts = vert;
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    loadVBOData(verts);

    rglVertexAttribPointer(vert[0]);
    
    
    glEnableVertexAttribArray(0);
    //********* m_VAO **************
}





template <class VertT>
void Triangle<VertT>::draw()
{
    glBindVertexArray(m_VAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}






template <class VertT>
Square<VertT>::Square(std::vector<VertT> vert, bool clockwise)
{
    this->verts = vert;
    
    
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    loadVBOData(verts);
    
    loadEBOData(indices);

    rglVertexAttribPointer(vert[0]);
    
    
    
    //********* m_VAO **************
    
}





template <class VertT>
void Square<VertT>::draw()
{
    glBindVertexArray(m_VAO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}






//-----------  Box  ---------------------//
template <typename VertT>
Box<VertT>::Box()
{
    VertT vert;
    m_verts.push_back(vert);
    
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    m_VBO = loadVBOData(m_verts);
    

    rglVertexAttribPointer(m_verts[0]);
    
    
    
    //********* VAO **************
}





//template <class VertT>
//Box<VertT>::Box(std::vector<VertT> verts) : m_verts(verts)
//{
//    m_material.ambient = glm::vec3(.5f);
//    m_material.diffuse = glm::vec3(1.f);
//    m_material.specular = glm::vec3(0.f);
//    m_material.shininess = 1;
//
//    //******* VBO/VAO   ***************
//    glGenVertexArrays(1, &m_VAO);
//    glBindVertexArray(m_VAO);
//
//    m_VBO = loadVBOData(m_verts);
//
//
//    rglVertexAttribPointer(m_verts[0]);
//
//
//
//    //********* VAO **************
//
//
//}


template <typename VertT>
Box<VertT>::Box(std::vector<VertT> verts, Material material) : m_verts(verts)
{
    m_material = material;
    
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    m_VBO = loadVBOData(m_verts);
    

    rglVertexAttribPointer(m_verts[0]);
    
    
    
    //********* VAO **************
}



template <class VertT>
Box<VertT>::Box(const Box& otherBox)
{
    m_VBO = otherBox.m_VBO;
    m_verts = otherBox.m_verts;
    m_material = otherBox.m_material;
    
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    rglVertexAttribPointer(m_verts[0]);
}


template <class VertT>
void Box<VertT>::draw()
{
    glBindVertexArray(m_VAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
}





#endif /* Shapes_h */
