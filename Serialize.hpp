//
//  Serialize.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 1/20/22.
//

#ifndef Serialize_h
#define Serialize_h

#include <fstream>

#include "glm/glm.hpp"
#include "nlohmann/json.hpp"
#include "Mesh.hpp"

using json = nlohmann::json;


// ///////////// JsonToFile   ////////////////
/**
 \brief Creates a json file from a json object.
 
 \param j - The json object containing all the data
 \param path - name of the file
 */
inline void JsonToFile(const json& j, const std::string& path)
{
    std::fstream file;
    file.open(path, std::ifstream::out);
    file << j.dump(4);
    file.close();
}




inline void from_json(const json& j, std::vector<Texture> textures)
{
    std::cout << "Trying to convert json to std::vector<Texture>.  YOU SHOULDN'T BE DOING THIS!!!  STOP!!\n";
}







//*********************************************
//            Convert Shapes
//*********************************************
template <typename T>
inline void JsonToShape(const json& j, T& shape)
{
    // Get all the texture paths ...
    json jTex;
    try
    {
        jTex = j.at("textures");
    }
    catch(json::out_of_range& e)
    {
        jTex = json{{"trash", 8}};
    }
    
    
    std::vector<std::string> diffTexPaths = jTex.value("diffuse", std::vector<std::string>());
    std::vector<std::string> specTexPaths = jTex.value("specular", std::vector<std::string>());
    std::vector<std::string> normTexPaths = jTex.value("normal", std::vector<std::string>());
    std::vector<std::string> bumpTexPaths = jTex.value("bump", std::vector<std::string>());
    
    std::vector<bool> diffTexsRGB = jTex.value("sRGBDiff", std::vector<bool>());
    std::vector<bool> specTexsRGB = jTex.value("sRGBSpec", std::vector<bool>());
    std::vector<bool> normTexsRGB = jTex.value("sRGBNorm", std::vector<bool>());
    std::vector<bool> bumpTexsRGB = jTex.value("sRGBBump", std::vector<bool>());
    
    // ... then create a new shape with these texture paths ...
    //TODO: Include the UVCorner parameter into the entire serialization pipeline
    shape = T();
    
    //... add all textures to the shape ...
    if(diffTexPaths.size() > 0)
    {
        if(diffTexsRGB.size() > 0)
            shape.loadTextures(diffTexPaths, TextureType::DIFFUSE, diffTexsRGB);
        else
            shape.loadTextures(diffTexPaths, TextureType::DIFFUSE);
    }
    
    if(specTexPaths.size() > 0)
    {
        if(specTexsRGB.size() > 0)
            shape.loadTextures(specTexPaths, TextureType::DIFFUSE, specTexsRGB);
        else
            shape.loadTextures(specTexPaths, TextureType::DIFFUSE);
    }
    
    if(normTexPaths.size() > 0)
    {
        if(normTexsRGB.size() > 0)
            shape.loadTextures(normTexPaths, TextureType::DIFFUSE, normTexsRGB);
        else
            shape.loadTextures(normTexPaths, TextureType::DIFFUSE);
    }
    
    if(bumpTexPaths.size() > 0)
    {
        if(bumpTexsRGB.size() > 0)
            shape.loadTextures(bumpTexPaths, TextureType::DIFFUSE, bumpTexsRGB);
        else
            shape.loadTextures(bumpTexPaths, TextureType::DIFFUSE);
    }
        
    
    // ... now modify all the properties of this shape based on the rest of the json file
    shape.m_transform.position = j.value("position", glm::vec3(0.f));
    shape.m_transform.rotation = j.value("rotation", glm::vec3(0.f));
    shape.m_transform.scale = j.value("scale", glm::vec3(1.f));
    json jMat;
    try
    {
        jMat = j.at("material");
    }
    catch(json::out_of_range& e)
    {
        // Fill `jMat` with something so that we can use `value()` later.  `value()` can't be used
        //  on a null json object.
        jMat["trash"] = 4;
    }
    shape.m_material.ambient = jMat.value("ambient", glm::vec3(.2f));
    shape.m_material.diffuse = jMat.value("diffuse", glm::vec3(1.f));
    shape.m_material.specular = jMat.value("specular", glm::vec3(.5f));
    shape.m_material.shininess = jMat.value("shininess", 32.0);
    
    if(j.value("flipNormals", false))
    {
        shape.FlipNormals();
    }
    
}


