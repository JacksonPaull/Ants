#include "core.hpp"
#include "grid.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main()
{
    
    Engine engine = Engine();
    GLFWwindow* window = engine.getWindow();
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    
    ResourceManager rm = ResourceManager();
    rm.loadShader("base", "./shaders/base");
    //------------------

    Grid* g = new Grid(0, 0, 200, 200, true, 3729);

    // render loop
    // -----------
    bool x = false;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !x) {
            //g->filterRegions(10);
            //g->triangulate();
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            x = true;
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && x) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            x = false;
        }

        
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        g->draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete g;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    delete &engine;
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}