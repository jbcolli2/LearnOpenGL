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

#include "Mesh.hpp"
#include "OpenGLUtil.hpp"


class Model
{
public:
    Model() = default;
    Model(const char* path)
    {
        loadModel(path);
    }
    
    void Draw(Shader& shader);
    
    
private:
    std::vector<Mesh> m_Meshes;
    std::string m_directory;
    std::vector<Texture> m_texturesLoaded;
    
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType textType, std::string type);
};


#endif /* Model_h */
