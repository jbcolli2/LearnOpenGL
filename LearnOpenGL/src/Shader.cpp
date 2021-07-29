//
//  Shader.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <iostream>

#include "Shader.hpp"



Shader::Shader(const char* vertFile, const char* fragFile)
{
    vertShader = "#version 330 core\n"
    "layout (location = 0) in vec3 pos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(pos.x,pos.y,pos.z, 1.0f);\n"
    "}\0";
    
    
    fragShader = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
        "FragColor = vec4(0.3f, 1.0f, 1.0f, 1.0f);\n"
    "}\0";
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
}
