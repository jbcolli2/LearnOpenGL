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
 \param filename - name of the file
 */
inline void JsonToFile(const json& j, const std::string& path)
{
    std::fstream file;
    file.open(path, std::ifstream::out);
    file << j.dump(2);
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
inline void shapeToJson(json& j, const T& shape)
{
    json jTextures = shape.getTextures();
    Material mat = shape.m_material;
    
    j = {
        {"type", shape.m_shapeType},
        {"position", shape.m_transform.position},
        {"rotation", shape.m_transform.rotation},
        {"scale", shape.m_transform.scale},
        {"material",
            {
            {"diffuse", mat.diffuse},
            {"specular", mat.specular},
            {"ambient", mat.ambient},
            {"shininess", mat.shininess}
            }
        }
    };
    j.insert(jTextures.begin(), jTextures.end());
    
    
}



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
    
    // ... then create a new shape with these texture paths ...
    shape = T(diffTexPaths, specTexPaths, normTexPaths, bumpTexPaths);
    
    // ... now modify all the properties of this shape based on the rest of the json file
    shape.m_transform.position = j.value("position", glm::vec3(0.f));
    shape.m_transform.rotation = j.value("rotation", glm::vec3(0.f));
    shape.m_transform.scale = j.value("scale", glm::vec3(1.f));
    json jMat;
    try
    {
        jMat = j.at("material");
    }
    catch(json::out_of_range& e){}
    shape.m_material.ambient = jMat.value("ambient", glm::vec3(.2f));
    shape.m_material.diffuse = jMat.value("diffuse", glm::vec3(1.f));
    shape.m_material.specular = jMat.value("specular", glm::vec3(.5f));
    
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
    light.setSpecular(j.value("specular", .5f));
    light.setAtten(j.value("constAtten", 1.f),
                   j.value("linAtten", .1f),
                   j.value("quadAtten", .02f));
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
