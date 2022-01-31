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
    
    
    // Vertex data to define the shape
    std::vector<Vert3x3x2f> m_verts;

    // Container for all the textures associated with the shape
    std::vector<Texture> m_textures;
    // Load texture in linear or sRGB space
    bool sRGBTexture{false};
    
     
    
    
    void setupMesh();
    
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) const
    = 0;

    // ///////////// loadTexture   ////////////////
    /**
     \brief Loads a texture from a file.  Creates texture buffer ID and adds texture struct to m_textures.
     
     \param filename - full path to the texture
    \param textureType - type of texture for the struct.  Diffuse, specular, normal, disp, ...
     
     */
    void loadTexture(std::string filename, std::string textureType);
    
    
    
    
    
public:
    // Material properties of object.  Things like diffuse/specular color and shininess
    Material m_material;
    // Position/rotation/scale of the object.  This is used to create model matrix for rendering
    Transform m_transform;
    // What kind of shape (PLANE,CUBE,...)  Used mainly for serialization at the moment.
    GameObject m_shapeType;
    // Should the object be outlined when rendering.
    bool m_outlined = false;
    
    
    //*********************************************
    //            Getters/Setters
    //*********************************************
    const std::vector<Texture> getTextures() const {return m_textures;};
    
    //*********************************************
    //            Load Texture Methods
    //*********************************************
    
    // ///////////// AttachTextures   //////////////////////////////////////
    /**
     * \brief These methods are used attach a texture to the shape.  The texture data is loaded and
     *      a TBO is created.  Finally all the texture data is stored in a Texture objects and added
     *      to the `m_textures` vector.
     *
     *      The main thing done is to fill the m_textures vector with the texture data.  This
     *      vector is then used during draw calls to bind the textures and set the uniforms.
     *      
     *
     * \param filename - full path of the texture file
     * \param type - type of texture being attached (diffuse, specular, ...)
     * \param sRGB - Should the texture be loaded as sRGB or linear
     *
     */
    // //////////////////////////////////////////////////////////////////////////
    void loadTextures(std::string filename, TextureType type, bool sRGB = false);
    void loadTextures(std::vector<std::string> filename, TextureType type, bool sRGB = Texture::sRGBDefault);
    void loadTextures(std::vector<std::string> filename, TextureType type, std::vector<bool> sRGB);
    
    
    virtual ~Shape();
    
    
    
    void virtual Draw(Shader shader, int instances = 0);
  
    
    

    
    

    
    
    
    
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
        
        // Check if normals are flipped
        std::vector<Vert3x3x2f> tempVerts = fillVerts();
        if(m_verts[0].r != tempVerts[0].r || m_verts[0].g != tempVerts[0].g
           || m_verts[0].b != tempVerts[0].b)
        {
            j["flipNormals"] = true;
        }

        return j;
    };
    
};




//*********************************************
//            Points Class
//*********************************************


class Points : public Shape
{
    std::vector<Vert3x3f> m_positions;
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) const override {return std::vector<Vert3x3x2f>();};
    
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
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) const override {return std::vector<Vert3x3x2f>();};
    
public:
    Line(std::vector<Vert3x3f> positions);
    
    void virtual Draw(Shader shader, int instances = 0) override;
};








class Plane : public Shape
{
    
    
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) const override;
    float m_UVCorner;

public:
    float m_width, m_height;
    
    
    
    Plane(float UVCorner = 1.f);
    Plane(const Material& material);
    Plane(const Plane& otherBox);
    
    
    

};




class Cube : public Shape
{
    
    
    
    
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) const override;

    
    
    
public:
    float m_width, m_length, m_height;
    
    
    
    Cube();
    Cube(const Material& material);
    Cube(const Cube& otherBox);
    
    
    
    
    
    
    
};
























#endif /* Shapes_h */
