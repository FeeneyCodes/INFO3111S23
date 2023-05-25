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

#include "cMeshObject.h"

#include "cLightHelper.h"

//#include "../AssimpFileLoaderHelper/AssimpFileLoaderHelper.h"

//static const struct
//{
//    float x, y;
//    float r, g, b;
//} vertices[3] =
//{
//    { -0.6f, -0.4f, 1.f, 0.f, 0.f },            // Spawn a vertex shader instance
//    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
//    {   0.f,  0.6f, 0.f, 0.f, 1.f }
//};

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


glm::vec3 g_cameraEye = glm::vec3(0.0, 0.0, -4.0f);
glm::vec3 g_cameraTarget = glm::vec3(0.0f, 25.0f, 0.0f);
glm::vec3 g_upVector = glm::vec3(0.0f, +1.0f, 0.0f);

void ChangeLightSphereVisibility(bool bIsOn);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// This is a list of the objects we want to draw in this scene
std::vector< cMeshObject > g_vecMeshesToDraw;
unsigned int g_selectedMeshIndex = 2;


cLightManager* g_pTheLights = NULL;
bool g_bShowDebugLightSpheres = true;


// Note I'm passing the array by reference so that the pointer
//  value I create inside the function can be "passed back out".
//property float x
//property float y
//property float z
//property uchar red
//property uchar green
//property uchar blue
//property uchar alpha
bool Load_mig29_xyz_rgba_PlyFile(std::string filename,
                                 sVertexXYZ_RGB* &pVertexArray,
                                 unsigned int &numVerticesLoaded,
                                 unsigned int &numTrianglesLoaded);
//property float x
//property float y
//property float z
//property float nx
//property float ny
//property float nz
//property float s
//property float t
bool Load_Mushrooms1_PlyFile(std::string filename,
                                 sVertexXYZ_RGB* &pVertexArray,
                                 unsigned int &numVerticesLoaded,
                                 unsigned int &numTrianglesLoaded);

//property float x
//property float y
//property float z
//property float nx
//property float ny
//property float nz
bool Load_Doom_spider_mastermind_PlyFile(std::string filename,
                                 sVertexXYZ_RGB* &pVertexArray,
                                 unsigned int &numVerticesLoaded,
                                 unsigned int &numTrianglesLoaded);


// Returns NULL (or 0) if not found
// Up to about 30-40 items, the linear search will be faster because of cache issues
cMeshObject* pFindObjectByFriendlyName(std::string theName)
{
    for (unsigned int index = 0; index != ::g_vecMeshesToDraw.size(); index++ )
    {
        if ( ::g_vecMeshesToDraw[index].friendlyName == theName )
        {
            // Found it!
            return &(::g_vecMeshesToDraw[index]);
        }
    }
    // didn't find it
    return NULL;
}


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
    GLuint shaderProgram = pShaderManager->getIDFromFriendlyName("basicShader");
    glUseProgram(shaderProgram);

    // Load the models into the VAO Manager

    cVAOManager* pModelManger = new cVAOManager();

    sModelDrawInfo modelILoadedInfo;
    if ( pModelManger->LoadModelIntoVAO("assets/models/spider_mastermind.bmd6model.fbx.ascii_Y_up.ply",
                                        modelILoadedInfo, shaderProgram))
    {
        std::cout << "spider_mastermind loaded OK" << std::endl;
    }

    std::cout << "Loaded " << modelILoadedInfo.numberOfTriangles << " triangles" << std::endl;
    if ( pModelManger->LoadModelIntoVAO("assets/models/Mushrooms1_xyz_normal.ply",
                                        modelILoadedInfo, shaderProgram))
    {
        std::cout << "Mushrooms1_xyz_normal loaded OK" << std::endl;
    }

    std::cout << "Loaded " << modelILoadedInfo.numberOfTriangles << " triangles" << std::endl;

    pModelManger->LoadModelIntoVAO("assets/models/bun_zipper_xyz_n.ply", modelILoadedInfo, shaderProgram);
    std::cout << "Loaded " << modelILoadedInfo.numberOfTriangles << " triangles" << std::endl;

    pModelManger->LoadModelIntoVAO("assets/models/mig29_xyz_n.ply", modelILoadedInfo, shaderProgram);
    std::cout << "Loaded " << modelILoadedInfo.numberOfTriangles << " triangles" << std::endl;

    pModelManger->LoadModelIntoVAO("assets/models/camion jugete_xyz_n.ply", modelILoadedInfo, shaderProgram);
    std::cout << "Loaded " << modelILoadedInfo.numberOfTriangles << " triangles" << std::endl;

    pModelManger->LoadModelIntoVAO("assets/models/FractalTerrainFromMeshLab_xyz_n.ply", modelILoadedInfo, shaderProgram);
    std::cout << "Loaded " << modelILoadedInfo.numberOfTriangles << " triangles" << std::endl;

