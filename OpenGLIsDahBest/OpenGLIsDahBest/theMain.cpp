//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "globalStuff.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>

#include "cShaderManager/cShaderManager.h"

#include "cVAOManager/cVAOManager.h"
#include "cLightManager.h"
// Now with textures!
#include "cBasicTextureManager/cBasicTextureManager.h"

#include "cMeshObject.h"

#include "cLightHelper.h"

//#include "../AssimpFileLoaderHelper/AssimpFileLoaderHelper.h"


struct sVertexXYZ_RGB
{
    float x, y, z;      // vec2 to vec3 
    float r, g, b;
};

// Google C++ dynamic array
// On the heap
//sVertexXYZ_RGB* pSexyVertex = new sVertexXYZ_RGB[3];
//
//// On the stack
//sVertexXYZ_RGB pVertexArray[3] =
//{
//    //   X      Y     Z     r     g     b
//    { -0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f },            // Spawn a vertex shader instance
//    {  0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f },
//    {  0.0f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f }
// 
//};

// Will allocate this in a moment...
sVertexXYZ_RGB* pVertexArray = NULL;    


glm::vec3 g_cameraEye = glm::vec3(0.0, 5.0, 12.0f);
glm::vec3 g_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_upVector = glm::vec3(0.0f, +1.0f, 0.0f);


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// This is a list of the objects we want to draw in this scene
//std::vector< cMeshObject > g_vecMeshesToDraw;       // From this...
std::vector< cMeshObject* > g_vec_pMeshesToDraw;    // ...to this (vector of pointers)
unsigned int g_selectedMeshIndex = 2;

// Returns true if there were no errors.
// If there are errors, vecErrors has error detail text.
bool LoadModels(cVAOManager* pModelManger, GLuint shaderProgram,
                std::vector< cMeshObject* >& vec_pMeshesToDraw,
                std::vector<std::string>& vecErrors,
                bool bLogProgressToConsole = true);

bool DrawObject(cMeshObject* pMeshToDraw,
                glm::mat4 matModel,
                cVAOManager* pModelManger,
                GLuint shaderProgram_ID);

void SetUpTexturesForMesh(cMeshObject* pCurrentMesh, 
                          GLuint shaderProgram_ID, 
                          cBasicTextureManager* pTheTextures);


//void ChangeLightSphereVisibility(bool bIsOn);
void DrawDebugLightSpheres(cVAOManager* pModelManager, GLuint shaderProgram_ID);

// Returns NULL (or 0) if not found
// Up to about 30-40 items, the linear search will be faster because of cache issues
cMeshObject* pFindObjectByFriendlyName(std::string theName);


cLightManager* g_pTheLights = NULL;
bool g_bShowDebugLightSpheres = true;


int main(void)
{
    GLFWwindow* window = NULL;
    GLint vpos_location = 0;
    GLint vcol_location = 0;

    glfwSetErrorCallback(error_callback);

    if ( ! glfwInit() )
    {
//        exit(EXIT_FAILURE);
        return -1;
    }



    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if ( ! window )
    {
        glfwTerminate();
//        exit(EXIT_FAILURE);
        return -1;
    }

    // More for "typing" style stuff
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress );
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    // How does it know about the functions?
    // These are from the AssimpFileLoaderHelper .lib
//    fnAssimpFileLoaderHelper("Yo!");

    // Because it's "exported" we can see it. 
//    cAssimpHelper myHelper;


    cShaderManager* pShaderManager = new cShaderManager();

    cShaderManager::cShader vertexShader;
    cShaderManager::cShader fragementShader;

    pShaderManager->setBasePath("assets/shaders/");

    vertexShader.fileName = "vertexShader01.glsl";
    fragementShader.fileName = "fragmentShader01.glsl";

   if ( !pShaderManager->createProgramFromFile( "basicShader",
                                                 vertexShader, fragementShader ) )
   {
       std::cout << "Error: Can't create shader program:" << std::endl;
       std::cout << pShaderManager->getLastError() << std::endl;
   }

