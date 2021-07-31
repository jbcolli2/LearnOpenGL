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
    
    int vertColorLocation;

public:
    
    Shader(const char* vsPath = "", const char* fsPath = "");
    
    void makeProgram();
    
    void useProgram(){ glUseProgram(program);};
    
    void setColorUniform(float r, float g, float b, float a)
    {
        glUniform4f(vertColorLocation, r, g, b, a);
    }
    
    
private:
    std::string loadShaderFile(const char* path);
};





#endif /* Shader_h */