// ********  Cube  ********** //
inline void to_json(json& j, const Cube& cube)
{
    j = cube.toJson();
}

inline void from_json(const json& j, Cube& cube)
{
    JsonToShape(j, cube);
    cube.m_shapeType = GameObject::CUBE;
}


// ********  Plane  ********** //
inline void to_json(json& j, const Plane& plane)
{
    j = plane.toJson();
}

inline void from_json(const json& j, Plane& plane)
{
    JsonToShape(j, plane);
    plane.m_shapeType = GameObject::PLANE;
}





//***************************************************************
//            Model Objects
//***************************************************************
inline void to_json(json& j, const Model& model)
{
    j = model.toJson();
}

inline void from_json(const json& j, Model& model)
{
    std::string path = j.value("path", std::string());
    bool sRGB = j.value("sRGB", Texture::sRGBDefault);
    model = Model(path.c_str(), sRGB);
    model.m_transform.position = j.value("position", glm::vec3(0.f));
    model.m_transform.rotation = j.value("rotation", glm::vec3(0.f));
    model.m_transform.scale = j.value("scale", glm::vec3(1.f));
}


//*********************************************
//            Light Objects
//*********************************************

// ********  Point Light  ********** //
inline void to_json(json& j, const PointLight& light)
{
    j = light.toJson();
}
inline void from_json(const json& j, PointLight& light)
{
    light = PointLight{j.value("position", glm::vec3(0.f))};
    light.setAmbient(j.value("ambient", .1f));
    light.setDiffuse(j.value("diffuse", glm::vec3(1.f)));
    light.setSpecular(j.value("specular", .4f));
    light.setAtten(j.value("constAtten", 1.f),
                   j.value("linAtten", .1f),
                   j.value("quadAtten", .02f));
    light.setScale(j.value("scale", .05f));
}

// ********  Directional Light  ********** //
inline void to_json(json& j, const DirLight& light)
{
    j = light.toJson();
}
inline void from_json(const json& j, DirLight& light)
{
    light = DirLight{j.value("direction", glm::vec3(0.f, 0.f, -1.f))};
    light.setAmbient(j.value("ambient", .1f));
    light.setDiffuse(j.value("diffuse", glm::vec3(1.f)));
    light.setSpecular(j.value("specular", .5f));
    light.setAtten(j.value("constAtten", 1.f),
                   j.value("linAtten", .1f),
                   j.value("quadAtten", .02f));
}

// ********  Spot Light  ********** //
inline void to_json(json& j, const SpotLight& light)
{
    j = light.toJson();
}
inline void from_json(const json& j, SpotLight& light)
{
    light = SpotLight{j.value("position", glm::vec3(0.f)),
        j.value("direction", glm::vec3(0.f, 0.f, -1.f)),
        j.value("innerCutoff", glm::cos(glm::radians(22.f))),
        j.value("outerCutoff", glm::cos(glm::radians(27.f)))
    };
    light.setAmbient(j.value("ambient", .1f));
    light.setDiffuse(j.value("diffuse", glm::vec3(1.f)));
    light.setSpecular(j.value("specular", .5f));
    light.setAtten(j.value("constAtten", 1.f),
                   j.value("linAtten", .1f),
                   j.value("quadAtten", .02f));
}




//*********************************************
//            Camera <--> JSON
//*********************************************
inline void to_json(json& j, const Camera& cam)
{
    j = cam.toJson();
}
inline void from_json(const json& j, Camera& cam)
{
    cam = Camera{j.value("fov", 45.f),
        j.value("aspectRatio", 4.f/3.f),
        j.value("nearField", .1f),
        j.value("farField", 100.f),
        j.value("position", glm::vec3(0.f)),
        j.value("pitch", 0.f),
        j.value("yaw", 0.f)
    };
}
#endif /* Serialize_h */
