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
    vertShader = loadShaderFile(vsPath.c_str());
    fragShader = loadShaderFile(fsPath.c_str());
}



void Shader::makeProgram()
{
    // Vertex Shader
    const char* vertShaderSource = vertShader.c_str();

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



    // Fragment Shader
    const char* fragShaderSource = fragShader.c_str();

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


    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    int linkSuccess;
    glGetShaderiv(program, GL_LINK_STATUS, &linkSuccess);
    if(!fragSuccess)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "ShaderProgram::Linkerr::Fail  " << infoLog << std::endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    
    
    
    
    // ****** Uniform ********//
//    vertColorLocation = glGetUniformLocation(program, "ourColor");
}





std::string Shader::loadShaderFile(const char *path)
{
    std::ifstream shaderFile(path);
    try {
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
