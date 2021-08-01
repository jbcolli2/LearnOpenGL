//
//  Shader.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#ifndef Shader_h
#define Shader_h

#include <GLFW/glfw3.h>
#include <string>


class Shader
{
    unsigned int program;
    
    std::string vertShader, fragShader;
    

public:
    
    Shader(std::string vsPath = "", std::string fsPath = "");
    
    void makeProgram();
    
    void useProgram(){ glUseProgram(program);};
    
    void setUniform1f(const std::string& uniformName, float f)
    {
        glUniform1f(glGetUniformLocation(program, uniformName.c_str()), f);
    }
    
    void setUniform4f(const std::string& uniformName, float r, float g, float b, float a)
    {
        glUniform4f(glGetUniformLocation(program, uniformName.c_str()), r, g, b, a);
    }
    
    void setUniform1i(const std::string& uniformName, int i)
    {
        glUniform1i(glGetUniformLocation(program, uniformName.c_str()), i);
    }
    
    
private:
    std::string loadShaderFile(const char* path);
};





#endif /* Shader_h */
