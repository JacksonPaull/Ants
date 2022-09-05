#include "core.hpp"
#include "grid.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main()
{
    GLFWwindow* window = engine->getWindow();
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    engine->resource_manager.loadShader("base", "./shaders/base");
    //------------------

    Grid g =  Grid(0, 0, 200, 200, true, 3729);
    engine->addObject(&g);


    while (!glfwWindowShouldClose(window))
    {

        engine->processInput();
        engine->renderWindow();

        if(engine->input_manager.keyboard_key_pressed(GLFW_KEY_SPACE))
            std::cout << "Spacebar Pressed\n";

        if(engine->input_manager.keyboard_key_down(GLFW_KEY_SPACE))
            std::cout << "Spacebar down\n";

        if(engine->input_manager.keyboard_key_released(GLFW_KEY_SPACE))
            std::cout << "Spacebar Released\n";

        engine->input_manager.update();
        glfwPollEvents();
    }

    return 0;
}