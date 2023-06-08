#include "globalStuff.h"

#include "cLightManager.h"

#include "cMeshObject.h"
#include <string>

extern cLightManager* g_pTheLights;// = NULL;

extern bool g_bShowDebugLightSpheres;// = true;

unsigned int g_SelectedLightIndex = 0;

// Defined in theMain.cpp
cMeshObject* pFindObjectByFriendlyName(std::string theName);


//    void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Turns off and on the "debug spheres"
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        ::g_bShowDebugLightSpheres = !::g_bShowDebugLightSpheres;
    }

    // Is the shift and ONLY the shift down?
    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        if ((key == GLFW_KEY_7) && (action == GLFW_PRESS))
        {
            ::g_SelectedLightIndex++;
            // Too large? 
            if (::g_SelectedLightIndex == cLightManager::NUMBEROFLIGHTS )
            {
                ::g_SelectedLightIndex = 0;
            }
        }        

        if ((key == GLFW_KEY_6) && (action == GLFW_PRESS))
        {
            if ( ::g_SelectedLightIndex == 0 )
            {
                ::g_SelectedLightIndex = cLightManager::NUMBEROFLIGHTS - 1;
            }
            else
            {
                ::g_SelectedLightIndex--;
            }
        }       

    }// if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)


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
    //const float CAMERA_MOVE_SPEED = 10.0f;
    //const float LIGHT_MOVE_SPEED = 5.0f;

    if (isShiftDown(window))
    {
        // I'll be messing with the lights...
        if (glfwGetKey(window, GLFW_KEY_A))
        {
            // Move "left"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.x += LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_D))
        {
            // Move "right"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.x -= CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_W))
        {
            // Move "forward"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.z += CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
            // Move "backwards"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.z -= CAMERA_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_Q))
        {
            // Move "down"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.y -= CAMERA_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_E))
        {
            // Move "up"
            ::g_pTheLights->myLights[g_SelectedLightIndex].position.y += CAMERA_MOVE_SPEED;
        }

        const float LINEAR_ATTEN_CHANGE_PERCENT = 0.01f;

        if (glfwGetKey(window, GLFW_KEY_1))
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].atten.y *= (1.0f + LINEAR_ATTEN_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_2))
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].atten.y *= (1.0f - LINEAR_ATTEN_CHANGE_PERCENT);
        }

        if (glfwGetKey(window, GLFW_KEY_3))
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].atten.z *= (1.0f + LINEAR_ATTEN_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_4))
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].atten.z *= (1.0f - LINEAR_ATTEN_CHANGE_PERCENT);
        }

        //glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
        const float SPOT_LIGHT_ANGLE_CHANGE_PERCENT = 0.001f;

        if (glfwGetKey(window, GLFW_KEY_UP))        // Increase inner angle
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].param1.y *= (1.0f + SPOT_LIGHT_ANGLE_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN))      // Decrease inner angle
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].param1.y *= (1.0f - SPOT_LIGHT_ANGLE_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT))    // Increase outer angle
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].param1.z *= (1.0f + SPOT_LIGHT_ANGLE_CHANGE_PERCENT);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT))      // Decrease outer angle
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].param1.z *= (1.0f - SPOT_LIGHT_ANGLE_CHANGE_PERCENT);
        }

        if (glfwGetKey(window, GLFW_KEY_5))
        {
//            ::g_pTheLights->myLights[g_SelectedLightIndex].param2.x = 1.0f;
            ::g_pTheLights->myLights[g_SelectedLightIndex].TurnOn();
        }
        if (glfwGetKey(window, GLFW_KEY_6))
        {
            ::g_pTheLights->myLights[g_SelectedLightIndex].TurnOff();
//            ::g_pTheLights->myLights[g_SelectedLightIndex].param2.x = 0.0f;
        }    

        // Oh no!!!
//        if (glfwGetKey(window, GLFW_KEY_7))
//        {
//            ::g_SelectedLightIndex++;
//        }



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


        // Change texture blending
        if (glfwGetKey(window, GLFW_KEY_M))
        {
            cMeshObject* pQuad = pFindObjectByFriendlyName("2D Quad Imposter");
            if (pQuad)
            {
                // More Tay Tay
                pQuad->textureMixingRatio[0] += 0.001f;
                if ( pQuad->textureMixingRatio[0] > 1.0f )
                {
                    pQuad->textureMixingRatio[0] = 1.0f;
                }
                // 
                pQuad->textureMixingRatio[1] = (1.0f - pQuad->textureMixingRatio[0]);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_N))
        {
            cMeshObject* pQuad = pFindObjectByFriendlyName("2D Quad Imposter");
            if (pQuad)
            {
                // More John Wick
                pQuad->textureMixingRatio[1] += 0.001f;
                if (pQuad->textureMixingRatio[1] > 1.0f)
                {
                    pQuad->textureMixingRatio[1] = 1.0f;
                }
                // 
                pQuad->textureMixingRatio[0] = (1.0f - pQuad->textureMixingRatio[1]);
            }
        }

    }//if (isShiftDown(window)

    return;
}
