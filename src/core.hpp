#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>

/*
THIS FILE CONTAINS ALL CORE MECHANISMS FOR OPENGL GLFW FUNCTIONALITY
 - LOADING RESOURCES
 - CAMERAS/TRANSFORMS
 - WINDOW CREATION
 - GLFW INIT
 - ETC
*/

class ResourceManager{
private:

    //Shader program map

    //Sprite map

    //--- later
    //Sound loader
public:

    //load shader

    //get shader


};

/*
    Class to handle input
    Gets all key input and marks flags for that frame
*/
class InputManager {

};


/*
    Class to handle logging
        Channels
            - Info
            - cout
            - err
        
        Message contains:
            - <message>
            - timestamp

        Can define output stream
            - File
            - cout
            - std::err
            - etc

        Automatic indenting
            - Indentation level tracked, increased and decreased when printing things

        logger.indent()
        logger.log(message, channel)
        logger.std(message)
        logger.err(message)
        etc
        logger.outdent()

*/
class Logger {

};


/*
    Class to handle most basic opengl/glfw functionality
    //TODO Likely want to expand this in the future to wrap other objects in this file as well

*/
class Engine {
    //private:
        //Collection of objects in the engine
public:
    InputManager input_manager;
    ResourceManager resource_manager;
    bool running;
    void initFLGW();
    GLFWwindow* createWindow();

    void processInput();
    void renderWindow();

};
