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

#include "OpenGLUtil.hpp"








class Shape
{
    
protected:
    unsigned int m_VAO;
    std::vector<unsigned int> m_textures;

    
public:
    Shape() {};
    virtual ~Shape() {};
    void virtual draw() = 0;
  
    
    
    void loadTexture(std::string filename, unsigned int rgbFlag = GL_RGB)
    {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

        if(data)
        {
            m_textures.push_back(0);

            glActiveTexture(GL_TEXTURE0 + m_textures.size() - 1);
            glGenTextures(1, &m_textures.back());
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
    
    void loadTextureAlpha(std::string filename)
    {
        loadTexture(filename, GL_RGBA);
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
    Box(std::vector<VertT> verts);
    
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
    for(int ii = 0; ii < m_textures.size(); ++ii)
    {
        glActiveTexture(GL_TEXTURE0 + ii);
        glBindTexture(GL_TEXTURE_2D, m_textures[ii]);
    }
    glBindVertexArray(m_VAO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}






//-----------  Box  ---------------------//
template <class VertT>
Box<VertT>::Box(std::vector<VertT> verts) : m_verts(verts)
{
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    loadVBOData(m_verts);
    

    rglVertexAttribPointer(m_verts[0]);
    
    
    
    //********* VAO **************
    
    
}


template <class VertT>
void Box<VertT>::draw()
{
    for(int ii = 0; ii < m_textures.size(); ++ii)
    {
        glActiveTexture(GL_TEXTURE0 + ii);
        glBindTexture(GL_TEXTURE_2D, m_textures[ii]);
    }
    glBindVertexArray(m_VAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
}





#endif /* Shapes_h */
