//
//  Model.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 9/12/21.
//

#include <stdio.h>
#include <iostream>

#include "Model.hpp"

std::vector<Texture> Model::m_texturesLoaded{};


void Model::Draw(Shader &shader, int instances)
{
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, m_transform.position);
    model = glm::rotate(model, glm::radians(m_transform.rotation.x), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(m_transform.rotation.y), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(m_transform.rotation.z), glm::vec3(0.f, 0.f, 1.f));
    model = glm::scale(model, m_transform.scale);
    shader.setUniformMatrix4f("model", model);

    if(instances <= 1)
    {
        for(int ii = 0; ii < m_Meshes.size(); ++ii)
        {
            m_Meshes[ii].Draw(shader);
        }
    }
    else
    {
        for(int ii = 0; ii < m_Meshes.size(); ++ii)
        {
            m_Meshes[ii].Draw(shader, instances);
        }
    }
    
}



void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene =  importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR WTIH LOADING SCENE USING ASSIMP: " << importer.GetErrorString() << std::endl;
        return;
    }
    
    m_directory = path.substr(0, path.find_last_of('/'));
    
    processNode(scene->mRootNode, scene);
}




void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(int ii = 0; ii < node->mNumMeshes; ++ii)
    {
        m_Meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[ii]], scene));
    }

    for(int ii = 0; ii < node->mNumChildren; ++ii)
    {
        processNode(node->mChildren[ii], scene);
    }
}





Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    bool hasSpecMap{false};
    
    
    //Load vertex data
    for(int ii = 0; ii < mesh->mNumVertices; ++ii)
    {
        Vertex vertex;
        
        vertex.position.x = mesh->mVertices[ii].x;
        vertex.position.y = mesh->mVertices[ii].y;
        vertex.position.z = mesh->mVertices[ii].z;
        
        vertex.normal.x = mesh->mNormals[ii].x;
        vertex.normal.y = mesh->mNormals[ii].y;
        vertex.normal.z = mesh->mNormals[ii].z;
        
        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][ii].x;
            vec.y = mesh->mTextureCoords[0][ii].y;
            vertex.textureUV = vec;
//            vertex.textureUV.x = mesh->mTextureCoords[0][ii].x;
//            vertex.textureUV.y = mesh->mTextureCoords[0][ii].y;
        }
        else
        {
            vertex.textureUV = glm::vec2(0.0f);
        }
        
        vertices.push_back(vertex);
    }
    
    
    
    // Load index data for faces
    for (int ii = 0; ii < mesh->mNumFaces; ++ii)
    {
        aiFace face = mesh->mFaces[ii];
        for(int jj = 0; jj < face.mNumIndices; ++jj)
        {
            indices.push_back(face.mIndices[jj]);
        }
    }
    
    
    
    // Load material data
    if(mesh->mMaterialIndex >=0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // Store that this model has a specular map
        if(material->GetTextureCount(aiTextureType_SPECULAR) > 0)
        {
            hasSpecMap = true;
        }
        
        //load Diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                                Texture::diffuseName);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        //load Specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                                                Texture::specName);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    
    return Mesh(vertices, indices, textures, hasSpecMap);
    
}





std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType textType,
                                                 std::string type)
{
    std::vector<Texture> textures;
    bool textureAlreadyLoaded;
    
    for(int ii = 0; ii < mat->GetTextureCount(textType); ++ii)
    {
        aiString textPath;
        Texture texture;
        mat->GetTexture(textType, ii, &textPath);
        textureAlreadyLoaded = false;
        
        for(int ii = 0; ii < m_texturesLoaded.size(); ++ii)
        {
            if(strcmp(m_texturesLoaded[ii].path.data(), textPath.C_Str()) == 0)
            {
                texture = m_texturesLoaded[ii];
                textureAlreadyLoaded = true;
                break;
            }
        }
        
        if(!textureAlreadyLoaded)
        {
            texture.id = loadTextureFromFile(textPath.C_Str(), m_directory, m_sRGB);
            texture.uniformName = type;
            texture.path = textPath.C_Str();
            texture.sRGB = m_sRGB;
            if(textType == aiTextureType_DIFFUSE)
                texture.type = TextureType::DIFFUSE;
            if(textType == aiTextureType_SPECULAR)
                texture.type = TextureType::SPECULAR;
            
            m_texturesLoaded.push_back(texture);
        }
        
        textures.push_back(texture);
    }
    
    
    return textures;
}



/*******************  setsRGB   ************************************
 * \brief Change the sRGB value.  This will reload the model so that the
 *      textures can be reloaded with the changed color space.
 *
 * \param sRGB - new value to set sRGB to
 **************************************************************///
void Model::setsRGB(bool sRGB)
{
    m_sRGB = sRGB;
    loadModel(m_path);
}



/*******************  toJson   ************************************
 * \brief Convert model object to json file.
 **************************************************************///
const json Model::toJson() const
{
    return json{
        {"type", "Model"},
        {"path", m_path},
        {"position", m_transform.position},
        {"rotation", m_transform.rotation},
        {"scale", m_transform.scale},
        {"sRGB", m_sRGB}
    };
}