//   glUseProgram(program);
    GLuint shaderProgram_ID = pShaderManager->getIDFromFriendlyName("basicShader");
    glUseProgram(shaderProgram_ID);

    // Load the models into the VAO Manager

    cVAOManager* pModelManger = new cVAOManager();

    std::vector<std::string> vecErrors;
    if ( ! LoadModels(pModelManger, shaderProgram_ID, ::g_vec_pMeshesToDraw, vecErrors, true ) )
    {
        std::cout << "There were errors loading some models:" << std::endl;
        for ( std::string errText : vecErrors )
        {
            std::cout << errText << std::endl;
        }
    }//if ( ! LoadModels(...

    // Load the textures
    cBasicTextureManager* pTheTextures = new cBasicTextureManager();

    // Load some textures
    pTheTextures->SetBasePath("assets/textures");

    if ( pTheTextures->Create2DTextureFromBMPFile("24taylor-notebook3-superJumbo.bmp", true) )
    {
        std::cout << "Loaded Tay Tay" << std::endl;
    }
    if ( pTheTextures->Create2DTextureFromBMPFile("parabellumcover.0.bmp", true) )
    {
        std::cout << "Loaded John Wick" << std::endl;
    }    
    if ( pTheTextures->Create2DTextureFromBMPFile("UV_checker_Map_byValle.bmp", true) )
    {
        std::cout << "Loaded UV_checker_Map_byValle" << std::endl;
    }    
    if ( pTheTextures->Create2DTextureFromBMPFile("DrakGrey.bmp", true) )
    {
        std::cout << "Loaded DrakGrey" << std::endl;
    }    

    if ( pTheTextures->Create2DTextureFromBMPFile("jeuusd992wd41.bmp", true) )
    {
        std::cout << "Loaded jeuusd992wd41 (duck)" << std::endl;
    }    

    pTheTextures->SetBasePath("assets/models/Simple_Space_Interiors_SourceFiles/Textures");
    if ( pTheTextures->Create2DTextureFromBMPFile("SpaceInteriors_Texture.bmp", true) )
    {
        std::cout << "Loaded SpaceInteriors_Texture.bmp" << std::endl;
    }    


    std::string errorString;
    pTheTextures->SetBasePath("assets/Textures/cubemap_textures");
    if ( pTheTextures->CreateCubeTextureFromBMPFiles("spaceSkybox",
                                                     "SpaceBox_right1_posX.bmp", "SpaceBox_left2_negX.bmp", 
                                                     "SpaceBox_top3_posY.bmp" , "SpaceBox_bottom4_negY.bmp",
                                                     "SpaceBox_front5_posZ.bmp", "SpaceBox_back6_negZ.bmp",
                                                     true, errorString ) )
    {
        std::cout << "Loaded the SpaceBox skybox OK" << std::endl;
    }
    else
    {
        std::cout << "Error loading the SpaceBox skybox: " << errorString << std::endl;
    }
                                                     
    
    ::g_pTheLights = new cLightManager();
    ::g_pTheLights->LoadUniformLocationsFromShader(shaderProgram_ID);

