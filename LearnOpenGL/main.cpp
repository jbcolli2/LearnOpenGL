//
//  main.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/25/21.
//


#define GL_SILENCE_DEPRECATION
#include <iostream>
//#include <glad/glad.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


void processInput(sf::Window& window, bool& running)
{
    // handle events
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            // end the program
            running = false;
        }
        else if (event.type == sf::Event::Resized)
        {
            // adjust the viewport when the window is resized
            glViewport(0, 0, event.size.width, event.size.height);
        }
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
    {
        running = false;
    }
    
    
}






int main(int argc, const char * argv[]) {
    
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.attributeFlags = sf::ContextSettings::Attribute::Core;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
    
    window.setActive(true);
    
    
    // run the main loop
        bool running = true;
        while (running)
        {
            processInput(window, running);
            
            // clear the buffers
            glClearColor(1.0f, 0.0f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw...

            // end the current frame (internally swaps the front and back buffers)
            window.display();
        }
    
    
    return 0;
}





