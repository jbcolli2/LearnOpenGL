//
//  BasicDraw.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#ifndef BasicDraw_h
#define BasicDraw_h

#include <stdio.h>
#include <vector>
#include <string>

#include "Shapes.hpp"
#include "Shader.hpp"




class Scene
{
    std::vector< std::unique_ptr<Shape> > v_shapes;
    Shader shader;
    
    
public:
    Scene(Shader shader) : shader(shader) {

        
//        std::vector<Vert3x3f> vertsTri = {
//            Vert3x3f(-.9f, -.5f, 0.0f, 1.0f, 0.0f, 0.0f),
//            Vert3x3f(-.5f, -.5f, 0.0f, 1.0f, 0.0f, 0.0f),
//            Vert3x3f(-.5f, .5f, 0.0f, 0.0f, 0.0f, 1.0f)
//        };
//
//
//        v_shapes.emplace_back(std::make_unique<Triangle<Vert3x3f>>(vertsTri));
        
        
        
//        std::vector<Vert3x3f> vertsSq = {
//            Vert3x3f(-.5f, -.5f, 0.0f,   1.0f, 0.0f, 0.0f),
//            Vert3x3f(.5f, -.5f, 0.0f,    0.0f, 1.0f, 0.0f),
//            Vert3x3f(.5f, .5f, 0.0f,     0.0f, 0.0f, 1.0f),
//            Vert3x3f(-.5f, .5f, 0.0f,    1.0f, 1.0f, 1.0f)
//        };
//
//
//        v_shapes.emplace_back(std::make_unique<Square <Vert3x3f> >(vertsSq));
        
        
        
        
        
//        std::vector<Vert3x3x2f> vertsTri = {
//            Vert3x3x2f(-.9f, -.5f, 0.0f, 1.0f, 0.0f, 0.0, 0, 0),
//            Vert3x3x2f(-.5f, -.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0, 0),
//            Vert3x3x2f(-.5f, .5f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0)
//        };
//
//
//        v_shapes.emplace_back(std::make_unique<Triangle<Vert3x3x2f>>(vertsTri));
        
        std::vector<Vert3x3x2f> vertsSq = {
            Vert3x3x2f(-.5f, -.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f),
            Vert3x3x2f(.5f, -.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f),
            Vert3x3x2f(.5f, .5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f),
            Vert3x3x2f(-.5f, .5f, 0.0f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f)
        };

        
        std::string imageFolder = "/Users/jebcollins/Documents/Personal/GameDev/C++/LearnOpenGL/LearnOpenGL/include/";

        v_shapes.emplace_back(std::make_unique<Square <Vert3x3x2f> >(vertsSq));
        v_shapes[0]->loadTexture(imageFolder + "container.jpeg");
        stbi_set_flip_vertically_on_load(true);
        v_shapes[0]->loadTextureAlpha(imageFolder + "awesomeface.png");
        

    };
    
    
    
    
    
    void draw()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        shader.setUniform1i("tex1", 0);
        shader.setUniform1i("tex2", 1);
        
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));
        transform = glm::scale(transform, glm::vec3(0.3f, .3f, .3f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.f, 0.f, 1.0f));

        
        shader.setUniformMatrix4f("transform", transform);
        
        
        v_shapes[0]->draw();
        
        
        transform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
        float scale_x = -std::sin(glfwGetTime());
        float scale_y = std::cos(glfwGetTime());
        transform = glm::scale(transform, glm::vec3(scale_x, scale_y, 0.f));
        shader.setUniformMatrix4f("transform", transform);

        v_shapes[0]->draw();
//        for(int ii = 0; ii < v_shapes.size(); ++ii)
//        {
//            v_shapes[ii]->draw();
//        }
        
    };
};










#endif /* BasicDraw_h */
