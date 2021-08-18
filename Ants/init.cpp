#include "init.h"



bool Keys[1024];



/***************************************************************************
 *  Is called once when the window is resized containing a pointer to the
 *  resized window as well as the new width and height
 ***************************************************************************/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);    //Set a new viewport of the size of the window

    /*  NOTE:   could do something more fancy with adjusting the camera size
     *          but this approach works best for games of fixed UI size and such*/
}

/***************************************************************************
 *  Is called once when the mouse is moved with its new position on screen
 ***************************************************************************/
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

}

/***************************************************************************
 *  Is called once when a key is pressed or released
 ***************************************************************************/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    static bool wire = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {   
        if (wire)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        wire = !wire;
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Keys[key] = true;
        else if (action == GLFW_RELEASE)
            Keys[key] = false;
    }
}

/***************************************************************************
 *  Is called once when the scrollwheel is moved
 *  +yoffset is scrolling up and -yoffset is scrolling down
 ***************************************************************************/
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void initGLAD()
{
    //Init GLAD to generate OS specific function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

GLFWwindow* createWindow(int width, int height, const char* title)
{
    glfwInit();                                                     //Basic set up for glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    

    

    //Create window and catch if failure
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    //Switch context and Set callbacks
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //Initialize GLAD then viewport
    initGLAD();
    glViewport(0, 0, width, height);
    glEnable(GL_PROGRAM_POINT_SIZE); //Can be controlled by setting gl_PointSize in the vertex shader
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return window;
}


void captureCursor(GLFWwindow* window, bool active)
{
    if (active)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}