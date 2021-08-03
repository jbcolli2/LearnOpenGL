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




class Scene
{
    std::vector< std::unique_ptr<Shape> > v_shapes;
    
    
public:
    Scene() {

        
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
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        for(int ii = 0; ii < v_shapes.size(); ++ii)
        {
            v_shapes[ii]->draw();
        }
        
    };
};







//****** Basic GLFW Stuff ********//

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);
};


#endif /* BasicDraw_h */
