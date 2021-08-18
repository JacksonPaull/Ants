#ifndef ANTS_INIT_H
#define ANTS_INIT_H

//OpenGL includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>


//GLM includes
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Other assorted base includes
#include <iostream>

#include "resourceManager.h"

bool Keys[];

// FUNCTION SIGNATURES
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void initGLAD();
GLFWwindow* createWindow(int width, int height, const char* title);
void captureCursor(GLFWwindow* window, bool active);

#endif //ANTS_INIT_H
