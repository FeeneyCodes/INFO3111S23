#include "globalStuff.h"

#include "cLightManager.h"

extern cLightManager* g_pTheLights;// = NULL;

extern bool g_bShowDebugLightSpheres;// = true;


//    void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        ::g_bShowDebugLightSpheres = !::g_bShowDebugLightSpheres;
    }

   const float CAMERA_MOVE_SPEED = 0.1f;

    if (mods == GLFW_MOD_SHIFT)
    {
        // Shift ONLY is down
    }

    // Note this is a bit mask 
    // 0001 0010 0100 
    // 0111 &
    // 0110
    // ----
    // 0000
    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Shift is down and maybe other keys as well
    }


    //if ( key == GLFW_KEY_A )    
    //{
    //    // Move "left"
    //    ::g_cameraEye.x += CAMERA_MOVE_SPEED;
    //}
    //if ( key == GLFW_KEY_D )    
    //{
    //    // Move "right"
    //    ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
    //}

    //if ( key == GLFW_KEY_W )    
    //{
    //    // Move "forward"
    //    ::g_cameraEye.z += CAMERA_MOVE_SPEED;
    //}
    //if ( key == GLFW_KEY_S )    
    //{
    //    // Move "backwards"
    //    ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
    //}

    //if ( key == GLFW_KEY_Q )    
    //{
    //    // Move "down"
    //    ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
    //}
    //if ( key == GLFW_KEY_E )    
    //{
    //    // Move "up"
    //    ::g_cameraEye.y += CAMERA_MOVE_SPEED;
    //}
}

bool isShiftDown(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        return true;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}

void handleKeyboardInput(GLFWwindow* window)
{
    const float CAMERA_MOVE_SPEED = 0.1f;
    const float LIGHT_MOVE_SPEED = 0.05f;

    if (isShiftDown(window))
    {
        // I'll be messing with the lights...
        if (glfwGetKey(window, GLFW_KEY_A))
        {
            // Move "left"
            ::g_pTheLights->myLights[0].position.x += LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_D))
        {
            // Move "right"
            ::g_pTheLights->myLights[0].position.x -= CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_W))
        {
            // Move "forward"
            ::g_pTheLights->myLights[0].position.z += CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
            // Move "backwards"
            ::g_pTheLights->myLights[0].position.z -= CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_Q))
        {
            // Move "down"
            ::g_pTheLights->myLights[0].position.y -= CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_E))
        {
            // Move "up"
            ::g_pTheLights->myLights[0].position.y += CAMERA_MOVE_SPEED;
        }

        const float LINEAR_ATTEN_CHANGE_PERCENT = 0.01f;

        if (glfwGetKey(window, GLFW_KEY_1))
        {
            ::g_pTheLights->myLights[0].atten.y *= (1.0f + LINEAR_ATTEN_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_2))
        {
            ::g_pTheLights->myLights[0].atten.y *= (1.0f - LINEAR_ATTEN_CHANGE_PERCENT);
        }

        if (glfwGetKey(window, GLFW_KEY_3))
        {
            ::g_pTheLights->myLights[0].atten.x *= (1.0f + LINEAR_ATTEN_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_4))
        {
            ::g_pTheLights->myLights[0].atten.z *= (1.0f - LINEAR_ATTEN_CHANGE_PERCENT);
        }
    }
    else   // Do camera stuff
    {
        // Do camera stuff
        if (glfwGetKey(window, GLFW_KEY_A))
        {
            // Move "left"
            ::g_cameraEye.x += CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_D))
        {
            // Move "right"
            ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_W))
        {
            // Move "forward"
            ::g_cameraEye.z += CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
            // Move "backwards"
            ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_Q))
        {
            // Move "down"
            ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_E))
        {
            // Move "up"
            ::g_cameraEye.y += CAMERA_MOVE_SPEED;
        }
    }//if (isShiftDown(window)

    return;
}
