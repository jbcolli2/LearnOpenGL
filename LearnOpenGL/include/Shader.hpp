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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
    
    void setUniform3f(const std::string& uniformName, float r, float g, float b)
    {
        glUniform3f(glGetUniformLocation(program, uniformName.c_str()), r, g, b);
    }
    
    void setUniform1i(const std::string& uniformName, int i)
    {
        glUniform1i(glGetUniformLocation(program, uniformName.c_str()), i);
    }
    
    void setUniformTex(const std::string& uniformName, int i)
    {
        setUniform1i(uniformName, i);
    }
    
    
    void setUniformMatrix4f(const std::string& uniformName, glm::mat4 mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(program, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    
    
private:
    std::string loadShaderFile(const char* path);
};





#endif /* Shader_h */
