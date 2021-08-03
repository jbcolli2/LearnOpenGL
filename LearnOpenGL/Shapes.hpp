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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OpenGLUtil.hpp"








class Shape
{
    
protected:
    unsigned int VAO;
    std::vector<unsigned int> textures;

    
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
            textures.push_back(0);

            glActiveTexture(GL_TEXTURE0 + textures.size() - 1);
            glGenTextures(1, &textures.back());
            glBindTexture(GL_TEXTURE_2D, textures.back());
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
void Square<VertT>::draw()
{
    for(int ii = 0; ii < textures.size(); ++ii)
    {
        glActiveTexture(GL_TEXTURE0 + ii);
        glBindTexture(GL_TEXTURE_2D, textures[ii]);
    }
    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}




#endif /* Shapes_h */
