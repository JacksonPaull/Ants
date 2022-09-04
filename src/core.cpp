#include "core.hpp"


/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   *************************************************************************************************** Resource Manager *****************************************************************************************************
   ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 

ResourceManager::ResourceManager() {
    this->shaders = std::map<string, unsigned int>();

}

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
    if(shaders.find(name) == shaders.end()) {
        std::cout << "ERROR Attempted to activate shader of unknown name: " << name << std::endl;
        return;
    }

    glUseProgram(shaders[name]);
}

void ResourceManager::setDefaultShader(string name) {
    if(shaders.find(name) == shaders.end()) {
        std::cout << "ERROR Attempted to set default shader of unknown name: " << name << std::endl;
        return;
    }

    this->default_shader = shaders[name];
}

void ResourceManager::deactivateShader() {
    glUseProgram(default_shader);
}





/* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   ******************************************************************************************************** Engine *********************************************************************************************************
   ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


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

    this->window = window;
}

GLFWwindow* Engine::getWindow() {
    return this->window;
}

void Engine::renderWindow() {

}

Engine::Engine() {
    initGLFW();
    createWindow(SCR_WIDTH, SCR_HEIGHT);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }   
}

Engine::~Engine() {


    glfwTerminate();
}
