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
        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
        
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







template <class T>
class Triangle : public Shape
{

    std::vector<T> verts;
public:
    Triangle(std::vector<T> vert);
    
    void virtual draw() override;
};



template <class T>
class Square : public Shape
{
    unsigned int VAO, texture;
    std::vector<T> verts;
    std::vector<unsigned int> indices = {0,1,2, 0, 2,3};

    
public:
    Square(std::vector<T> verts, bool clockwise = true);
    
    void virtual draw() override;
};






















template <class T>
Triangle<T>::Triangle(std::vector<T> vert)
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





template <class T>
void Triangle<T>::draw()
{
    glBindVertexArray(VAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}






template <class T>
Square<T>::Square(std::vector<T> vert, bool clockwise)
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



template <class T>
void Square<T>::draw()
{
    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}




#endif /* Shapes_h */
