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
#include "Shapes.hpp"



class Scene
{
    std::vector< std::unique_ptr<Shape> > v_shapes;
    
    
public:
    Scene() {
        float vertsTri[] = {
            -0.9f, -.5f, 0.0f,
            -.5f,  -.5f, 0.0f,
            -.5f, .5f,  0.0f
        };
        
        v_shapes.emplace_back(std::make_unique<Triangle>(vertsTri));
        
        
        float vertsSq[] = {
                0.0f, 0.0f, 0.0f,
                .5f, 0.0f, 0.0f,
                .5f, .75f, 0.0f,
                0.0f, .75f, 0.0f
            };
        v_shapes.emplace_back(std::make_unique<Square>(vertsSq));
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