//    ::g_pTheLights->myLights[0].position = glm::vec4(-5.0f, 15.0f, 0.0f, 1.0f);
    ::g_pTheLights->myLights[0].position = glm::vec4(2.4f, 4.0f, 10.6f, 1.0f);
    ::g_pTheLights->myLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);    // light colour
    float shininess = 1.0f;
    ::g_pTheLights->myLights[0].specular = glm::vec4(1.0f, 1.0f, 1.0f, shininess);    // light colour

    // To change the "brightness" or the "throw" of the light
    ::g_pTheLights->myLights[0].atten.x = 1.0f;     // Constant;
    ::g_pTheLights->myLights[0].atten.y = 0.2f;     // Linear attenuation  
    ::g_pTheLights->myLights[0].atten.z = 0.01f;    // quadratic attenuation  

    //::g_pTheLights->myLights[0].direction 

    ::g_pTheLights->myLights[0].param1.x = 0.0f;    // 0 = Point light

    ::g_pTheLights->myLights[0].param2.x = 1.0f;    // 1 = on, 0 is off


    // Light #1 
    ::g_pTheLights->myLights[1].position = glm::vec4(0.0f, 50.0f, 25.0f, 1.0f);
    ::g_pTheLights->myLights[1].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);    // light colour
    ::g_pTheLights->myLights[1].specular = glm::vec4(1.0f, 1.0f, 1.0f, shininess);    // light colour

    // To change the "brightness" or the "throw" of the light
    ::g_pTheLights->myLights[1].atten.x = 1.0f;     // Constant;
    ::g_pTheLights->myLights[1].atten.y = 0.01f;     // Linear attenuation  
    ::g_pTheLights->myLights[1].atten.z = 0.001f;    // quadratic attenuation  

    //vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
    //            // 0 = pointlight
    //            // 1 = spot light
                // 2 = directional light
    ::g_pTheLights->myLights[1].param1.x = 1.0f;    // Spot light
    ::g_pTheLights->myLights[1].param1.y = 2.0f;    // 5 degrees
    ::g_pTheLights->myLights[1].param1.z = 4.0f;    // 10 degrees

    ::g_pTheLights->myLights[1].TurnOn();
 
    // Look at the spider with a spot light
    cMeshObject* pSpider = pFindObjectByFriendlyName("Mr. Spider");
    // Direction is normalized vector from light to target
    glm::vec3 rayToTarget = glm::vec3(::g_pTheLights->myLights[1].position) - pSpider->position;
    // Normlize makes the ray 1 unit long
    rayToTarget = glm::normalize(rayToTarget);  // Also can divide by length of ray

    // The direction is 4 floats because I used vec4 because the registers are all vec4s.
    ::g_pTheLights->myLights[1].direction = glm::vec4(rayToTarget, 1.0f);


    float heyHeyILoveMath = 0.0f;
    
   // float HACK_FAR_CLIPPING_PLANE = 1000.0f;

    // Turn on back face culling 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    float nearPlane = 1.0f;          // Near plane
    float farPlane = 10000.0f;      // Far plane


    // When this while exits, your program exits, too
    while (!glfwWindowShouldClose(window))
    {

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);

    //        mat4x4 m, p, mvp;
    //    glm::mat4 m, mvp;
        glm::mat4 mModel = glm::mat4(1.0f);
        glm::mat4 mProjection = glm::mat4(1.0f);
        glm::mat4 mView = glm::mat4(1.0f);

        mProjection = glm::perspective(0.6f,
                                         ratio,
                                         nearPlane, // 1.0f,          // Near plane
                                         farPlane); // 100000.0f);      // Far plane

        GLint matProjection_UL = glGetUniformLocation(shaderProgram_ID, "matProjection");
        glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, glm::value_ptr(mProjection));


        // Look at the spider
        cMeshObject* pSpider = pFindObjectByFriendlyName("Mr. Spider");
//        pSpider->position.z -= 0.01f;


        // Look at the spider with a spot light
        // Direction is normalized vector from light to target
        glm::vec3 rayToTarget = pSpider->position - glm::vec3(::g_pTheLights->myLights[1].position);
        // Normlize makes the ray 1 unit long
        rayToTarget = glm::normalize(rayToTarget);  // Also can divide by length of ray

        // The direction is 4 floats because I used vec4 because the registers are all vec4s.
        ::g_pTheLights->myLights[1].direction = glm::vec4(rayToTarget, 1.0f);


        // Turn on "alpha transparency"
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



