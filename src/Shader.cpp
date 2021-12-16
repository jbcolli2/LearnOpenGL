//
//  Shader.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Shader.hpp"

Shader Shader::solidShader = Shader();

Shader::Shader(std::string vsPath, std::string fsPath)
{
    m_vertShaderStr = loadShaderFile(vsPath.c_str());
    m_fragShaderStr = loadShaderFile(fsPath.c_str());
}

Shader::Shader(std::string vsPath, std::string gsPath, std::string fsPath) : Shader(vsPath, fsPath)
{
    m_geomShaderStr = loadShaderFile(gsPath.c_str());
}



void Shader::makeProgram()
{
    // Vertex Shader
    const char* vertShaderSource = m_vertShaderStr.c_str();

    unsigned int vertShader;
    vertShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertShader, 1, &vertShaderSource, NULL);
    glCompileShader(vertShader);

    // Check compile status
    int vertSuccess;
    char infoLog[512];
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vertSuccess);
    if(!vertSuccess)
    {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cout << "VertexShader::Compile::Fail  " << infoLog << std::endl;
    }
    
    
    // Geometry Shader
    unsigned int geomShader = 0;
    if(!m_geomShaderStr.empty())
    {
        const char* geomShaderSource = m_geomShaderStr.c_str();

        geomShader = glCreateShader(GL_GEOMETRY_SHADER);

        glShaderSource(geomShader, 1, &geomShaderSource, NULL);
        glCompileShader(geomShader);

        // Check compile status
        int geomSuccess;
        glGetShaderiv(geomShader, GL_COMPILE_STATUS, &geomSuccess);
        if(!geomSuccess)
        {
            glGetShaderInfoLog(geomShader, 512, NULL, infoLog);
            std::cout << "GeometryShader::Compile::Fail  " << infoLog << std::endl;
        }
    }
    



    // Fragment Shader
    const char* fragShaderSource = m_fragShaderStr.c_str();

    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);

    // Check compile status
    int fragSuccess;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragSuccess);
    if(!fragSuccess)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "FragmentShader::Compile::Fail  " << infoLog << std::endl;
    }


    m_program = glCreateProgram();
    glAttachShader(m_program, vertShader);
    if(!m_geomShaderStr.empty())
    {
        glAttachShader(m_program, geomShader);
    }
    glAttachShader(m_program, fragShader);
    glLinkProgram(m_program);
    int linkSuccess;
    glGetShaderiv(m_program, GL_LINK_STATUS, &linkSuccess);
    if(!fragSuccess)
    {
        glGetShaderInfoLog(m_program, 512, NULL, infoLog);
        std::cout << "ShaderProgram::Linkerr::Fail  " << infoLog << std::endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    
    
    
    
    // ****** Uniform ********//
//    vertColorLocation = glGetUniformLocation(program, "ourColor");
}





std::string Shader::loadShaderFile(const char *path)
{
    std::ifstream shaderFile;
    try {
        shaderFile.open(path, std::ifstream::in);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        
        shaderFile.close();
        
        return shaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "Error reading shader file at " << path << std::endl;
        shaderFile.close();
        return std::string("");
    }
    
    
    
    
    
    
}
