//
//  Model.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 9/11/21.
//

#ifndef Model_h
#define Model_h

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "GameObjects/Mesh.hpp"
#include "Utilities/OpenGLUtil.hpp"


class Model
{
public:
    Transform m_transform;
    
    Model() = default;
    Model(const char* path, bool sRGB = Texture::sRGBDefault) : m_path(path), m_sRGB(sRGB)
    {
        loadModel(path);
    }
    
    void Draw(Shader& shader, int instances = 1);
    
    void BindVertexArray()
    {
        for(auto& mesh : m_Meshes)
        {
            mesh.BindVertexArray();
        }
    };
    
    
    
    /*******************  toJson   ************************************
     * \brief Convert model object to json file.
     **************************************************************///
    const json toJson() const;
    
    
    /*******************  setsRGB   ************************************
     * \brief Change the sRGB value.  This will invoke the loadModel method
     *
     * \param sRGB - new value to set sRGB to
     **************************************************************///
    void setsRGB(bool sRGB);
    
private:
    // Path to the model, including filename of model obj file
    std::string m_path{""};
    // Is model loaded in linear or sRGB space
    bool m_sRGB{Texture::sRGBDefault};
    std::vector<Mesh> m_Meshes;
    std::string m_directory;
    static std::vector<Texture> m_texturesLoaded;

    
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType textType, std::string type);
};


#endif /* Model_h */