//    pModelManger->LoadModelIntoVAO("assets/models/Apartment Building_26_xyz_n.ply", modelILoadedInfo, shaderProgram);
    pModelManger->LoadModelIntoVAO("assets/models/Apartment Building_26_xyz_n (flat shaded export).ply", modelILoadedInfo, shaderProgram);
    std::cout << "Loaded " << modelILoadedInfo.numberOfTriangles << " triangles" << std::endl;

    pModelManger->LoadModelIntoVAO("assets/models/Smooth_UV_Sphere_xyz_n.ply", modelILoadedInfo, shaderProgram);
    std::cout << "Loaded " << modelILoadedInfo.numberOfTriangles << " triangles" << std::endl;

    cMeshObject smallSphereLightCentre;
    smallSphereLightCentre.meshName = "assets/models/Smooth_UV_Sphere_xyz_n.ply";
    smallSphereLightCentre.colour = glm::vec3(1.0f, 1.0f, 1.0f);
    smallSphereLightCentre.isWireframe = true;
    smallSphereLightCentre.scale = 0.25f;
    smallSphereLightCentre.friendlyName = "lightCentre";
    smallSphereLightCentre.bDontLight = true;
    ::g_vecMeshesToDraw.push_back(smallSphereLightCentre);

    cMeshObject smallSphere90PercentBrightness;
    smallSphere90PercentBrightness.meshName = "assets/models/Smooth_UV_Sphere_xyz_n.ply";
    smallSphere90PercentBrightness.colour = glm::vec3(0.5f, 0.5f, 0.0f);
    smallSphere90PercentBrightness.isWireframe = true;
    smallSphere90PercentBrightness.scale = 2.0f;
    smallSphere90PercentBrightness.friendlyName = "90Percent";
    smallSphere90PercentBrightness.bDontLight = true;
    ::g_vecMeshesToDraw.push_back(smallSphere90PercentBrightness);

    cMeshObject smallSphere50PercentBrightness;
    smallSphere50PercentBrightness.meshName = "assets/models/Smooth_UV_Sphere_xyz_n.ply";
    smallSphere50PercentBrightness.colour = glm::vec3(0.0f, 0.5f, 0.5f);
    smallSphere50PercentBrightness.isWireframe = true;
    smallSphere50PercentBrightness.scale = 3.0f;
    smallSphere50PercentBrightness.friendlyName = "50Percent";
    smallSphere50PercentBrightness.bDontLight = true;
    ::g_vecMeshesToDraw.push_back(smallSphere50PercentBrightness);

    cMeshObject smallSphere25PercentBrightness;
    smallSphere25PercentBrightness.meshName = "assets/models/Smooth_UV_Sphere_xyz_n.ply";
    smallSphere25PercentBrightness.colour = glm::vec3(0.0f, 0.5f, 0.0f);
    smallSphere25PercentBrightness.isWireframe = true;
    smallSphere25PercentBrightness.scale = 4.0f;
    smallSphere25PercentBrightness.friendlyName = "25Percent";
    smallSphere25PercentBrightness.bDontLight = true;
    ::g_vecMeshesToDraw.push_back(smallSphere25PercentBrightness);

    cMeshObject smallSphere05PercentBrightness;
    smallSphere05PercentBrightness.meshName = "assets/models/Smooth_UV_Sphere_xyz_n.ply";
    smallSphere05PercentBrightness.colour = glm::vec3(0.5f, 0.0f, 0.5f);
    smallSphere05PercentBrightness.isWireframe = true;
    smallSphere05PercentBrightness.scale = 5.0f;
    smallSphere05PercentBrightness.friendlyName = "5Percent";
    smallSphere05PercentBrightness.bDontLight = true;
    ::g_vecMeshesToDraw.push_back(smallSphere05PercentBrightness);

    //std::vector< cMeshObject > g_vecMeshesToDraw;
//    appartmentBuildingMesh.colour = glm::vec3(0.2f, 0.4f, 0.3f);

   cMeshObject appartmentBuildingMesh;
//.    appartmentBuildingMesh.meshName = "assets/models/Apartment Building_26_xyz_n.ply";
    appartmentBuildingMesh.meshName = "assets/models/Apartment Building_26_xyz_n (flat shaded export).ply";
    appartmentBuildingMesh.colour = glm::vec3(0.8f, 0.8f, 0.8f);
//    appartmentBuildingMesh.isWireframe = true;
    appartmentBuildingMesh.scale = 1.0f;
    appartmentBuildingMesh.position = glm::vec3(15.0f, 0.0f, 0.0f);
    ::g_vecMeshesToDraw.push_back(appartmentBuildingMesh);
    //std::vector< cMeshObject > g_vecMeshesToDraw;
//    appartmentBuildingMesh.colour = glm::vec3(0.2f, 0.4f, 0.3f);


    cMeshObject terrainMesh;
    terrainMesh.meshName = "assets/models/FractalTerrainFromMeshLab_xyz_n.ply";
    terrainMesh.colour = glm::vec3(0.8f, 0.8f, 0.8f);