//        ::g_cameraTarget = pSpider->position;

        mView = glm::lookAt(::g_cameraEye,
                            ::g_cameraTarget,  // newTarget
                            ::g_upVector);

        GLint matView_UL = glGetUniformLocation(shaderProgram_ID, "matView");
        glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(mView));



        // Update the camera 'eye' location in the shader
        // uniform vec3 eyeLocation;
        GLint eyeLocation_UniformLocation = glGetUniformLocation(shaderProgram_ID, "eyeLocation");
        glUniform3f(eyeLocation_UniformLocation,
                    ::g_cameraEye.x, ::g_cameraEye.y, ::g_cameraEye.z);

        // Adding clearing the "depth" buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Change the screen clear colour
        // 100, 149, 237 (cornflower blue)
     //   glClearColor(100.0f/255.0f, 149.0f/255.0f, 237.0f/255.0f, 1.0f);

        // Turn on "depth buffer" testing
        glEnable(GL_DEPTH_TEST);

        // Update all the light stuff
        ::g_pTheLights->UpdateLightInfoToShader(shaderProgram_ID);



        if ( ::g_bShowDebugLightSpheres )
        {
            DrawDebugLightSpheres(pModelManger, shaderProgram_ID);
        }


        // Draw skybox
        {
            cMeshObject* pSkyBox = pFindObjectByFriendlyName("Sky Sphere");

            glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

            pSkyBox->bIsVisible = true;
            pSkyBox->isWireframe = true;
            pSkyBox->diffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
            pSkyBox->bDontLight = true;

            // Proportional to the far plane
            pSkyBox->scale = farPlane * 0.5f;

            // Place this sphere where the camera is
            pSkyBox->position = ::g_cameraEye;

            DrawObject(pSkyBox, matModel, pModelManger, shaderProgram_ID);

            pSkyBox->bIsVisible = false;

        }




        //cMeshObject* pCharlotte = pFindObjectByFriendlyName("Mrs. Spider");

        //glm::vec3 oldColor = pSpider->diffuseColour;
        //glm::vec3 oldPosition = pSpider->position;

        //pCharlotte->diffuseColour = glm::vec3(0.0f, 1.0f, 1.0f);

        //for ( float x = -500.0f; x < 600.0f; x += 100.0f)
        //{
        //    pCharlotte->position = glm::vec3(x, 50.0f, 0.0f);

        //    DrawObject(pCharlotte, glm::mat4(1.0f), pModelManger, shaderProgram_ID);
        //}

        // *********************************************************
        // Draw all the stuff in the vector
        for ( cMeshObject* pCurrentMesh : ::g_vec_pMeshesToDraw )
        {

            SetUpTexturesForMesh(pCurrentMesh, shaderProgram_ID, pTheTextures);


            glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

            DrawObject(pCurrentMesh, matModel, pModelManger, shaderProgram_ID);

        }//for (std::vector< cMeshObject >
        // *********************************************************



        glfwSwapBuffers(window);
        glfwPollEvents();       // Checks keyboard, mouse, joystick, etc.

        // Handle asyn keyboard inputs
        handleKeyboardInput(window);

//        ChangeLightSphereVisibility(::g_bShowDebugLightSpheres);

        std::stringstream ssTitle;
        ssTitle << "Camera (x,y,z): "
            << ::g_cameraEye.x << ", "
            << ::g_cameraEye.y << ", "
            << ::g_cameraEye.z
            << " Light[" << g_SelectedLightIndex << "] "
            << "(xyz: " << ::g_pTheLights->myLights[0].position.x
            << ", " << ::g_pTheLights->myLights[0].position.y
            << ", " << ::g_pTheLights->myLights[0].position.z << ") "
            << " l_atten: " << ::g_pTheLights->myLights[0].atten.y
            << " q_atten: " << ::g_pTheLights->myLights[0].atten.z;


        glfwSetWindowTitle( window, ssTitle.str().c_str() );

    }

    // STARTOF: Clean up...
    delete ::g_pTheLights;
    delete pModelManger;

    for ( cMeshObject* pCurrentMesh : ::g_vec_pMeshesToDraw )
    {
        delete pCurrentMesh;
    }
    ::g_vec_pMeshesToDraw.clear();
    // ENDOF: Clean up

    glfwDestroyWindow(window);

    glfwTerminate();
