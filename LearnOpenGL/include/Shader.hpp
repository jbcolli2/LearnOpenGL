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
    Shader(const char* vertFile = "", const char* fragFile = "");
    
    void makeProgram();
    
    void useProgram(){ glUseProgram(program);};
};





#endif /* Shader_h */
