//
//  OpenGLUtil.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/30/21.
//

#ifndef OpenGLUtil_h
#define OpenGLUtil_h

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include <glm/glm.hpp>

#include "Utilities/VertexData.hpp"
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



enum class TextureType
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    BUMP
};

struct Texture
{
    const static std::string materialName;
    const static std::string diffuseName;
    const static std::string specName;
    const static std::string normalName;
    const static std::string bumpName;
    
    // The default sRGB choice for loading texture.  True means textures loaded in sRGB space
    // False means textures loaded in linear space.
    const static bool sRGBDefault;
    
    // the TBO of the texture
    unsigned int id;
    // The unit id used in the shader to reference the texture
    char unitID;
    // The string of the sampler2D uniform for this texture
    std::string uniformName;
    // Full path of the texture file
    std::string path;
    // Type of texture (diffuse, specular, ...)
    TextureType type;
    // Should texture be loaded as sRGB or linear.  This info is mostly
    //      kept for serialization purposes
    bool sRGB;
};






//*********************************************
//            Serialization
//*********************************************
enum GameObject
{
    CUBE,
    PLANE,
    MODEL,
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
{MODEL, "Model"},
{PTLIGHT, "PointLight"},
{DIRLIGHT, "DirLight"},
{CAMERA, "Camera"},
})


//*********************************************
//            Convert glm <--> json
//*********************************************
namespace glm
{
    // ******** vec3  ********** //
void to_json(json &j, const glm::vec3 &vec);

void from_json(const json &j, glm::vec3 &vec);


    // ******** vec2  ********** //
void to_json(json &j, const glm::vec2 &vec);

void from_json(const json &j, glm::vec2 &vec);
}



//*********************************************
//      Convert std::vector<Textures> --> json
//*********************************************
/*
 * Take the vector of Textures from a shape and turn it into a "texture" block in json.
 */

void to_json(json& j, const std::vector<Texture> textures);










//*********************************************
//            Set Vertex attributes
//*********************************************
void rglVertexAttribPointer(Vert3f v, int layoutOffset = 0);
void rglVertexAttribPointer(Inst3f v, int layoutOffset = 0);
void rglVertexAttribPointer(Inst4f v, int layoutOffset = 0);
void rglVertexAttribPointer(Vert3x3f v, int layoutOffset = 0);
void rglVertexAttribPointer(Vert3x3x2f v, int layoutOffset = 0);
void rglVertexAttribPointer(Vert3x3x2x3x3f v, int layoutOffset = 0);
void rglVertexAttribPointer(Vert3x_x_f v, int layoutOffset = 0);
void rglVertexAttribPointer(Vert2x2f v, int layoutOffset = 0);
void rglVertexAttribPointer(Vert3x2f v, int layoutOffset = 0);










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

unsigned int loadTextureFromFile(const char* path, bool sRGB);
unsigned int loadTextureFromFile(const char* path, std::string directory, bool sRGB);

#endif /* OpenGLUtil_h */
                    