//    exit(EXIT_SUCCESS);
    return 0;
}

// Returns NULL (or 0) if not found
// Up to about 30-40 items, the linear search will be faster because of cache issues
cMeshObject* pFindObjectByFriendlyName(std::string theName)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshesToDraw.size(); index++)
    {
        if (::g_vec_pMeshesToDraw[index]->friendlyName == theName)
        {
            // Found it!
            cMeshObject* pFoundMesh = ::g_vec_pMeshesToDraw[index];
            return pFoundMesh;
        }
    }
    // didn't find it
    return NULL;
}

//void ChangeLightSphereVisibility(bool bIsOn)
//{
//    cMeshObject* plightCentre = pFindObjectByFriendlyName("lightCentre");
//    plightCentre->bIsVisible = bIsOn;
//
//    cMeshObject* p90Percent = pFindObjectByFriendlyName("90Percent");
//    p90Percent->bIsVisible = bIsOn;
//
//    cMeshObject* p50Percent = pFindObjectByFriendlyName("50Percent");
//    p50Percent->bIsVisible = bIsOn;
//
//    cMeshObject* p25Percent = pFindObjectByFriendlyName("25Percent");
//    p25Percent->bIsVisible = bIsOn;
//
//    cMeshObject* p5Percent = pFindObjectByFriendlyName("5Percent");
//    p5Percent->bIsVisible = bIsOn;
//
//    return;
//}


