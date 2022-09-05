#include "core.hpp"
Engine* engine = new Engine();

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   *************************************************************************************************** Resource Manager ****************************************************************************************************
   ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 

ResourceManager::ResourceManager() {
    this->shaders = std::map<string, unsigned int>();

}

/**
 * @brief Read in and return raw text of a file
 *
 * @param path Absolute or relative path to the file to read in 
 */
string ResourceManager::readFile(string path) {
    std::ifstream f(path);
    string rawText;

    if(!f.is_open()) {
        std::cout << "ERROR: Failed to open file";
        return "";
    }

    while(f) {
        string l;
        std::getline(f, l);
        rawText.append(l+"\n");
    }

    f.close();
    return rawText;
}

/**
 * @brief Check if a shader of name exists in the resource manager
 * 
 * @param name name to be looked up
 * @return Whether or not the shader exists
 */
bool ResourceManager::shaderExists(string name) {
    if(shaders.find(name) != shaders.end())
        return true;
    return false;
}

/**
 * @brief Load and compile a new shader
 * 
 * @param name name of the shader to be used by the resource manager
 * @param vsource raw text source of the vertex shader
 * @param fsource raw text source of the fragment shader
 */
unsigned int ResourceManager::createShader(string name, string vsource, string fsource) {

    const char* vs = vsource.data();
    const char* fs = fsource.data();

    //Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vs, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }


    //Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fs, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }


    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 

    //If a shader already exists with this name, overwrite it
    if(shaderExists(name)) {
        glDeleteProgram(shaders[name]);
    }

    this->shaders[name] = shaderProgram;
    if(this->default_shader == -1) {
        this->default_shader = shaderProgram;
        glUseProgram(shaderProgram);
    }
    return shaderProgram;
}

//TODO Add functionality for geometry shaders as well
bool ResourceManager::loadShader(string name, string path) {
    return loadShader(name, path+".vs", path+".fs");
}

bool ResourceManager::loadShader(string name, string vpath, string fpath) {
    string vsource = readFile(vpath);
    string fsource = readFile(fpath);

    unsigned int success = createShader(name, vsource, fsource);
    if(success != -1)
        return true;

    return false;
}

void ResourceManager::activateShader(string name) {
    if(!shaderExists(name)) {
        std::cout << "ERROR Attempted to activate shader of unknown name: " << name << std::endl;
        return;
    }

    glUseProgram(shaders[name]);
}

void ResourceManager::setDefaultShader(string name) {
    if(!shaderExists(name)) {
        std::cout << "ERROR Attempted to set default shader of unknown name: " << name << std::endl;
        return;
    }

    this->default_shader = shaders[name];
}

void ResourceManager::deactivateShader() {
    glUseProgram(default_shader);
}


/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   **************************************************************************************************** Input Manager ******************************************************************************************************
   ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 

InputManager::Device::Device() {
    key = std::vector<bool>(1024);
    key_prev = std::vector<bool>(1024);
}


InputManager::InputManager() {
    keyboard = Device();
    mouse = Device();
    gamepads = std::vector<Device>();
}


void InputManager::update() {
    keyboard.key_prev = keyboard.key;
    mouse.key_prev = mouse.key;
    for(Device gamepad : gamepads) {
        gamepad.key_prev = gamepad.key;
    }

}

bool InputManager::keyboard_key_down(int key) {
    return keyboard.key[key];
}

bool InputManager::keyboard_key_up(int key) {
    return !keyboard.key[key];
}

bool InputManager::keyboard_key_pressed(int key) {
    return (keyboard.key[key] && !keyboard.key_prev[key]);
}

bool InputManager::keyboard_key_released(int key) {
    return (!keyboard.key[key] && keyboard.key_prev[key]);
}




/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   **************************************************************************************************** Engine Object ******************************************************************************************************
   ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 





/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   ******************************************************************************************************** Engine *********************************************************************************************************
   ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) 
            engine->input_manager.keyboard.key[key] = true;
        
        else if (action == GLFW_RELEASE) 
            engine->input_manager.keyboard.key[key] = false;
        
        else if (action == GLFW_REPEAT) 
            engine->input_manager.keyboard.key[key] = true;
        
        //std::cout << "Key call back for key " << key << " with action " << action << std::endl;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

}

void joystick_callback(int jid, int event) {
    //Called when a joystick is connected or disconnected
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void Engine::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Engine::createWindow(int width, int height) {
    GLFWwindow* window = glfwCreateWindow(width, height, "Ant Pheremone Simulation", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl; //TODO Update to logger
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "Failed to initialize GLAD" << std::endl;

    glViewport(0, 0, width, height);

    this->window = window;
}

GLFWwindow* Engine::getWindow() {
    return this->window;
}

Engine::Engine() {
    engine = this;
    running = true;
    
    initGLFW();
    
    createWindow(SCR_WIDTH, SCR_HEIGHT);
    resource_manager = ResourceManager();
    input_manager = InputManager();

    objects = std::vector<EngineObject*>();

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetJoystickCallback(joystick_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Engine::~Engine() {


    glfwTerminate();
}

void Engine::addObject(EngineObject* obj) {
    objects.push_back(obj);
}

void Engine::processInput() {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window)) {
        glfwSetWindowShouldClose(window, true);
        running = false;
    }

    //Loop through all engine objects and call their process input method
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->processInput();
    }
}

void Engine::renderWindow() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for(int i = 0; i < objects.size(); i++) {
        objects[i]->draw();
    }
    
    glfwSwapBuffers(window);
}




