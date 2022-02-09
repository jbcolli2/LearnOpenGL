//
//  OpenGLUtil.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 1/31/22.
//
#include "OpenGLUtil.hpp"


#include <stdio.h>



//*********************************************
//            Define all static variables for Texture struct
//*********************************************
const std::string Texture::materialName = "material";
const std::string Texture::diffuseName = Texture::materialName + ".diffuse";
const std::string Texture::specName = Texture::materialName + ".specular";
const std::string Texture::normalName = Texture::materialName + ".normal";
const std::string Texture::bumpName = Texture::materialName + ".bump";

const bool Texture::sRGBDefault = false;






//***************************************************************
//            Serialization
//***************************************************************

//*********************************************
//            Convert glm <--> json
//*********************************************
namespace glm {
    // ******** vec3  ********** //
    void to_json(json &j, const glm::vec3 &vec) {
        j = json::array({vec.x, vec.y, vec.z});
    }

    void from_json(const json &j, glm::vec3 &vec) {
        vec.x = j.at(0).get<float>();
        vec.y = j.at(1).get<float>();
        vec.z = j.at(2).get<float>();
    }


    // ******** vec2  ********** //
   void to_json(json &j, const glm::vec2 &vec) {
        j = json::array({vec.x, vec.y,});
    }

    void from_json(const json &j, glm::vec2 &vec) {
        vec.x = j.at(0).get<float>();
        vec.y = j.at(1).get<float>();
    }
}


/*******************  addToJsonArray   ************************************
 * \brief Add a vector of objects to a json array at a particular key.  If the key doesn't already
 *      exist, this method will create the key.
 *
 * \param j - The json array to modify by adding an array
 * \param key - The name of the key for the array
 * \param values - a std::vector of values to be transfered into a json array
 **************************************************************/
template <typename T>
void addToJsonArray(json& j, const std::string& key, const std::vector<T>& values)
{
    for(T val : values)
    {
        if(j.find(key) == j.end())
            j[key] = json::array();
        
        j[key].push_back(val);
    }
}





//*********************************************
//      Convert std::vector<Textures> --> json
//*********************************************
/*
 * Take the vector of Textures from a shape and turn it into a "texture" block in json.
 * The block will look like
 * "texture:
 *      "diffuse": [path1, path2, ...]
 *      "specular": [path1, path2, ...]
 *      ...
 *
 * This conversion will not work from json to std::vector<Textures>.  We will take the json
 *  and feeding the texture portion directly into a Shape, instead of turning it into std::vector<Texture>
 *  and using that to create the Shape.
 * This conversion is only used to create a Shape (for right now at least), not to create any other objects.
 */

void to_json(json& j, const std::vector<Texture> textures)
{
    std::vector<std::string> diffTexPaths, specTexPaths, normTexPaths, bumpTexPaths;
    std::vector<bool> diffsRGB, specsRGB, normsRGB, bumpsRGB;
    
    for(int ii = 0; ii < textures.size(); ++ii)
    {
        if(textures[ii].type == TextureType::DIFFUSE)
        {
            diffTexPaths.push_back(textures[ii].path);
            diffsRGB.push_back(textures[ii].sRGB);
        }
        else if(textures[ii].type == TextureType::SPECULAR)
        {
            specTexPaths.push_back(textures[ii].path);
            specsRGB.push_back(textures[ii].sRGB);
        }
        else if(textures[ii].type == TextureType::NORMAL)
        {
            normTexPaths.push_back(textures[ii].path);
            normsRGB.push_back(textures[ii].sRGB);
        }
        else if(textures[ii].type == TextureType::BUMP)
        {
            bumpTexPaths.push_back(textures[ii].path);
            bumpsRGB.push_back(textures[ii].sRGB);
        }
    }
    
    j = {{"textures", json()}};
    
    
    addToJsonArray<std::string>(j["textures"], "diffuse", diffTexPaths);
    addToJsonArray(j["textures"], "sRGBDiff", diffsRGB);
    
    addToJsonArray<std::string>(j["textures"], "specular", specTexPaths);
    addToJsonArray(j["textures"], "sRGBSpec", specsRGB);
    
    addToJsonArray<std::string>(j["textures"], "normal", normTexPaths);
    addToJsonArray(j["textures"], "sRGBNorm", normsRGB);
    
    addToJsonArray<std::string>(j["textures"], "bump", bumpTexPaths);
    addToJsonArray(j["textures"], "sRGBBump", bumpsRGB);
    
}









//*********************************************
//            Set Vertex attributes
//*********************************************
void rglVertexAttribPointer(Vert3f v, int layoutOffset)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
}

void rglVertexAttribPointer(Inst3f v, int layoutOffset)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    glVertexAttribDivisor(0 + layoutOffset, 1);
};

void rglVertexAttribPointer(Inst4f v, int layoutOffset)
{
    glVertexAttribPointer(0 + layoutOffset, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    glVertexAttribDivisor(0 + layoutOffset, 1);
};

void rglVertexAttribPointer(Vert3x3f v, int layoutOffset)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1 + layoutOffset);
};

void rglVertexAttribPointer(Vert3x3x2f v, int layoutOffset)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    
    glVertexAttribPointer(1 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1 + layoutOffset);
    
    glVertexAttribPointer(2 + layoutOffset, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2 + layoutOffset);
 
};

void rglVertexAttribPointer(Vert3x3x2x3x3f v, int layoutOffset)
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

void rglVertexAttribPointer(Vert3x_x_f v, int layoutOffset)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);

};

void rglVertexAttribPointer(Vert2x2f v, int layoutOffset)
{
    glVertexAttribPointer(0 + layoutOffset, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    
    glVertexAttribPointer(1 + layoutOffset, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(offsetof(Vert2x2f, u)));
    glEnableVertexAttribArray(1 + layoutOffset);
};

void rglVertexAttribPointer(Vert3x2f v, int layoutOffset)
{
    glVertexAttribPointer(0 + layoutOffset, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0 + layoutOffset);
    
    glVertexAttribPointer(1 + layoutOffset, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(offsetof(Vert3x2f, u)));
    glEnableVertexAttribArray(1 + layoutOffset);
};









//*********************************************
//            Graphics data loading functions
//*********************************************
unsigned int loadTextureFromFile(const char* path, bool sRGB)
{
    unsigned int texID = 0;
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    unsigned int textureFormat, internalFormat;
    if(nrChannels == 3)
    {
        textureFormat = GL_RGB;
        internalFormat = sRGB ? GL_SRGB : GL_RGB;
    }
    else if(nrChannels == 4)
    {
        textureFormat = GL_RGBA;
        internalFormat = sRGB ? GL_SRGB_ALPHA : GL_RGBA;
    }
    else if(nrChannels == 1)
    {
        textureFormat = GL_RED;
        internalFormat = GL_RED;
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
        
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, data);
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

unsigned int loadTextureFromFile(const char* path, std::string directory, bool sRGB)
{
    const char* fullpath = (directory+'/').append(path).c_str();
    return loadTextureFromFile(fullpath, sRGB);
}
