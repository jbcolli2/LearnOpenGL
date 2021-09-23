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






class Shape
{
    
protected:
    unsigned int m_VAO, m_VBO;
    std::vector<Vert3x3x2f> m_verts;

    std::vector<Texture> m_textures;
    
    
    void setupMesh(const std::vector<std::string>& diffTexturePaths = std::vector<std::string>(),
    const std::vector<std::string>& specTexturePaths = std::vector<std::string>(),
                   const Material& material = Material());
    
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) = 0;

    
public:
    Material m_material;
    Transform m_transform;
    bool m_outlined = false;
    
    
    
    
    virtual ~Shape() {};
    
    
    
    void Draw(Shader shader);
  
    
    

    
    
    
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
    
    
};








//class Triangle : public Shape
//{
//
//    std::vector<Vert3x3x2f> verts;
//public:
//    Triangle(std::vector<Vert3x3x2f> vert);
//
//    void virtual draw() override;
//};




class Plane : public Shape
{
    
    
    std::vector<Vert3x3x2f> virtual fillVerts(std::vector<Vert3x3x2f> verts = std::vector<Vert3x3x2f>()) override;

public:
    float m_width, m_height;
    
    
    
    Plane(const std::vector<std::string>& diffTexturePaths = std::vector<std::string>(),
         const std::vector<std::string>& specTexturePaths = std::vector<std::string>(),
         const Material& material = Material());
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
         const Material& material = Material());
    Cube(const Material& material);
    Cube(const Cube& otherBox);
    
    
    
    
    
    
    
};







































//template <class VertT>
//Triangle<VertT>::Triangle(std::vector<VertT> vert)
//{
//    verts = vert;
//    //******* VBO/VAO   ***************
//    glGenVertexArrays(1, &m_VAO);
//    glBindVertexArray(m_VAO);
//
//    loadVBOData(verts);
//
//    rglVertexAttribPointer(vert[0]);
//
//
//    glEnableVertexAttribArray(0);
//    //********* m_VAO **************
//}
//
//
//
//
//
//template <class VertT>
//void Triangle<VertT>::draw()
//{
//    glBindVertexArray(m_VAO);
//
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//}






//template <class VertT>
//Square<VertT>::Square(std::vector<VertT> vert, bool clockwise)
//{
//    this->verts = vert;
//
//
//    //******* VBO/VAO   ***************
//    glGenVertexArrays(1, &m_VAO);
//    glBindVertexArray(m_VAO);
//
//    loadVBOData(verts);
//
//    loadEBOData(indices);
//
//    rglVertexAttribPointer(vert[0]);
//
//
//
//    //********* m_VAO **************
//
//}
//
//
//
//
//
//template <class VertT>
//void Square<VertT>::draw()
//{
//    glBindVertexArray(m_VAO);
//
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//}

















#endif /* Shapes_h */
