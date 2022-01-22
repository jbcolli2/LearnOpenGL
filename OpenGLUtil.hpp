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
#include "nlohmann/json.hpp"

using json = nlohmann::json;



//*********************************************
//            Information for any GameObject
//*********************************************
struct Transform
{
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 rotation = glm::vec3(0.f);
    glm::vec3 scale = glm::vec3(1.f);
};


struct Material
{
    glm::vec3 ambient = glm::vec3(1.f);
    glm::vec3 diffuse = glm::vec3(1.f);
    glm::vec3 specular = glm::vec3(1.f);
    
    
    float shininess = 1;
};


struct Texture
{
    const static std::string materialName;
    const static std::string diffuseName;
    const static std::string specName;
    const static std::string normalName;
    const static std::string dispName;
    unsigned int id;
    char unitID;
    std::string typeName;
    std::string path;
};






//*********************************************
//            Serialization
//*********************************************
enum GameObject
{
    CUBE,
    PLANE,
    PTLIGHT,
    DIRLIGHT,
    SPOTLIGHT,
    CAMERA,
    INVALID,
};

NLOHMANN_JSON_SERIALIZE_ENUM(GameObject,
{
{INVALID, nullptr},
{CUBE, "Cube"},
{PLANE, "Plane"},
{PTLIGHT, "PointLight"},
{DIRLIGHT, "DirLight"},
{CAMERA, "Camera"},
})


//*********************************************
//            Convert glm <--> json
//*********************************************
namespace glm {
    // ******** vec3  ********** //
    inline void to_json(json &j, const glm::vec3 &vec) {
        j = json::array({vec.x, vec.y, vec.z});
    }

    inline void from_json(const json &j, glm::vec3 &vec) {
        vec.x = j.at(0).get<float>();
        vec.y = j.at(1).get<float>();
        vec.z = j.at(2).get<float>();
    }


    // ******** vec2  ********** //
   inline void to_json(json &j, const glm::vec2 &vec) {
        j = json::array({vec.x, vec.y,});
    }

    inline void from_json(const json &j, glm::vec2 &vec) {
        vec.x = j.at(0).get<float>();
        vec.y = j.at(1).get<float>();
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

inline void to_json(json& j, const std::vector<Texture> textures)
{
    std::vector<std::string> diffTexPaths, specTexPaths, normTexPaths, bumpTexPaths;
    
    for(int ii = 0; ii < textures.size(); ++ii)
    {
        if(textures[ii].typeName == Texture::diffuseName)
        {
            diffTexPaths.push_back(textures[ii].path);
        }
        else if(textures[ii].typeName == Texture::specName)
        {
            specTexPaths.push_back(textures[ii].path);
        }
        else if(textures[ii].typeName == Texture::normalName)
        {
            normTexPaths.push_back(textures[ii].path);
        }
        else if(textures[ii].typeName == Texture::dispName)
        {
            bumpTexPaths.push_back(textures[ii].path);
        }
    }
    
    json jDiffuse, jSpec, jNormal, jBump;
    j = {{"textures",
        {
            {"diffuse", json::array()},
            {"specular", json::array()},
            {"normal", json::array()},
            {"bump", json::array()}
        }
        }};
    for(const auto& paths : diffTexPaths)
    {
        j["textures"]["diffuse"].push_back(paths);
    }
    for(const auto& paths : specTexPaths)
    {
        j["textures"]["specular"].push_back(paths);
    }
    for(const auto& paths : normTexPaths)
    {
        j["textures"]["normal"].push_back(paths);
    }
    for(const auto& paths : bumpTexPaths)
    {
        j["textures"]["bump"].push_back(paths);
    }
}










//*********************************************
//            Set Vertex attributes
//*********************************************
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










//*********************************************
//            Graphics data loading functions
//*********************************************
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
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, rgbFlag, GL_UNSIGNED_BYTE, data);
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
                    