//    terrainMesh.isWireframe = true;
    terrainMesh.position = glm::vec3(0.0f, -210.0f, 0.0f);
    ::g_vecMeshesToDraw.push_back(terrainMesh);

    cMeshObject SpiderMesh;
    SpiderMesh.meshName = "assets/models/spider_mastermind.bmd6model.fbx.ascii_Y_up.ply";
    SpiderMesh.friendlyName = "Mr. Spider";
    SpiderMesh.colour = glm::vec3(1.0f, 0.0f, 0.0f);
//    SpiderMesh.isWireframe = false;
    SpiderMesh.position = glm::vec3(-25.0f, 0.0f, 0.0f);
    ::g_vecMeshesToDraw.push_back(SpiderMesh);

    cMeshObject airplane1;
    airplane1.meshName = "assets/models/mig29_xyz_n.ply";
    airplane1.colour = glm::vec3(0.0f, 1.0f, 0.0f);
    airplane1.position = glm::vec3(0.0f, 10.0f, 0.0f);
    airplane1.scale = 25.0f;
    ::g_vecMeshesToDraw.push_back(airplane1);

    cMeshObject mushRoomMesh;
    mushRoomMesh.meshName = "assets/models/Mushrooms1_xyz_normal.ply";
    mushRoomMesh.colour = glm::vec3(0.0f, 0.0f, 1.0f);
    mushRoomMesh.position = glm::vec3(0.0f, 0.0f, 0.0f);
    mushRoomMesh.scale = 0.1f;
    mushRoomMesh.orientation.x = -90.0f;         // Note it's being converted into radians
    ::g_vecMeshesToDraw.push_back(mushRoomMesh);

    cMeshObject toyTruck;
    toyTruck.meshName = "assets/models/camion jugete_xyz_n.ply";
    toyTruck.colour = glm::vec3(0.0f, 1.0f, 1.0f);
    toyTruck.position.x = 4.0f;
    toyTruck.scale = 5.0f;
    toyTruck.orientation.y = glm::radians(90.0f);
    toyTruck.position = glm::vec3(-8.0f, -1.0f, -5.0f);
    ::g_vecMeshesToDraw.push_back(toyTruck);

    cMeshObject toyTruck2;
    toyTruck2.meshName = "assets/models/camion jugete_xyz_n.ply";
    toyTruck2.colour = glm::vec3(1.0f, 1.0f, 0.0f);
    toyTruck2.position.x = -4.0f;
    toyTruck2.scale = 8.0f;
    toyTruck2.orientation.x = glm::radians(135.0f);
    ::g_vecMeshesToDraw.push_back(toyTruck2);

//    mvp_location = glGetUniformLocation(shaderProgram, "MVP");


    ::g_pTheLights = new cLightManager();
    ::g_pTheLights->LoadUniformLocationsFromShader(shaderProgram);

    ::g_pTheLights->myLights[0].position = glm::vec4(-5.0f, 15.0f, 0.0f, 1.0f);
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
                                         0.1f,          // Near plane
                                         1000.0f);      // Far plane

        mView = glm::mat4(1.0f);

        //glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
        //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        //glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

