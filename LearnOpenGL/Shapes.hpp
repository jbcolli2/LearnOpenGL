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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OpenGLUtil.hpp"








class Shape
{
    
protected:
    unsigned int VAO, texture;

    
public:
    Shape() {};
    void virtual draw() = 0;
  
    
    
protected:
    void loadTexture(const char* filename)
    {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

        if(data)
        {
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failure to load texture " << filename << std::endl;
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
    Square(std::vector<VertT> vert, const char* textureFile, bool clockwise = true);
    
    void virtual draw() override;
};






















template <class VertT>
Triangle<VertT>::Triangle(std::vector<VertT> vert)
{
    verts = vert;
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    loadVBOData(verts);

    rglVertexAttribPointer(vert[0]);
    
    
    glEnableVertexAttribArray(0);
    //********* VAO **************
}





template <class VertT>
void Triangle<VertT>::draw()
{
    glBindVertexArray(VAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}






template <class VertT>
Square<VertT>::Square(std::vector<VertT> vert, bool clockwise)
{
    this->verts = vert;
    
    
    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    loadVBOData(verts);
    
    loadEBOData(indices);

    rglVertexAttribPointer(vert[0]);
    
    
    
    //********* VAO **************
    
}


template <class VertT>
Square<VertT>::Square(std::vector<VertT> vert, const char* textureFile, bool clockwise) : Square(vert, clockwise)
{
    //********* VAO **************
    loadTexture(textureFile);
}



template <class VertT>
void Square<VertT>::draw()
{
    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}




#endif /* Shapes_h */
