#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>


/*
THIS FILE CONTAINS ALL CORE MECHANISMS FOR OPENGL GLFW FUNCTIONALITY
 - LOADING RESOURCES
 - CAMERAS/TRANSFORMS
 - WINDOW CREATION
 - GLFW INIT
 - ETC
*/


using std::string;
class ResourceManager{
    private:
        string readFile(string path);
        unsigned int createShader(string name, string vsource, string fsource);
        std::map<string, unsigned int> shaders;
        unsigned int default_shader = -1;

        //Shader program map

        //Sprite map

        //--- later
        //Sound loader
    public:
        ResourceManager();
        bool shaderExists(string name);
        bool loadShader(string name, string path);
        bool loadShader(string name, string vpath, string fpath);
        void activateShader(string name);
        void setDefaultShader(string name);
        void deactivateShader();
};

/*
    Class to handle input
    Gets all key input and marks flags for that frame

    primary device <- device to use for input checking if device is not specified

    Set of devices, each with their own 
        name
        type
            Keyboard/Mouse
            Gamepad (Controller)

        key           := If the key is currently active
        key_pressed   := If the key was activated this frame
        key_released  := If the key was released this frame

    callback
        Find which input device the key press came from
        assign that device with a key pressed
*/
class InputManager {
    public:
        class Device {
            public:
                string name;
                std::vector<bool> key;
                std::vector<bool> key_prev;
            
                Device();
        };

        Device keyboard;
        Device mouse;
        std::vector<Device> gamepads;

        InputManager();

        void detectGamepads();
        void update();

        bool keyboard_key_up(int key);
        bool keyboard_key_down(int key);
        bool keyboard_key_pressed(int key);
        bool keyboard_key_released(int key);

        bool mouse_button_up(int key);
        bool mouse_button_down(int key);
        bool mouse_button_pressed(int key);
        bool mouse_button_released(int key);
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



class EngineObject {
    private:
    
    public:
        virtual void processInput() {

        }

        virtual void draw() {
            std::cout << "Using Base Draw Method" << std::endl;
        }
};


class Engine {
    private:
            //Collection of objects in the engine
        GLFWwindow* window;


        void initGLFW();
        void createWindow(int width, int height);

        std::vector<EngineObject*> objects;

    public:
        InputManager input_manager;
        ResourceManager resource_manager;
        bool running;
        
        Engine();
        ~Engine();

        GLFWwindow* getWindow();

        void processInput();
        void update();
        void renderWindow();
        void addObject(EngineObject* obj);
};

//GLOBAL ENGINE VARIABLE
extern Engine* engine;