//        glm::vec3 newTarget = ::g_cameraEye + glm::vec3(0.0f, 0.0f, 10.0f);

        // Look at the spider
        cMeshObject* pSpider = pFindObjectByFriendlyName("Mr. Spider");
        pSpider->position.z -= 0.01f;


        // Look at the spider with a spot light
        // Direction is normalized vector from light to target
        glm::vec3 rayToTarget = pSpider->position - glm::vec3(::g_pTheLights->myLights[1].position);
        // Normlize makes the ray 1 unit long
        rayToTarget = glm::normalize(rayToTarget);  // Also can divide by length of ray

        // The direction is 4 floats because I used vec4 because the registers are all vec4s.
        ::g_pTheLights->myLights[1].direction = glm::vec4(rayToTarget, 1.0f);





        ::g_cameraTarget = pSpider->position;

        mView = glm::lookAt(::g_cameraEye,
                            ::g_cameraTarget,  // newTarget
                            ::g_upVector);

        // Adding clearing the "depth" buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Change the screen clear colour
        // 100, 149, 237 (cornflower blue)
        glClearColor(100.0f/255.0f, 149.0f/255.0f, 237.0f/255.0f, 1.0f);

        // Turn on "depth buffer" testing
        glEnable(GL_DEPTH_TEST);

        // Update all the light stuff
        ::g_pTheLights->UpdateLightInfoToShader(shaderProgram);

        if ( g_bShowDebugLightSpheres )
        {

        }

        // Place the sphere where the light #0 was90Percent
        {
            cLightHelper myLightHelper;

            cMeshObject* plightCentre = pFindObjectByFriendlyName("lightCentre");
            if ( plightCentre)
            {
                plightCentre->position = glm::vec3(::g_pTheLights->myLights[::g_SelectedLightIndex].position);
            }

            cMeshObject* p90Percent = pFindObjectByFriendlyName("90Percent");
            if ( p90Percent)
            {
                p90Percent->position = glm::vec3(::g_pTheLights->myLights[::g_SelectedLightIndex].position);
                // 
                float distance = myLightHelper.calcApproxDistFromAtten(0.9f,        // Target 90% brightness
                                                      0.001f,      // Within 0.001 of that value
                                                      100000.0f,    // Give up if I get this far away
                                                      ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
                                                      ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
                                                      ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
                p90Percent->scale = distance;
            }

            cMeshObject* p50Percent = pFindObjectByFriendlyName("50Percent");
            if (p50Percent)
            {
                p50Percent->position = glm::vec3(::g_pTheLights->myLights[::g_SelectedLightIndex].position);
                // 
                float distance = myLightHelper.calcApproxDistFromAtten(0.5f,        // Target 50% brightness
                                                                       0.001f,      // Within 0.001 of that value
                                                                       100000.0f,    // Give up if I get this far away
                                                                       ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
                                                                       ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
                                                                       ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
                p50Percent->scale = distance;
            }

            cMeshObject* p25Percent = pFindObjectByFriendlyName("25Percent");
            if (p25Percent)
            {
                p25Percent->position = glm::vec3(::g_pTheLights->myLights[::g_SelectedLightIndex].position);

                float distance = myLightHelper.calcApproxDistFromAtten(0.25f,        // Target 50% brightness
                                                                       0.001f,      // Within 0.001 of that value
                                                                       100000.0f,    // Give up if I get this far away
                                                                       ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
                                                                       ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
                                                                       ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
                p25Percent->scale = distance;
            }

            cMeshObject* p5Percent = pFindObjectByFriendlyName("5Percent");
            if (p5Percent)
            {
                p5Percent->position = glm::vec3(::g_pTheLights->myLights[::g_SelectedLightIndex].position);

                float distance = myLightHelper.calcApproxDistFromAtten(0.05f,        // Target 50% brightness
                                                                       0.001f,      // Within 0.001 of that value
                                                                       100000.0f,    // Give up if I get this far away
                                                                       ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.x,
                                                                       ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.y,
                                                                       ::g_pTheLights->myLights[::g_SelectedLightIndex].atten.z);
                p5Percent->scale = distance;
            }

        }// END OF: Place the sphere where the light #0 was90Percent

        // Draw all the stuff in the vector
        for (std::vector< cMeshObject >::iterator itMesh = ::g_vecMeshesToDraw.begin();
             itMesh != ::g_vecMeshesToDraw.end(); itMesh++)
        {
            // Copy the mesh (for ease of reading)
            cMeshObject currentMesh = *itMesh;


            if ( ! currentMesh.bIsVisible )
            {
                // Skip rest of rendering
                continue;
            }

    //        mat4x4_identity(m);
            mModel = glm::mat4(1.0f);        // Identity matrix

//            glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
//                                                    glm::vec3(heyHeyILoveMath, 0.0, 0.0f) );
            glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                                                    currentMesh.position );


            glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
                                               glm::radians( currentMesh.orientation. x),
                                               glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
                                               glm::radians(currentMesh.orientation.y),
                                               glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
                                               glm::radians(currentMesh.orientation.z),
                                               glm::vec3(0.0f, 0.0f, 1.0f));

            glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                                            glm::vec3(currentMesh.scale, currentMesh.scale, currentMesh.scale));

            mModel = matTranslate * mModel; // Math order is the last thing

            mModel = mModel * matRotateX;
            mModel = mModel * matRotateY;
            mModel = mModel * matRotateZ;

            mModel = mModel * matScale; // Math order is 1st thing

    //        mat4x4_rotate_Z(m, m, (float)glfwGetTime());
    //        glm::mat4 mRotateZ = glm::rotate(glm::mat4(1.0f),
    //                                         (float) glfwGetTime(),
    //                                         glm::vec3(0.0f, 0.0f, 1.0f));
    ////        mat4x4_mul(mvp, p, m);
    //        mModel = mRotateZ * mModel;


    //        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);


//            mvp = p * v * m;
//            uniform mat4x4 matModel;			// M
//            uniform mat4x4 matView;				// V
//            uniform mat4x4 matProjection;		// P
            GLint matModel_UL = glGetUniformLocation(shaderProgram, "matModel");
            GLint matView_UL = glGetUniformLocation(shaderProgram, "matView");
            GLint matProjection_UL = glGetUniformLocation(shaderProgram, "matProjection");
            glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(mModel));
            glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(mView));
            glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, glm::value_ptr(mProjection));


    //        glUseProgram(program);
    //        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
//            mvp_location = glGetUniformLocation(shaderProgram, "MVP");
//            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));


            // uniform vec3 colorOverrideRGB;
            GLint colorOverrideRGB_UL = glGetUniformLocation(shaderProgram, "colorOverrideRGB");
