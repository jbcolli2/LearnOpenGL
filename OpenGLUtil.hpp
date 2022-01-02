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
#include <glm/glm.hpp>

#include "VertexData.hpp"

struct Transform
{
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 rotation = glm::vec3(0.f);
    glm::vec3 scale = glm::vec3(1.f);
};



inline void rglVertexAttribPointer(Vert3f v, int layoutOffset = 0)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
};

inline void rglVertexAttribPointer(Inst3f v, int layoutOffset = 0)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    glVertexAttribDivisor(0 + layoutOffset, 1);
};

inline void rglVertexAttribPointer(Inst4f v, int layoutOffset = 0)
{
    glVertexAttribPointer(0 + layoutOffset, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    glVertexAttribDivisor(0 + layoutOffset, 1);
};




inline void rglVertexAttribPointer(Vert3x3f v, int layoutOffset = 0)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1 + layoutOffset);
};


inline void rglVertexAttribPointer(Vert3x3x2f v, int layoutOffset = 0)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    
    glVertexAttribPointer(1 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1 + layoutOffset);
    
    glVertexAttribPointer(2 + layoutOffset, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2 + layoutOffset);
 
};


inline void rglVertexAttribPointer(Vert3x3x2x3x3f v, int layoutOffset = 0)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 14*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    
    glVertexAttribPointer(1 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 14*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1 + layoutOffset);
    
    glVertexAttribPointer(2 + layoutOffset, 2, GL_FLOAT, GL_FALSE, 14*sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2 + layoutOffset);
    
    glVertexAttribPointer(3 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 14*sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3 + layoutOffset);
    
    glVertexAttribPointer(4 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 14*sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(4 + layoutOffset);
 
};



inline void rglVertexAttribPointer(Vert3x_x_f v, int layoutOffset = 0)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);

};



inline void rglVertexAttribPointer(Vert2x2f v, int layoutOffset = 0)
{
    glVertexAttribPointer(0 + layoutOffset, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    
    glVertexAttribPointer(1 + layoutOffset, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(offsetof(Vert2x2f, u)));
    glEnableVertexAttribArray(1 + layoutOffset);
};



inline void rglVertexAttribPointer(Vert3x2f v, int layoutOffset = 0)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    
    glVertexAttribPointer(1 + layoutOffset, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(offsetof(Vert3x2f, u)));
    glEnableVertexAttribArray(1 + layoutOffset);
};










template <class T>
unsigned int loadVBOData(std::vector<T> vec, int layoutOffset = 0)
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vec.size()*sizeof(T), &vec[0], GL_STATIC_DRAW);
    
    rglVertexAttribPointer(vec[0], layoutOffset);
    
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
                    
