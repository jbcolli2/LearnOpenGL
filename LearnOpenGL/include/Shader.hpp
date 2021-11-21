//
//  Shader.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#ifndef Shader_h
#define Shader_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
    unsigned int m_program;
    
    std::string m_vertShaderStr, m_fragShaderStr, m_geomShaderStr;
    

public:
    static Shader solidShader;
    
    Shader() {};
    Shader(std::string vsPath, std::string fsPath);
    Shader(std::string vsPath, std::string gsPath, std::string fsPath);
    
    void makeProgram();
    
    void useProgram(){ glUseProgram(m_program);};
    void stopUseProgram() {glUseProgram(0);};
    
    void setUniform1f(const std::string& uniformName, float f)
    {
        glUniform1f(glGetUniformLocation(m_program, uniformName.c_str()), f);
    }
    
    void setUniform4f(const std::string& uniformName, float r, float g, float b, float a)
    {
        glUniform4f(glGetUniformLocation(m_program, uniformName.c_str()), r, g, b, a);
    }
    
    void setUniform3f(const std::string& uniformName, float r, float g, float b)
    {
        glUniform3f(glGetUniformLocation(m_program, uniformName.c_str()), r, g, b);
    }
    
    void setUniform1i(const std::string& uniformName, int i)
    {
        glUniform1i(glGetUniformLocation(m_program, uniformName.c_str()), i);
    }
    
    void setUniformTex(const std::string& uniformName, int i)
    {
        setUniform1i(uniformName, i);
    }
    
    
    void setUniformMatrix4f(const std::string& uniformName, glm::mat4 mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_program, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    
    void bindUniformBlock(const std::string& uniformBlockName, int bindPoint)
    {
        glUniformBlockBinding( m_program, bindPoint, glGetUniformBlockIndex(m_program, uniformBlockName.c_str()) );
    }
    
    
private:
    std::string loadShaderFile(const char* path);
};





#endif /* Shader_h */