//            glUniform3f(colorOverrideRGB_UL, 1.0f, 0.0f, 0.0f);
            glUniform3f(colorOverrideRGB_UL, 
                        currentMesh.colour.r, 
                        currentMesh.colour.g,
                        currentMesh.colour.b);

            // Is this a "debug object" (i.e. not lit)
            //uniform bool bDontLight;
            GLint bDontLight_UL = glGetUniformLocation(shaderProgram, "bDontLight");
            if ( currentMesh.bDontLight )
            {
                glUniform1f(bDontLight_UL, (float)GL_TRUE);
            }
            else
            {
                glUniform1f(bDontLight_UL, (float)GL_FALSE);
            }



            //uniform vec3 positionOffset;
            GLint positionOffset_UL = glGetUniformLocation(shaderProgram, "positionOffset");
            glUniform3f(positionOffset_UL,
                        currentMesh.position.x,     // 4
                        currentMesh.position.y,     // 0 
                        currentMesh.position.z);    // 0

            //uniform float scale;
            GLint scale_UL = glGetUniformLocation(shaderProgram, "scale");
            glUniform1f(scale_UL,
                        currentMesh.scale);


            // GL_POINT, GL_LINE, and GL_FILL
            if ( currentMesh.isWireframe )
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            // glDrawArrays(GL_TRIANGLES, 0, 3);
    //        unsigned int numberOfVerticesToDraw = numTrianglesLoaded * 3;
    //        glDrawArrays(GL_TRIANGLES, 0, numberOfVerticesToDraw);

           // "assets/models/SpaceShuttleOrbiter.ply"
            sModelDrawInfo modelToDraw;
            if ( pModelManger->FindDrawInfoByModelName(currentMesh.meshName, modelToDraw) )
            {
                glBindVertexArray(modelToDraw.VAO_ID);

                glDrawElements(GL_TRIANGLES,
                               modelToDraw.numberOfIndices,
                               GL_UNSIGNED_INT,
                               (void*)0);

                // Bind it to nothing
                glBindVertexArray(0);

            }

        }//for (std::vector< cMeshObject >


        glfwSwapBuffers(window);
        glfwPollEvents();       // Checks keyboard, mouse, joystick, etc.

        // Handle asyn keyboard inputs
        handleKeyboardInput(window);

        ChangeLightSphereVisibility(::g_bShowDebugLightSpheres);

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

    delete ::g_pTheLights;

    glfwDestroyWindow(window);

    glfwTerminate();
//    exit(EXIT_SUCCESS);
    return 0;
}


