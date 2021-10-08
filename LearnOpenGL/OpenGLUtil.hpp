//
//  OpenGLUtil.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/30/21.
//

#ifndef OpenGLUtil_h
#define OpenGLUtil_h


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include "VertexData.hpp"




inline void rglVertexAttribPointer(Vert3f v)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
};


inline void rglVertexAttribPointer(Vert3x3f v)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
};


inline void rglVertexAttribPointer(Vert3x3x2f v)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
 
};



inline void rglVertexAttribPointer(Vert3x_x_f v)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

};



inline void rglVertexAttribPointer(Vert2x2f v)
{
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(offsetof(Vert2x2f, u)));
    glEnableVertexAttribArray(1);
};






template <class T>
unsigned int loadVBOData(std::vector<T> vec)
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vec.size()*sizeof(T), &vec[0], GL_STATIC_DRAW);
    
    rglVertexAttribPointer(vec[0]);
    
    return VBO;
}


template <class T>
unsigned int loadEBOData(std::vector<T> vec)
{
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vec.size() * sizeof(T), &vec.front(), GL_STATIC_DRAW);
    
    return EBO;
}






inline unsigned int loadTextureFromFile(const char* path)
{
    unsigned int texID = 0;
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
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
        std::cout << "Number of channels in image is not 3 or 4, it is " << nrChannels << "\n";
        return 0;
    }
    
    if(data)
    {
        glGenTextures(1, &texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, rgbFlag, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failure to load texture " << path << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return texID;
}





inline unsigned int loadTextureFromFile(const char* path, std::string directory)
{
    const char* fullpath = (directory+'/').append(path).c_str();
    return loadTextureFromFile(fullpath);
}

#endif /* OpenGLUtil_h */
