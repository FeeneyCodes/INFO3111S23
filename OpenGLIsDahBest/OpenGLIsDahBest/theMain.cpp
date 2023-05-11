//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "globalStuff.h"

#include <iostream>
#include <fstream>
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
//sVertexXYZ_RGB* pSexyVertex = new sVertexXYZ_RGB[11582 * 3];

//sVertexXYZ_RGB pVertexArray[3] =
//{
//    //   X      Y     Z     r     g     b
//    { -0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f },            // Spawn a vertex shader instance
//    {  0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f },
//    {  0.0f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f }
//};

// Will allocate this in a moment...
sVertexXYZ_RGB* pVertexArray = NULL;    


//static const char* vertex_shader_text =
//"#version 110\n"
//"uniform mat4 MVP;\n"
//"attribute vec3 vCol;\n"
//"attribute vec2 vPos;\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_Position = MVP * vec4(vPos.x, vPos.y, 0.0, 1.0);\n"
//"    color = vCol;\n"
//"}\n";

//static const char* fragment_shader_text =
//"#version 110\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_FragColor = vec4(color, 1.0);\n"
//"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// Note I'm passing the array by reference so that the pointer
//  value I create inside the function can be "passed back out".
bool Load_mig29_xyz_rgba_PlyFile(std::string filename, 
                                 sVertexXYZ_RGB* &pVertexArray,
                                 unsigned int &numVerticesLoaded,
                                 unsigned int &numTrianglesLoaded);

bool Load_Mushrooms1_PlyFile(std::string filename,
                                 sVertexXYZ_RGB* &pVertexArray,
                                 unsigned int &numVerticesLoaded,
                                 unsigned int &numTrianglesLoaded);



int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer;
//    GLuint vertex_shader;
//    GLuint fragment_shader;
//    GLuint program;
    GLint mvp_location, vpos_location, vcol_location;

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

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);


    unsigned int numVerticesLoaded = 0;
    unsigned int numTrianglesLoaded = 0;

//    if (Load_mig29_xyz_rgba_PlyFile( "mig29_xyz_rgba.ply",
    if (Load_Mushrooms1_PlyFile( "Mushrooms1 (ASCII format).ply",
                                     pVertexArray,
                                     numVerticesLoaded,
                                     numTrianglesLoaded) )
    {
        std::cout << "File loaded OK" << std::endl;
    }
    else
    {
        std::cout << "Error! ALL IS LOST! FOREVER LOST! Can't load PLY file." << std::endl;
  //      return 0;
    }

//    unsigned int numberOfBytes = sizeof(sVertexXYZ_RGB) * 3;

//    unsigned int numberOfVerticesToDraw = numTrianglesLoaded * 3;
    unsigned int numberOfBytes = sizeof(sVertexXYZ_RGB) * numTrianglesLoaded * 3;
    
    glBufferData(GL_ARRAY_BUFFER, 
                 numberOfBytes,    // Each vertex in bytes
                 pVertexArray,  // vertices,                  // Pointer to the start of the array
                 GL_STATIC_DRAW);

    // Note that I can delete the original array if I want to.
    // The data has been copied from the C++ (application) side into the GPU (video card) RAM
    delete [] pVertexArray;

//
//    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
//    glCompileShader(vertex_shader);
//
//    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
//    glCompileShader(fragment_shader);
//
//    program = glCreateProgram();
//    glAttachShader(program, vertex_shader);
//    glAttachShader(program, fragment_shader);
//    glLinkProgram(program);
// 
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

    mvp_location = glGetUniformLocation(shaderProgram, "MVP");


// Vertex layout specification
 //   struct sVertexXYZ_RGB {
//        float x, y, z;      // vec2 to vec3 
//        float r, g, b;  };
    vpos_location = glGetAttribLocation(shaderProgram, "vPos");
    vcol_location = glGetAttribLocation(shaderProgram, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 
                          3, 
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(sVertexXYZ_RGB),       // sizeof(vertices[0]), 
                          (void*)offsetof(sVertexXYZ_RGB, x)); // (void*)0);

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 
                          3, 
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(sVertexXYZ_RGB),   // sizeof(vertices[0]), 
                          (void*)offsetof(sVertexXYZ_RGB, r));
                          //(void*)(sizeof(float) * 3));
//


    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
//        mat4x4 m, p, mvp;
        glm::mat4 m, p, v, mvp;


        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

//        mat4x4_identity(m);
        m = glm::mat4(1.0f);        // Identity matrix
//        mat4x4_rotate_Z(m, m, (float)glfwGetTime());
        glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
                                           (float)glm::radians(-90.0f),  // glfwGetTime(),
                                           glm::vec3(1.0f, 0.0f, 0.0f));
//        mat4x4_mul(mvp, p, m);
        m = matRotateZ * m;


//        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);

        p = glm::perspective(0.6f,
                             ratio,
                             0.1f,          // Near plane
                             1000.0f);      // Far plane

        v = glm::mat4(1.0f);

        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -150.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        v = glm::lookAt(cameraEye,
                        cameraTarget,
                        upVector);

        mvp = p * v * m;

//        glUseProgram(program);
//        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));


        // uniform vec3 colorOverrideRGB;
        GLint colorOverrideRGB_UL = glGetUniformLocation(shaderProgram, "colorOverrideRGB");
        glUniform3f(colorOverrideRGB_UL, 1.0f, 1.0f, 1.0f);

        // GL_POINT, GL_LINE, and GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        unsigned int numberOfVerticesToDraw = numTrianglesLoaded * 3;
        glDrawArrays(GL_TRIANGLES, 0, numberOfVerticesToDraw);


        glfwSwapBuffers(window);
        glfwPollEvents();
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

