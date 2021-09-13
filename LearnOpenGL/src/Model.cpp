//
//  Model.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 9/12/21.
//

#include <stdio.h>
#include <iostream>

#include "Model.hpp"



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
            vertex.textureUV.x = mesh->mTextureCoords[0][ii].x;
            vertex.textureUV.y = mesh->mTextureCoords[0][ii].y;
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
        
        //load Diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                                Texture::diffuseName);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        //load Specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                                                Texture::specName);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    
    return Mesh(vertices, indices, textures);
    
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
            texture.id = loadTextureFromFile(textPath.C_Str());
            texture.typeName = type;
            texture.path = textPath.C_Str();
            m_texturesLoaded.push_back(texture);
        }
        
        textures.push_back(texture);
    }
    
    
    return textures;
}