bool Load_mig29_xyz_rgba_PlyFile(std::string filename,
                                 sVertexXYZ_RGB* &pVertexArray,
                                 unsigned int& numVerticesLoaded,
                                 unsigned int& numTrianglesLoaded)
{
    std::ifstream fileToLoad( filename.c_str() );

    if ( fileToLoad.is_open() == false )
    {   
        // Oh no!
        return false;
    }

    std::string tempToken;
    while ( fileToLoad >> tempToken )
    {
        if ( tempToken == "vertex" )
        {
            break;  // Get out of the while
        }
    }

    // At this point, we've read the word "vertex".
    fileToLoad >> numVerticesLoaded;

    
    while ( fileToLoad >> tempToken )
    {
        if ( tempToken == "face" )
        {
            break;  // Get out of the while
        }
    }
    // At this point, we've read the word "face".
    fileToLoad >> numTrianglesLoaded;


    while ( fileToLoad >> tempToken )
    {
        if ( tempToken == "end_header" )
        {
            break;  // Get out of the while
        }
    }

    // At this point, we've read the word "end_header".

    // Now, we can read the vertices from the file...

    struct sTempPlyFileVertex
    {
        float x;
        float y;
        float z;
        int red;      // aka byte (0-255)  a 'char' is -128 to 127
        int green;
        int blue;
        int alpha;
    };

    std::vector<sTempPlyFileVertex> vecVertsFromFile;

    for ( unsigned int count = 0; count != numVerticesLoaded; count++ )
    {
        // -0.113944 0.168176 -0.404122 255 255 0 255 
        sTempPlyFileVertex tempVertexLoaded;

        fileToLoad >> tempVertexLoaded.x;
        fileToLoad >> tempVertexLoaded.y;
        fileToLoad >> tempVertexLoaded.z;
        fileToLoad >> tempVertexLoaded.red;
        fileToLoad >> tempVertexLoaded.green;
        fileToLoad >> tempVertexLoaded.blue;
        fileToLoad >> tempVertexLoaded.alpha;

//        tempVertexLoaded.x *= 10.0f;
//        tempVertexLoaded.y *= 10.0f;
//        tempVertexLoaded.z *= 10.0f;

        vecVertsFromFile.push_back(tempVertexLoaded);
    }

    // Now load the elements (aka triangles)
    struct sTempPlyFileTriangle
    {
        unsigned int vertex_0;
        unsigned int vertex_1;
        unsigned int vertex_2;
    };

    std::vector<sTempPlyFileTriangle> vecTrianglesFromFile;

    for (unsigned int count = 0; count != numTrianglesLoaded; count++)
    {
        // -0.113944 0.168176 -0.404122 255 255 0 255 
        sTempPlyFileTriangle tempTriangleLoaded;

        //  3 5 14 21
        unsigned int discardThis = 0;
        fileToLoad >> discardThis;      // Throw out the '3'

        fileToLoad >> tempTriangleLoaded.vertex_0;
        fileToLoad >> tempTriangleLoaded.vertex_1;
        fileToLoad >> tempTriangleLoaded.vertex_2;

        vecTrianglesFromFile.push_back(tempTriangleLoaded);
    }

    // Now I have to "translate" or convert this laoded data 
    //  into a fomat that the OpenGL code we already have 
    //  can draw to the screen.

    // Google C++ dynamic array

    unsigned int totalNumberOfVerticesToDraw = numTrianglesLoaded * 3;

    // Boom goes the dynomite!
    pVertexArray = new sVertexXYZ_RGB[totalNumberOfVerticesToDraw];

// DO NOT USE malloc if you are using "new". And why on Earth would you ever use malloc in C++??
//    pVertexArray = (sVertexXYZ_RGB*)malloc(totalNumberOfVerticesToDraw * sizeof(sVertexXYZ_RGB));

    // Note: this array will be filled with garbage values.
    // If this is very offensive to you, you can clear it with a loop, or memset
//    memset(pVertexArray, 0, totalNumberOfVerticesToDraw * sizeof(sVertexXYZ_RGB));
//    for ( unsigned int index = 0; index != 0; index++ )
//    {
//        pVertexArray[index].x = 0.0f; 
//        pVertexArray[index].y = 0.0f;
//        // ... and so on
//    }


    // Copy (or sort of "unroll" the PLY file into a 1D vertex list that we would like

    unsigned int finalVertexArrayIndex = 0;

    for ( unsigned int triangleIndex = 0; triangleIndex != vecTrianglesFromFile.size(); triangleIndex++ )
    {
        // Copy vertex 0...
        unsigned int vertexIndex_0 = vecTrianglesFromFile[triangleIndex].vertex_0;

        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_0].x;
        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_0].y;
        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_0].z;
        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
        pVertexArray[finalVertexArrayIndex].r = static_cast<float>( vecVertsFromFile[vertexIndex_0].red / 255.0f);
        pVertexArray[finalVertexArrayIndex].g = static_cast<float>( vecVertsFromFile[vertexIndex_0].green / 255.0f);
        pVertexArray[finalVertexArrayIndex].b = static_cast<float>( vecVertsFromFile[vertexIndex_0].blue / 255.0f);

        finalVertexArrayIndex++;

        // Copy vertex 1...
        unsigned int vertexIndex_1 = vecTrianglesFromFile[triangleIndex].vertex_1;

        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_1].x;
        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_1].y;
        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_1].z;
        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
        pVertexArray[finalVertexArrayIndex].r = static_cast<float>( vecVertsFromFile[vertexIndex_1].red / 255.0f);
        pVertexArray[finalVertexArrayIndex].g = static_cast<float>( vecVertsFromFile[vertexIndex_1].green / 255.0f);
        pVertexArray[finalVertexArrayIndex].b = static_cast<float>( vecVertsFromFile[vertexIndex_1].blue / 255.0f);

        finalVertexArrayIndex++;

        // Copy vertex 2...
        unsigned int vertexIndex_2 = vecTrianglesFromFile[triangleIndex].vertex_1;

        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_2].x;
        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_2].y;
        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_2].z;
        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
        pVertexArray[finalVertexArrayIndex].r = static_cast<float>( vecVertsFromFile[vertexIndex_2].red / 255.0f);
        pVertexArray[finalVertexArrayIndex].g = static_cast<float>( vecVertsFromFile[vertexIndex_2].green / 255.0f);
        pVertexArray[finalVertexArrayIndex].b = static_cast<float>( vecVertsFromFile[vertexIndex_2].blue / 255.0f);

        finalVertexArrayIndex++;
    }

    fileToLoad.close();

    return true;
}


