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

#include "cMeshObject.h"

#include "../AssimpFileLoaderHelper/AssimpFileLoaderHelper.h"

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
glm::vec3 g_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_upVector = glm::vec3(0.0f, +1.0f, 0.0f);



static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// This is a list of the objects we want to draw in this scene
std::vector< cMeshObject > g_vecMeshesToDraw;
unsigned int g_selectedMeshIndex = 2;


//    void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    const float CAMERA_MOVE_SPEED = 0.1f;

    if ( mods == GLFW_MOD_SHIFT )
    {
        // Shift ONLY is down
    }

    // Note this is a bit mask 
    // 0001 0010 0100 
    // 0111 &
    // 0110
    // ----
    // 0000
    if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT )
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

void handleKeyboardInput(GLFWwindow* window)
{
    const float CAMERA_MOVE_SPEED = 0.1f;

    if ( glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) )
    {
        if ( glfwGetKey(window, GLFW_KEY_A) )
        {
            // Move "left"
            ::g_vecMeshesToDraw[::g_selectedMeshIndex].orientation.x += 0.01f;
        }
        else
        {
        // Move "left"
        ::g_cameraEye.x += CAMERA_MOVE_SPEED;
        }
    }
//    if ( glfwGetKey(window, GLFW_KEY_A) )
//    {
//    }
    if (glfwGetKey(window, GLFW_KEY_D) )
    {
        // Move "right"
        ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_W) )
    {
        // Move "forward"
        ::g_cameraEye.z += CAMERA_MOVE_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_S) )
    {
        // Move "backwards"
        ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) )
    {
        // Move "down"
        ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_E) )
    {
        // Move "up"
        ::g_cameraEye.y += CAMERA_MOVE_SPEED;
    }

    return;
}

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

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress );
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    // How does it know about the functions?
    // These are from the AssimpFileLoaderHelper .lib
    fnAssimpFileLoaderHelper("Yo!");

    // Because it's "exported" we can see it. 
    cAssimpHelper myHelper;
    myHelper.LetsDoThis("Hey");


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

    pModelManger->LoadModelIntoVAO("assets/models/Apartment Building_26_xyz_n.ply", modelILoadedInfo, shaderProgram);
    std::cout << "Loaded " << modelILoadedInfo.numberOfTriangles << " triangles" << std::endl;

    cMeshObject appartmentBuildingMesh;
    appartmentBuildingMesh.meshName = "assets/models/Apartment Building_26_xyz_n.ply";
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
    terrainMesh.position = glm::vec3(0.0f, -250.0f, 0.0f);
    ::g_vecMeshesToDraw.push_back(terrainMesh);

    cMeshObject SpiderMesh;
    SpiderMesh.meshName = "assets/models/spider_mastermind.bmd6model.fbx.ascii_Y_up.ply";
    SpiderMesh.colour = glm::vec3(1.0f, 0.0f, 0.0f);
//    SpiderMesh.isWireframe = false;
    SpiderMesh.position = glm::vec3(4.0f, -15.0f, 0.0f);
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

        mView = glm::lookAt(::g_cameraEye,
                            ::g_cameraTarget,  // newTarget
                            ::g_upVector);

        // Adding clearing the "depth" buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Turn on "depth buffer" testing
        glEnable(GL_DEPTH_TEST);


        // Draw all the stuff in the vector
        for (std::vector< cMeshObject >::iterator itMesh = ::g_vecMeshesToDraw.begin();
             itMesh != ::g_vecMeshesToDraw.end(); itMesh++)
        {
            // Copy the mesh (for ease of reading)
            cMeshObject currentMesh = *itMesh;


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

        std::stringstream ssTitle;
        ssTitle << "Camera (x,y,z): "
            << ::g_cameraEye.x << ", "
            << ::g_cameraEye.y << ", "
            << ::g_cameraEye.z;

        glfwSetWindowTitle( window, ssTitle.str().c_str() );

    }

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