void DrawDebugLightSpheres(cVAOManager* pModelManager, GLuint shaderProgram_ID)
{

    cMeshObject* pDebugSphere = pFindObjectByFriendlyName("DebugSphere");
    if ( ! pDebugSphere)
    {
        // Didn't find the Debug Sphere model
        return;
    }


    // Save whatever state the debug sphere is currently in
    cMeshObject::sMeshState oldState;
    oldState.SaveState(pDebugSphere);

    pDebugSphere->position = glm::vec3(::g_pTheLights->myLights[::g_SelectedLightIndex].position);
    pDebugSphere->bDontLight = true;
    pDebugSphere->isWireframe = true;
    pDebugSphere->bIsVisible = true;

    const float LIGHT_SPHERES_TRANSPARENCY = 0.4f;
    pDebugSphere->alphaTransparency = LIGHT_SPHERES_TRANSPARENCY; 


    // Centre of light:
    pDebugSphere->diffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
    pDebugSphere->scale = 0.1f;
    DrawObject(pDebugSphere, glm::mat4(1.0f), pModelManager, shaderProgram_ID);


    // To calculate the size of the debug spheres based on attenuation
    cLightHelper myLightHelper;


    // 90% of full brightness...
    float distance90percent = myLightHelper.calcApproxDistFromAtten(0.9f,        // Target 90% brightness
                                                            0.001f,      // Within 0.001 of that value
                                                            100000.0f,    // Give up if I get this far away
                                                            ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
                                                            ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
                                                            ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
    pDebugSphere->diffuseColour = glm::vec3(0.0f, 1.0f, 1.0f);
    pDebugSphere->scale = distance90percent;
    DrawObject(pDebugSphere, glm::mat4(1.0f), pModelManager, shaderProgram_ID);



    // 50% of full brightness...
    float distance50percent = myLightHelper.calcApproxDistFromAtten(0.5f,        // Target 90% brightness
                                                                    0.001f,      // Within 0.001 of that value
                                                                    100000.0f,    // Give up if I get this far away
                                                                    ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
                                                                    ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
                                                                    ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
    pDebugSphere->diffuseColour = glm::vec3(1.0f, 1.0f, 0.0f);  // Yellow
    pDebugSphere->scale = distance50percent;
    DrawObject(pDebugSphere, glm::mat4(1.0f), pModelManager, shaderProgram_ID);


    // 25% of full brightness...
    float distance25percent = myLightHelper.calcApproxDistFromAtten(0.25f,        // Target 90% brightness
                                                                    0.001f,      // Within 0.001 of that value
                                                                    100000.0f,    // Give up if I get this far away
                                                                    ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
                                                                    ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
                                                                    ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
    pDebugSphere->diffuseColour = glm::vec3(0.0f, 1.0f, 0.0f);  // Light blue or Cyan
    pDebugSphere->scale = distance25percent;
    DrawObject(pDebugSphere, glm::mat4(1.0f), pModelManager, shaderProgram_ID);


    // 5% of full brightness...
    float distance05percent = myLightHelper.calcApproxDistFromAtten(0.05f,        // Target 90% brightness
                                                                    0.001f,      // Within 0.001 of that value
                                                                    100000.0f,    // Give up if I get this far away
                                                                    ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
                                                                    ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
                                                                    ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
    pDebugSphere->diffuseColour = glm::vec3(0.0f, 0.0f, 2.0f);  // Blue
    pDebugSphere->scale = distance05percent;
    DrawObject(pDebugSphere, glm::mat4(1.0f), pModelManager, shaderProgram_ID);


    // Restore the old debug sphere state
    oldState.RestoreState(pDebugSphere);




    return;
}// END OF: Place the sphere where the light #0 was90Percent


void SetUpTexturesForMesh(cMeshObject* pCurrentMesh, GLuint shaderProgram_ID, cBasicTextureManager* pTheTextures)
{
    // ***********************************************************
    // We love this, don't we? 

    {// Texture #0
        GLint textureNumber = 0;

        glActiveTexture(GL_TEXTURE0 + textureNumber);	// GL_TEXTURE0 = 33984

        // Look up the texture
        GLuint meshTexture00 = pTheTextures->getTextureIDFromName(pCurrentMesh->textureName[0]);

        glBindTexture(GL_TEXTURE_2D, meshTexture00);

        // uniform sampler2D texture01;
        GLint texture00_UniformLocation = glGetUniformLocation(shaderProgram_ID, "texture00");
        glUniform1i(texture00_UniformLocation, textureNumber);       // 0x84C6   33990
    }//Texture #0


    {// Texture #1
        GLint textureNumber = 1;

        glActiveTexture(GL_TEXTURE0 + textureNumber);	// GL_TEXTURE0 = 33984

        // Look up the texture
        GLuint meshTexture01 = pTheTextures->getTextureIDFromName(pCurrentMesh->textureName[1]);
        glBindTexture(GL_TEXTURE_2D, meshTexture01);

        // uniform sampler2D texture01;
        GLint texture01_UniformLocation = glGetUniformLocation(shaderProgram_ID, "texture01");
        glUniform1i(texture01_UniformLocation, textureNumber);       // 0x84C6   33990
    }//Texture #0

    {// Texture #2
        GLint textureNumber = 2;

        glActiveTexture(GL_TEXTURE0 + textureNumber);	// GL_TEXTURE0 = 33984

        // Look up the texture
        GLuint meshTexture02 = pTheTextures->getTextureIDFromName(pCurrentMesh->textureName[2]);
        glBindTexture(GL_TEXTURE_2D, meshTexture02);

        // uniform sampler2D texture01;
        GLint texture02_UniformLocation = glGetUniformLocation(shaderProgram_ID, "texture02");
        glUniform1i(texture02_UniformLocation, textureNumber);       // 0x84C6   33990
    }//Texture #0




    // And set the mixing values as well
    //uniform vec4 textureMix_00_to_03;
    GLint textureMix_00_to_03_UniformLocation = glGetUniformLocation(shaderProgram_ID, "textureMix_00_to_03");
    // 
    glUniform4f(textureMix_00_to_03_UniformLocation,
                pCurrentMesh->textureMixingRatio[0],
                pCurrentMesh->textureMixingRatio[1],
                pCurrentMesh->textureMixingRatio[2],
                pCurrentMesh->textureMixingRatio[3]);


    return;
}
 
// ***********************************************************