bool Load_Mushrooms1_PlyFile(std::string filename,
                             sVertexXYZ_RGB*& pVertexArray,
                             unsigned int& numVerticesLoaded,
                             unsigned int& numTrianglesLoaded)
{
    std::ifstream fileToLoad(filename.c_str());

    if (fileToLoad.is_open() == false)
    {
        // Oh no!
        return false;
    }

    std::string tempToken;
    while (fileToLoad >> tempToken)
    {
        if (tempToken == "vertex")
        {
            break;  // Get out of the while
        }
    }

    // At this point, we've read the word "vertex".
    fileToLoad >> numVerticesLoaded;


    while (fileToLoad >> tempToken)
    {
        if (tempToken == "face")
        {
            break;  // Get out of the while
        }
    }
    // At this point, we've read the word "face".
    fileToLoad >> numTrianglesLoaded;


    while (fileToLoad >> tempToken)
    {
        if (tempToken == "end_header")
        {
            break;  // Get out of the while
        }
    }

    // At this point, we've read the word "end_header".

    // Now, we can read the vertices from the file...

    struct sTempPlyFileVertex_MushroomFormat
    {
        float x;
        float y;
        float z;
        float nx;
        float ny;
        float nz;
        float s;
        float t;
    };

    std::vector<sTempPlyFileVertex_MushroomFormat> vecVertsFromFile;

    for (unsigned int count = 0; count != numVerticesLoaded; count++)
    {
        // -0.113944 0.168176 -0.404122 255 255 0 255 
        sTempPlyFileVertex_MushroomFormat tempVertexLoaded;

        fileToLoad >> tempVertexLoaded.x;
        fileToLoad >> tempVertexLoaded.y;
        fileToLoad >> tempVertexLoaded.z;
        fileToLoad >> tempVertexLoaded.nx;
        fileToLoad >> tempVertexLoaded.ny;
        fileToLoad >> tempVertexLoaded.nx;
        fileToLoad >> tempVertexLoaded.s;
        fileToLoad >> tempVertexLoaded.t;

//        tempVertexLoaded.x *= 10.0f;
//        tempVertexLoaded.y *= 10.0f;
//        tempVertexLoaded.z *= 10.0f;

        vecVertsFromFile.push_back(tempVertexLoaded);
    }

    // Now load the elements (aka triangles)
    struct sTempPlyFileTriangle
    {
        unsigned int vertex_0;
        unsigned int vertex_1;
        unsigned int vertex_2;
    };

    std::vector<sTempPlyFileTriangle> vecTrianglesFromFile;

    for (unsigned int count = 0; count != numTrianglesLoaded; count++)
    {
        // -0.113944 0.168176 -0.404122 255 255 0 255 
        sTempPlyFileTriangle tempTriangleLoaded;

        //  3 5 14 21
        unsigned int discardThis = 0;
        fileToLoad >> discardThis;      // Throw out the '3'

        fileToLoad >> tempTriangleLoaded.vertex_0;
        fileToLoad >> tempTriangleLoaded.vertex_1;
        fileToLoad >> tempTriangleLoaded.vertex_2;

        vecTrianglesFromFile.push_back(tempTriangleLoaded);
    }

    // Now I have to "translate" or convert this laoded data 
    //  into a fomat that the OpenGL code we already have 
    //  can draw to the screen.

    // Google C++ dynamic array

    unsigned int totalNumberOfVerticesToDraw = numTrianglesLoaded * 3;

    // Boom goes the dynomite!
    pVertexArray = new sVertexXYZ_RGB[totalNumberOfVerticesToDraw];

// DO NOT USE malloc if you are using "new". And why on Earth would you ever use malloc in C++??
//    pVertexArray = (sVertexXYZ_RGB*)malloc(totalNumberOfVerticesToDraw * sizeof(sVertexXYZ_RGB));

    // Note: this array will be filled with garbage values.
    // If this is very offensive to you, you can clear it with a loop, or memset
//    memset(pVertexArray, 0, totalNumberOfVerticesToDraw * sizeof(sVertexXYZ_RGB));
//    for ( unsigned int index = 0; index != 0; index++ )
//    {
//        pVertexArray[index].x = 0.0f; 
//        pVertexArray[index].y = 0.0f;
//        // ... and so on
//    }


    // Copy (or sort of "unroll" the PLY file into a 1D vertex list that we would like

    unsigned int finalVertexArrayIndex = 0;

    for (unsigned int triangleIndex = 0; triangleIndex != vecTrianglesFromFile.size(); triangleIndex++)
    {
        // Copy vertex 0...
        unsigned int vertexIndex_0 = vecTrianglesFromFile[triangleIndex].vertex_0;

        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_0].x;
        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_0].y;
        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_0].z;
        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
        pVertexArray[finalVertexArrayIndex].r = 1.0f;
        pVertexArray[finalVertexArrayIndex].g = 1.0f;
        pVertexArray[finalVertexArrayIndex].b = 1.0f;

        finalVertexArrayIndex++;

        // Copy vertex 1...
        unsigned int vertexIndex_1 = vecTrianglesFromFile[triangleIndex].vertex_1;

        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_1].x;
        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_1].y;
        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_1].z;
        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
        pVertexArray[finalVertexArrayIndex].r = 1.0f;
        pVertexArray[finalVertexArrayIndex].g = 1.0f;
        pVertexArray[finalVertexArrayIndex].b = 1.0f;

        finalVertexArrayIndex++;

        // Copy vertex 2...
        unsigned int vertexIndex_2 = vecTrianglesFromFile[triangleIndex].vertex_1;

        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_2].x;
        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_2].y;
        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_2].z;
        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
        pVertexArray[finalVertexArrayIndex].r = 1.0f;
        pVertexArray[finalVertexArrayIndex].g = 1.0f;
        pVertexArray[finalVertexArrayIndex].b = 1.0f;

        finalVertexArrayIndex++;
    }

    fileToLoad.close();

    return true;
}



