//
//  Skybox.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 10/20/21.
//

#include <stdio.h>

#include "Skybox.hpp"




Skybox::Skybox(std::vector<std::string> texFilenames)
{
    
    // Setup VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    m_vbo = loadVBOData(m_box);
    
    glBindVertexArray(0);
    
    
    
    // Setup cubedata textures
    glGenTextures(1, &m_tbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tbo);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    int width, height, nrChannels;
    unsigned char* data;
    for(int ii = 0; ii < 6; ++ii)
    {
        data = 0;
        data = stbi_load(texFilenames[ii].c_str(), &width, &height, &nrChannels, 0);
        unsigned int rgbFlag = GL_RGB;
        if(nrChannels == 4)
        {
            rgbFlag = GL_RGBA;
        }
        
        
        
        if(data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + ii, 0, GL_RGB, width, height, 0, rgbFlag, GL_UNSIGNED_BYTE, data);
        }
    }
}




void Skybox::Draw()
{
    glDepthMask(GL_FALSE);
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tbo);
    glDrawArrays(GL_TRIANGLES, 0, m_box.size());
    glDepthMask(GL_TRUE);
}
