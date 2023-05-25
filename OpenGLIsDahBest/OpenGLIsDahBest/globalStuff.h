#pragma once

// This is glad is seen before glfw
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// ALl variable are extern and NEVER assigned anything 
// (in the header file)
extern glm::vec3 g_cameraEye;// = glm::vec3(0.0, 0.0, -4.0f);
extern glm::vec3 g_cameraTarget;// = glm::vec3(0.0f, 0.0f, 0.0f);
extern glm::vec3 g_upVector;// = glm::vec3(0.0f, +1.0f, 0.0f);


// Unless it's templated, only put function signatures here
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void handleKeyboardInput(GLFWwindow* window);

extern unsigned int g_SelectedLightIndex;