bool Load_Doom_spider_mastermind_PlyFile(std::string filename,
                                         sVertexXYZ_RGB*& pVertexArray,
                                         unsigned int& numVerticesLoaded,
                                         unsigned int& numTrianglesLoaded)
{
    std::ifstream plyFile(filename);
    if ( ! plyFile.is_open()  )
    {
        return false;
    }



    std::string tempToken;
    while (plyFile >> tempToken)
    {
        if (tempToken == "vertex")
        {
            break;  // Get out of the while
        }
    }
    // Read number of vertices
    plyFile >> numVerticesLoaded;

    while (plyFile >> tempToken)
    {
        if (tempToken == "face")
        {
            break;  // Get out of the while
        }
    }
    // At this point, we've read the word "face".
    plyFile >> numTrianglesLoaded;


    while (plyFile >> tempToken)
    {
        if (tempToken == "end_header")
        {
            break;  // Get out of the while
        }
    }

    // 0.8062328 1.704445 2.518811 -0.8739801 -0.4150925 -0.2526995 
    struct sVertexFormatFromFile
    {
        float x, y, z;
        float nx, ny, nz;
//        float theData[6];
    };
    std::vector<sVertexFormatFromFile> vecVertsFromFile;

    for (unsigned int count = 0; count != numVerticesLoaded; count++)
    {
        // -0.113944 0.168176 -0.404122 255 255 0 255 
        sVertexFormatFromFile tempVertexLoaded;

        plyFile >> tempVertexLoaded.x;
        plyFile >> tempVertexLoaded.y;
        plyFile >> tempVertexLoaded.z;
        plyFile >> tempVertexLoaded.nx;
        plyFile >> tempVertexLoaded.ny;
        plyFile >> tempVertexLoaded.nx;


//        tempVertexLoaded.x += 2.0f;
// 
//        tempVertexLoaded.x *= 0.2f;
//        tempVertexLoaded.y *= 0.2f;
//        tempVertexLoaded.z *= 0.2f;



        vecVertsFromFile.push_back(tempVertexLoaded);
    }

    // Now the triangles.
    // Technically the vertex indices (index values into the vertex array)

    std::vector<int> vecVertexIndices;

    for ( unsigned int triIndex = 0; triIndex != numTrianglesLoaded; triIndex++ )
    {
        // 3 55942 55950 55951

        int discard = 0;
        plyFile >> discard;                     // 3

        int nextNum = 0;
        plyFile >> nextNum;                     // 55942
        vecVertexIndices.push_back(nextNum);

        plyFile >> nextNum;                     // 55950
        vecVertexIndices.push_back(nextNum);

        plyFile >> nextNum;                     // 55951
        vecVertexIndices.push_back(nextNum);
    }

    // convert this into a format the the OpenGL code wants.
    // Why? Because that's the code that we have and it works...
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
    unsigned int totalVerticesToDraw = numTrianglesLoaded * 3;
    // Or vecVertexIndices.size()
    
    // Allocat this on the heap (dynamically)
 //   sVertexXYZ_RGB* pVertexArray = new sVertexXYZ_RGB[totalVerticesToDraw]
    
    pVertexArray = new sVertexXYZ_RGB[totalVerticesToDraw];

    // Copy the data from the vectors we loaded into this c style array
    for (unsigned int index = 0; index != vecVertexIndices.size(); index++ )
    {
        // Get the vertex at this location...
        unsigned int vertexIndexINeed = vecVertexIndices[index];

        sVertexFormatFromFile thisVertex = vecVertsFromFile[vertexIndexINeed];

        // To OpenGL
        // sVertexXYZ_RGB       sVertexFormatFromFile
        pVertexArray[index].x = thisVertex.x;
        pVertexArray[index].y = thisVertex.y;
        pVertexArray[index].z = thisVertex.z;
        // 
        pVertexArray[index].r = 1.0f;
        pVertexArray[index].g = 1.0f;
        pVertexArray[index].b = 1.0f;        

    }

    return true;
}


void ChangeLightSphereVisibility(bool bIsOn)
{
    cMeshObject* plightCentre = pFindObjectByFriendlyName("lightCentre");
    plightCentre->bIsVisible = bIsOn;

    cMeshObject* p90Percent = pFindObjectByFriendlyName("90Percent");
    p90Percent->bIsVisible = bIsOn;

    cMeshObject* p50Percent = pFindObjectByFriendlyName("50Percent");
    p50Percent->bIsVisible = bIsOn;

    cMeshObject* p25Percent = pFindObjectByFriendlyName("25Percent");
    p25Percent->bIsVisible = bIsOn;

    cMeshObject* p5Percent = pFindObjectByFriendlyName("5Percent");
    p5Percent->bIsVisible = bIsOn;

    return;
}

