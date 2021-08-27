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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include "OpenGLUtil.hpp"



struct Material
{
    glm::vec3 ambient = glm::vec3(1.f);
    glm::vec3 diffuse = glm::vec3(1.f);
    glm::vec3 specular = glm::vec3(1.f, 0.f, 0.f);
    float shininess = 1;
};


struct TextureMaterial
{
    unsigned int ambdiffID = 0;
    unsigned int specID = 1;
    float shininess = 1;
};




class Shape
{
    
protected:
    unsigned int m_VAO, m_VBO;
    std::vector<unsigned int> m_textures;
    
    Material m_material;
    TextureMaterial m_texMaterial;

    
public:
    Shape() {};
    virtual ~Shape() {};
    
    Material getMaterial() {return m_material;};
    TextureMaterial getTexMaterial() {return m_texMaterial;};
    void virtual draw() = 0;
  
    
    
    void loadTexture(std::string filename, unsigned int texUnit, unsigned int rgbFlag = GL_RGB)
    {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

        if(data)
        {
            m_textures.push_back(0);

            
            glGenTextures(1, &m_textures.back());
            glActiveTexture(GL_TEXTURE0 + texUnit);
            glBindTexture(GL_TEXTURE_2D, m_textures.back());
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rgbFlag, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failure to load texture " << filename << std::endl;
        }
    }
    
    void loadTextureAlpha(std::string filename, unsigned int texUnit)
    {
        loadTexture(filename, texUnit, GL_RGBA);
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







template <class VertT>
class Triangle : public Shape
{

    std::vector<VertT> verts;
public:
    Triangle(std::vector<VertT> vert);
    
    void virtual draw() override;
};



template <class VertT>
class Square : public Shape
{
    std::vector<VertT> verts;
    std::vector<unsigned int> indices = {0,1,2, 0, 2,3};

    
public:
    Square(std::vector<VertT> vert, bool clockwise = true);
    
    
    void virtual draw() override;
};



template <class VertT>
class Box : public Shape
{
    float m_width, m_length, m_height;
    std::vector<VertT> m_verts;
    
    
    
public:
    Box();
//    Box(std::vector<VertT> verts);
    Box(std::vector<VertT> verts, Material material = Material(), TextureMaterial texMaterial = TextureMaterial());
    Box(const Box& otherBox);
    
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
Box<VertT>::Box(std::vector<VertT> verts, Material material, TextureMaterial texMaterial) : m_verts(verts)
{
    m_material = material;
    m_texMaterial = texMaterial;
    
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
