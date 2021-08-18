
/******************************************************************************************************************************************
*															Include Section																  *
******************************************************************************************************************************************/

//Includes everything needed for base openGL functions as well as houses init and callback functions
#include "init.h"
#include "grid.h"
#include "ant.h"

//Space for more
	//Irrklang
	//Custom classes



/******************************************************************************************************************************************
*															Macros Section																  *
******************************************************************************************************************************************/

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 900



int main()
{
	/************************************************ Window Init ******************************************************/
	GLFWwindow* window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ants");   //Create first window

	/************************************************ Game Init Phase **************************************************/
	// deltaTime variables
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


    ResourceManager::LoadShader("C:/Users/Jackson/source/repos/Ants/Resources/Shaders/base.vs",     //Vs Source
                                "C:/Users/Jackson/source/repos/Ants/Resources/Shaders/base.fs",     //Fs Source
                                nullptr,    //Geometry Shader Source
                                "base");    //Shader Name
    ResourceManager::LoadShader("C:/Users/Jackson/source/repos/Ants/Resources/Shaders/grid.vs",     //Vs Source
                                "C:/Users/Jackson/source/repos/Ants/Resources/Shaders/grid.fs",     //Fs Source
                                 nullptr,    //Geometry Shader Source
                                "grid");    //Shader Name
    ResourceManager::LoadShader("C:/Users/Jackson/source/repos/Ants/Resources/Shaders/ant.vs",     //Vs Source
        "C:/Users/Jackson/source/repos/Ants/Resources/Shaders/ant.fs",     //Fs Source
        nullptr,    //Geometry Shader Source
        "ant");


    ResourceManager::LoadTexture("C:/Users/Jackson/source/repos/Ants/Resources/Sprites/ant.png", true, "ant");

    //-----------------
    glm::mat4 ortho = glm::ortho(0.0f, WINDOW_WIDTH + 0.0f , WINDOW_HEIGHT + 0.0f,  0.0f, -100.0f, 100.0f);
    Shader s = ResourceManager::GetShader("grid");
    Grid grid(50, 0, 900, WINDOW_HEIGHT, 10, s, &ortho);

    glEnable(GL_MULTISAMPLE);

    Ant ant = Ant();

	/************************************************ Game Main Loop ***************************************************/
    while (!glfwWindowShouldClose(window))
    {
        //Set deltatime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //std::cout << "fps: " << 1 / deltaTime << std::endl;

        //Process Input
        glfwPollEvents();

        //Render
        glClearColor(0.09f, 0.25f, 0.5f, 1.0f);   //Draw black over entire screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        
        

        grid.processInput();
        grid.draw();

        ant.draw();
        
        glfwSwapBuffers(window);
    }

    /******************************************* Clean Up ******************************************/
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}