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
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include "OpenGLUtil.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"


using json = nlohmann::json;


std::vector<Vert3x3x2x3x3f> addTBtoVerts(std::vector<Vert3x3x2f>);



class Shape
{
    
protected:
    unsigned int m_VAO, m_VBO;
    unsigned int m_numVerts;
    std::vector<Vert3x3x2f> m_verts;

    std::vector<Texture> m_textures;
    
    
    void setupMesh(const std::vector<std::string>& diffTexturePaths = std::vector<std::string>(),
                   const std::vector<std::string>& specTexturePaths = std::vector<std::string>(),
                   const std::vector<std::string>& normalTexturePaths = std::vector<std::string>(),
                   const std::vector<std::string>& dispTexturePaths = std::vector<std::string>(),
                   const Material& material = Material());
    
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) = 0;

    
public:
    Material m_material;
    Transform m_transform;
    GameObject m_shapeType;
    bool m_outlined = false;
    
    const std::vector<Texture> getTextures() const {return m_textures;};
    
    
    
    
    virtual ~Shape() {};
    
    
    
    void virtual Draw(Shader shader, int instances = 0);
  
    
    

    
    
    // ///////////// loadTexture   ////////////////
    /**
     \brief Loads a texture from a file.  Creates texture buffer ID and adds texture struct to m_textures.
     
     \param filename - full path to the texture
    \param textureType - type of texture for the struct.  Diffuse, specular, normal, disp, ...
     
     */
    void loadTexture(std::string filename, std::string textureType)
    {
        unsigned int texID = loadTextureFromFile(filename.c_str());
        if(texID == 0)
        {
            std::cout << "SHAPE::Failed to load texture " << filename << "\n";
            return;
        }
        
        Texture texture;
        texture.id = texID;
        texture.path = filename;
        texture.typeName = textureType;
        
        m_textures.push_back(texture);
    }
    
    
    
    
    // ///////////// FlipNormals   ////////////////
    /**
     \brief Flips the normals in the vertex data.  Useful for using a cube to make a room, you want the inside to be shown
     */
    void FlipNormals()
    {
        if(m_verts.size() == 0)
        {
            std::cout << "SHAPE::No verts defined in FlipNormals()\n";
            return;
        }
        
        for (auto& vert : m_verts)
        {
            vert.r = -vert.r;
            vert.g = -vert.g;
            vert.b = -vert.b;
        }
        
        glBindVertexArray(m_VAO);
        glDeleteBuffers(1, &m_VBO);
        m_VBO = loadVBOData(m_verts);
        glBindVertexArray(0);
        
        return;
    }
    
    
    
    // ///////////// toJson   ////////////////
    /**
     \brief Convert the shape into a json object.  This should not depend on the type of shape.
     
     \returns The json object created to represent the shape.
     */
    
    const json toJson() const
    {
        json jTextures = m_textures;
        
        json j = {
            {"type", m_shapeType},
            {"position", m_transform.position},
            {"rotation", m_transform.rotation},
            {"scale", m_transform.scale},
            {"material",
                {
                {"diffuse", m_material.diffuse},
                {"specular", m_material.specular},
                {"ambient", m_material.ambient},
                {"shininess", m_material.shininess}
                }
            }
        };
        j.insert(jTextures.begin(), jTextures.end());

        return j;
    };
    
};




//*********************************************
//            Points Class
//*********************************************


class Points : public Shape
{
    std::vector<Vert3x3f> m_positions;
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) override {return std::vector<Vert3x3x2f>();};
    
public:
    Points(std::vector<Vert3x3f> positions);
    
    void virtual Draw(Shader shader, int instances = 0) override;
};





//*********************************************
//            Line Class
//*********************************************

class Line : public Shape
{
    std::vector<Vert3x3f> m_positions;
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) override {return std::vector<Vert3x3x2f>();};
    
public:
    Line(std::vector<Vert3x3f> positions);
    
    void virtual Draw(Shader shader, int instances = 0) override;
};








class Plane : public Shape
{
    
    
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) override;
    float m_UVCorner;

public:
    float m_width, m_height;
    
    
    
    Plane(const std::vector<std::string>& diffTexturePaths = std::vector<std::string>(),
         const std::vector<std::string>& specTexturePaths = std::vector<std::string>(),
          const std::vector<std::string>& normalTexturePaths = std::vector<std::string>(),
          const std::vector<std::string>& dispTexturePaths = std::vector<std::string>(),
         float UVCorner = 1.f, const Material& material = Material());
    Plane(const Material& material);
    Plane(const Plane& otherBox);
    
    
    

};




class Cube : public Shape
{
    
    
    
    
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) override;

    
    
    
public:
    float m_width, m_length, m_height;
    
    
    
    Cube(const std::vector<std::string>& diffTexturePaths = std::vector<std::string>(),
         const std::vector<std::string>& specTexturePaths = std::vector<std::string>(),
         const std::vector<std::string>& normalTexturePaths = std::vector<std::string>(),
         const std::vector<std::string>& dispTexturePaths = std::vector<std::string>(),
         const Material& material = Material());
    Cube(const Material& material);
    Cube(const Cube& otherBox);
    
    
    
    
    
    
    
};
























#endif /* Shapes_h */
