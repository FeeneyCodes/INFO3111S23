#include "globalStuff.h"

#include <vector>
#include <string>
#include "cVAOManager/cVAOManager.h"
#include "cMeshObject.h"
#include <iostream>


// Returns true if there were no errors.
// If there are errors, vecErrors has error detail text.
bool LoadModels(cVAOManager* pModelManger, GLuint shaderProgram,
                std::vector< cMeshObject* > &vec_pMeshesToDraw,
                std::vector<std::string> &vecErrors, 
                bool bLogProgressToConsole /*=true*/)
{
    bool bAnyErrors = false;

    std::vector<std::string> vecModelsToLoad;

    //vecModelsToLoad.push_back("assets/models/spider_mastermind.bmd6model.fbx.ascii_Y_up.ply");
    //vecModelsToLoad.push_back("assets/models/Mushrooms1_xyz_normal.ply");
    //vecModelsToLoad.push_back("assets/models/bun_zipper_xyz_n.ply");
    //vecModelsToLoad.push_back("assets/models/mig29_xyz_n.ply");
    //vecModelsToLoad.push_back("assets/models/camion jugete_xyz_n.ply");
    //vecModelsToLoad.push_back("assets/models/FractalTerrainFromMeshLab_xyz_n.ply");
    //vecModelsToLoad.push_back("assets/models/Apartment Building_26_xyz_n (flat shaded export).ply");
    //vecModelsToLoad.push_back("assets/models/Smooth_UV_Sphere_xyz_n.ply");
    //vecModelsToLoad.push_back("assets/models/Battlestar_Galactica_Res_0_(444,087 faces)_xyz_n.ply");
    //vecModelsToLoad.push_back("assets/models/SpaceShuttleOrbiter_xyz_n_small.ply");

    // These models have UV (texture) coordinates
    vecModelsToLoad.push_back("assets/models/spider_mastermind.bmd6model.fbx.ascii_Y_up_UVs.ply");
    vecModelsToLoad.push_back("assets/models/Smooth_UV_Sphere_xyz_n_uv.ply");
    vecModelsToLoad.push_back("assets/models/Mushrooms1_xyz_n_UVs.ply");
    vecModelsToLoad.push_back("assets/models/camion jugete_xyz_n_UV.ply");
    vecModelsToLoad.push_back("assets/models/FractalTerrainFromMeshLab_xyz_n_UV.ply");
    vecModelsToLoad.push_back("assets/models/Apartment Building_26_xyz_n_UV (flat shaded export).ply");
    vecModelsToLoad.push_back("assets/models/bun_zipper_xyz_n_UV.ply");
    vecModelsToLoad.push_back("assets/models/mig29_xyz_n_UV.ply");
    vecModelsToLoad.push_back("assets/models/SpaceShuttleOrbiter_smallxyz_n_UV.ply");
    vecModelsToLoad.push_back("assets/models/Battlestar_Galactica_x10_(blender_export)_xyz_n_uv.ply");
    vecModelsToLoad.push_back("assets/models/Imposter_Shapes/Quad_2_sided_aligned_on_XY_plane.ply");

    for ( std::string modelName : vecModelsToLoad )
    {
        sModelDrawInfo modelILoadedInfo;
        if ( pModelManger->LoadModelIntoVAO(modelName, modelILoadedInfo, shaderProgram) )
        {
            if (bLogProgressToConsole)
            {
                std::cout
                    << "Loaded <" << modelName << "> OK: "
                    << modelILoadedInfo.numberOfTriangles << " triangles, "
                    << modelILoadedInfo.numberOfVertices << " vertices." << std::endl;
            }
        }
        else
        {
            // Error
            bAnyErrors = true;

            vecErrors.push_back("Error: Didn't load >" + modelName + "<");

            if (bLogProgressToConsole)
            {
                std::cout << "Error: Didn't load >" << modelName << "<" << std::endl;
            }
        }//if ( pModelManger->LoadModelIntoVAO(...

    }//for ...vecModelsToLoad





    // Since we are using pointers, we can handle the debug objects differently.
    // Now we can re-use this same object and draw the sphere over and over, changing it slightly.
    cMeshObject* pDebugSphere = new cMeshObject();
//    pDebugSphere->meshName = "assets/models/Smooth_UV_Sphere_xyz_n.ply";
    pDebugSphere->meshName = "assets/models/Smooth_UV_Sphere_xyz_n_uv.ply";
    pDebugSphere->diffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
    pDebugSphere->isWireframe = true;
    pDebugSphere->friendlyName = "DebugSphere";
    pDebugSphere->bDontLight = true;
    pDebugSphere->bIsVisible = false;       // **** NOTE THAT IT'S INVISIBLE ****
    vec_pMeshesToDraw.push_back(pDebugSphere);


    cMeshObject* pSpiderMesh1 = new cMeshObject();
    pSpiderMesh1->meshName = "assets/models/spider_mastermind.bmd6model.fbx.ascii_Y_up_UVs.ply";
//    pSpiderMesh1->meshName = "assets/models/spider_mastermind.bmd6model.fbx.ascii_Y_up.ply";
    pSpiderMesh1->friendlyName = "Mr. Spider";
    pSpiderMesh1->diffuseColour = glm::vec3(1.0f, 0.0f, 0.0f);
    pSpiderMesh1->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pSpiderMesh1->specularPower_or_shininess = 100.0f;

//    pSpiderMesh1->alphaTransparency = 0.5f;

    //    SpiderMesh.isWireframe = false;
    pSpiderMesh1->position = glm::vec3(-5.0f, 0.0f, 0.0f);
    vec_pMeshesToDraw.push_back(pSpiderMesh1);

    cMeshObject* pSpiderMesh2 = new cMeshObject();
    pSpiderMesh2->meshName = "assets/models/spider_mastermind.bmd6model.fbx.ascii_Y_up_UVs.ply";
//    pSpiderMesh2->meshName = "assets/models/spider_mastermind.bmd6model.fbx.ascii_Y_up.ply";
    pSpiderMesh2->friendlyName = "Mrs. Spider";
    pSpiderMesh2->diffuseColour = glm::vec3(0.0f, 1.0f, 0.0f);
    pSpiderMesh2->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pSpiderMesh2->specularPower_or_shininess = 100.0f;
//    pSpiderMesh2->alphaTransparency = 0.5f;
    pSpiderMesh2->position = glm::vec3(20.0f, -10.0f, 0.0f);
    vec_pMeshesToDraw.push_back(pSpiderMesh2);

    cMeshObject* pMushRoomMesh = new cMeshObject();
    pMushRoomMesh->meshName = "assets/models/Mushrooms1_xyz_n_UVs.ply";
    pMushRoomMesh->diffuseColour = glm::vec3(0.0f, 0.0f, 1.0f);
    pMushRoomMesh->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pMushRoomMesh->specularPower_or_shininess = 10.0f;
    pMushRoomMesh->position = glm::vec3(10.0f, -10.0f, 10.0f);
    pMushRoomMesh->scale = 0.25f;
    pMushRoomMesh->orientation.x = -90.0f;         // Note it's being converted into radians
    vec_pMeshesToDraw.push_back(pMushRoomMesh);


    cMeshObject* pToyTruck = new cMeshObject();
    pToyTruck->meshName = "assets/models/camion jugete_xyz_n_UV.ply";
    pToyTruck->diffuseColour = glm::vec3(0.0f, 1.0f, 1.0f);
    pToyTruck->position.x = 4.0f;
    pToyTruck->scale = 5.0f;
    pToyTruck->orientation.y = glm::radians(90.0f);
    pToyTruck->position = glm::vec3(-8.0f, -1.0f, -5.0f);
    vec_pMeshesToDraw.push_back(pToyTruck);

    cMeshObject* pToyTruck2 = new cMeshObject();
    pToyTruck2->meshName = "assets/models/camion jugete_xyz_n_UV.ply";
    pToyTruck2->diffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
    pToyTruck2->position.x = -4.0f;
    pToyTruck2->scale = 8.0f;
    pToyTruck2->orientation.x = glm::radians(135.0f);
    vec_pMeshesToDraw.push_back(pToyTruck2);

    cMeshObject* pTerrainMesh = new cMeshObject();
    pTerrainMesh->meshName = "assets/models/FractalTerrainFromMeshLab_xyz_n_UV.ply";
    pTerrainMesh->diffuseColour = glm::vec3(0.8f, 0.8f, 0.8f);
    //    terrainMesh.isWireframe = true;
    // There's a large mountain right at -z location, blocking the camera, 
    //  so I'm rotating it out of the way.
//    pTerrainMesh->orientation.y = 90.0f;     
    // The original model is 2560x2560 (x-z axis)
    pTerrainMesh->scale = 1.0f;
    // It's shifted 100 in the +z direction rather than having the 
    //  centre of the terrain being under the camera (so it goes off in the distance
    //  away from where the camera starts)
    pTerrainMesh->position = glm::vec3(0.0f, -100.0f, -650.0f);
    vec_pMeshesToDraw.push_back(pTerrainMesh);


    cMeshObject* pAppartmentBuildingMesh = new cMeshObject();
    pAppartmentBuildingMesh->meshName = "assets/models/Apartment Building_26_xyz_n_UV (flat shaded export).ply";
    pAppartmentBuildingMesh->diffuseColour = glm::vec3(0.8f, 0.8f, 0.8f);
    pAppartmentBuildingMesh->scale = 1.0f;
    pAppartmentBuildingMesh->position = glm::vec3(-50.0f, 0.0f, 0.0f);
    vec_pMeshesToDraw.push_back(pAppartmentBuildingMesh);


    cMeshObject* pBunny01 = new cMeshObject();
    pBunny01->meshName = "assets/models/bun_zipper_xyz_n_UV.ply";
    pBunny01->diffuseColour = glm::vec3(1.0f, 1.0f, 0.0f);
    pBunny01->position = glm::vec3(-5.0f, -3.0f, 5.0f);
    pBunny01->scale = 25.0f;
    vec_pMeshesToDraw.push_back(pBunny01);


    cMeshObject* pAirplane1 = new cMeshObject();
    pAirplane1->meshName = "assets/models/mig29_xyz_n_UV.ply";
    pAirplane1->diffuseColour = glm::vec3(61.0f/255.0f, 85.0f/255.0f, 184.0f/255.0f);
    pAirplane1->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pAirplane1->specularPower_or_shininess = 10.0f;
    pAirplane1->position = glm::vec3(10.0f, 0.0f, -5.0f);
    pAirplane1->orientation.x = -90.0f;
    pAirplane1->orientation.z = 180.0f;
    pAirplane1->scale = 10.0f;
    vec_pMeshesToDraw.push_back(pAirplane1);


    cMeshObject* pSpaceShuttle = new cMeshObject();
    pSpaceShuttle->meshName = "assets/models/SpaceShuttleOrbiter_smallxyz_n_UV.ply";
    pSpaceShuttle->diffuseColour = glm::vec3(0.0f, 1.0f, 1.0f);

    pSpaceShuttle->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pSpaceShuttle->specularPower_or_shininess = 1000.0f;
    pSpaceShuttle->position = glm::vec3(15.0f, 2.5, 0.0f);
    pSpaceShuttle->scale = 1.0f;
    //    pSpaceShuttle->isWireframe = true;
    //    pSpaceShuttle->bDontLight = true;
    pSpaceShuttle->friendlyName = "Space Shuttle";
    vec_pMeshesToDraw.push_back(pSpaceShuttle);


    cMeshObject* pGalactica = new cMeshObject();
    pGalactica->meshName = "assets/models/Battlestar_Galactica_x10_(blender_export)_xyz_n_uv.ply";
    pGalactica->diffuseColour = glm::vec3(0.4f, 0.4f, 0.5f);
    pGalactica->friendlyName = "Galactica";
    pGalactica->orientation.y = 180.0f;
    pGalactica->position = glm::vec3(0.0f, 800.0f, -6000.0f);
    //pGalactica->isWireframe = true;
    //pGalactica->bDontLight = true;
    vec_pMeshesToDraw.push_back(pGalactica);


    // "Imposter" flat quad
    cMeshObject* p2DImposterQuad = new cMeshObject();
    p2DImposterQuad->meshName = "assets/models/Imposter_Shapes/Quad_2_sided_aligned_on_XY_plane.ply";
    p2DImposterQuad->diffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
    p2DImposterQuad->friendlyName = "2D Quad Imposter";
    p2DImposterQuad->scale = 5.0f;
    p2DImposterQuad->position.x = 3.0f;
    p2DImposterQuad->position.y = 3.0f;
    //p2DImposterQuad->isWireframe = true;
    //p2DImposterQuad->bDontLight = true;
    vec_pMeshesToDraw.push_back(p2DImposterQuad);





//    toyTruck2.diffuseColour = glm::vec3(1.0f, 0.0f, 0.0f);





    return ! bAnyErrors;
}








// OLD loading models, that aren't used anymore
//
//
//bool Load_mig29_xyz_rgba_PlyFile(std::string filename,
//                                 sVertexXYZ_RGB*& pVertexArray,
//                                 unsigned int& numVerticesLoaded,
//                                 unsigned int& numTrianglesLoaded)
//{
//    std::ifstream fileToLoad(filename.c_str());
//
//    if (fileToLoad.is_open() == false)
//    {
//        // Oh no!
//        return false;
//    }
//
//    std::string tempToken;
//    while (fileToLoad >> tempToken)
//    {
//        if (tempToken == "vertex")
//        {
//            break;  // Get out of the while
//        }
//    }
//
//    // At this point, we've read the word "vertex".
//    fileToLoad >> numVerticesLoaded;
//
//
//    while (fileToLoad >> tempToken)
//    {
//        if (tempToken == "face")
//        {
//            break;  // Get out of the while
//        }
//    }
//    // At this point, we've read the word "face".
//    fileToLoad >> numTrianglesLoaded;
//
//
//    while (fileToLoad >> tempToken)
//    {
//        if (tempToken == "end_header")
//        {
//            break;  // Get out of the while
//        }
//    }
//
//    // At this point, we've read the word "end_header".
//
//    // Now, we can read the vertices from the file...
//
//    struct sTempPlyFileVertex
//    {
//        float x;
//        float y;
//        float z;
//        int red;      // aka byte (0-255)  a 'char' is -128 to 127
//        int green;
//        int blue;
//        int alpha;
//    };
//
//    std::vector<sTempPlyFileVertex> vecVertsFromFile;
//
//    for (unsigned int count = 0; count != numVerticesLoaded; count++)
//    {
//        // -0.113944 0.168176 -0.404122 255 255 0 255 
//        sTempPlyFileVertex tempVertexLoaded;
//
//        fileToLoad >> tempVertexLoaded.x;
//        fileToLoad >> tempVertexLoaded.y;
//        fileToLoad >> tempVertexLoaded.z;
//        fileToLoad >> tempVertexLoaded.red;
//        fileToLoad >> tempVertexLoaded.green;
//        fileToLoad >> tempVertexLoaded.blue;
//        fileToLoad >> tempVertexLoaded.alpha;
//
////        tempVertexLoaded.x *= 10.0f;
////        tempVertexLoaded.y *= 10.0f;
////        tempVertexLoaded.z *= 10.0f;
//
//        vecVertsFromFile.push_back(tempVertexLoaded);
//    }
//
//    // Now load the elements (aka triangles)
//    struct sTempPlyFileTriangle
//    {
//        unsigned int vertex_0;
//        unsigned int vertex_1;
//        unsigned int vertex_2;
//    };
//
//    std::vector<sTempPlyFileTriangle> vecTrianglesFromFile;
//
//    for (unsigned int count = 0; count != numTrianglesLoaded; count++)
//    {
//        // -0.113944 0.168176 -0.404122 255 255 0 255 
//        sTempPlyFileTriangle tempTriangleLoaded;
//
//        //  3 5 14 21
//        unsigned int discardThis = 0;
//        fileToLoad >> discardThis;      // Throw out the '3'
//
//        fileToLoad >> tempTriangleLoaded.vertex_0;
//        fileToLoad >> tempTriangleLoaded.vertex_1;
//        fileToLoad >> tempTriangleLoaded.vertex_2;
//
//        vecTrianglesFromFile.push_back(tempTriangleLoaded);
//    }
//
//    // Now I have to "translate" or convert this laoded data 
//    //  into a fomat that the OpenGL code we already have 
//    //  can draw to the screen.
//
//    // Google C++ dynamic array
//
//    unsigned int totalNumberOfVerticesToDraw = numTrianglesLoaded * 3;
//
//    // Boom goes the dynomite!
//    pVertexArray = new sVertexXYZ_RGB[totalNumberOfVerticesToDraw];
//
//// DO NOT USE malloc if you are using "new". And why on Earth would you ever use malloc in C++??
////    pVertexArray = (sVertexXYZ_RGB*)malloc(totalNumberOfVerticesToDraw * sizeof(sVertexXYZ_RGB));
//
//    // Note: this array will be filled with garbage values.
//    // If this is very offensive to you, you can clear it with a loop, or memset
////    memset(pVertexArray, 0, totalNumberOfVerticesToDraw * sizeof(sVertexXYZ_RGB));
////    for ( unsigned int index = 0; index != 0; index++ )
////    {
////        pVertexArray[index].x = 0.0f; 
////        pVertexArray[index].y = 0.0f;
////        // ... and so on
////    }
//
//
//    // Copy (or sort of "unroll" the PLY file into a 1D vertex list that we would like
//
//    unsigned int finalVertexArrayIndex = 0;
//
//    for (unsigned int triangleIndex = 0; triangleIndex != vecTrianglesFromFile.size(); triangleIndex++)
//    {
//        // Copy vertex 0...
//        unsigned int vertexIndex_0 = vecTrianglesFromFile[triangleIndex].vertex_0;
//
//        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_0].x;
//        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_0].y;
//        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_0].z;
//        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
//        pVertexArray[finalVertexArrayIndex].r = static_cast<float>(vecVertsFromFile[vertexIndex_0].red / 255.0f);
//        pVertexArray[finalVertexArrayIndex].g = static_cast<float>(vecVertsFromFile[vertexIndex_0].green / 255.0f);
//        pVertexArray[finalVertexArrayIndex].b = static_cast<float>(vecVertsFromFile[vertexIndex_0].blue / 255.0f);
//
//        finalVertexArrayIndex++;
//
//        // Copy vertex 1...
//        unsigned int vertexIndex_1 = vecTrianglesFromFile[triangleIndex].vertex_1;
//
//        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_1].x;
//        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_1].y;
//        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_1].z;
//        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
//        pVertexArray[finalVertexArrayIndex].r = static_cast<float>(vecVertsFromFile[vertexIndex_1].red / 255.0f);
//        pVertexArray[finalVertexArrayIndex].g = static_cast<float>(vecVertsFromFile[vertexIndex_1].green / 255.0f);
//        pVertexArray[finalVertexArrayIndex].b = static_cast<float>(vecVertsFromFile[vertexIndex_1].blue / 255.0f);
//
//        finalVertexArrayIndex++;
//
//        // Copy vertex 2...
//        unsigned int vertexIndex_2 = vecTrianglesFromFile[triangleIndex].vertex_1;
//
//        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_2].x;
//        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_2].y;
//        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_2].z;
//        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
//        pVertexArray[finalVertexArrayIndex].r = static_cast<float>(vecVertsFromFile[vertexIndex_2].red / 255.0f);
//        pVertexArray[finalVertexArrayIndex].g = static_cast<float>(vecVertsFromFile[vertexIndex_2].green / 255.0f);
//        pVertexArray[finalVertexArrayIndex].b = static_cast<float>(vecVertsFromFile[vertexIndex_2].blue / 255.0f);
//
//        finalVertexArrayIndex++;
//    }
//
//    fileToLoad.close();
//
//    return true;
//}
//
//
//bool Load_Mushrooms1_PlyFile(std::string filename,
//                             sVertexXYZ_RGB*& pVertexArray,
//                             unsigned int& numVerticesLoaded,
//                             unsigned int& numTrianglesLoaded)
//{
//    std::ifstream fileToLoad(filename.c_str());
//
//    if (fileToLoad.is_open() == false)
//    {
//        // Oh no!
//        return false;
//    }
//
//    std::string tempToken;
//    while (fileToLoad >> tempToken)
//    {
//        if (tempToken == "vertex")
//        {
//            break;  // Get out of the while
//        }
//    }
//
//    // At this point, we've read the word "vertex".
//    fileToLoad >> numVerticesLoaded;
//
//
//    while (fileToLoad >> tempToken)
//    {
//        if (tempToken == "face")
//        {
//            break;  // Get out of the while
//        }
//    }
//    // At this point, we've read the word "face".
//    fileToLoad >> numTrianglesLoaded;
//
//
//    while (fileToLoad >> tempToken)
//    {
//        if (tempToken == "end_header")
//        {
//            break;  // Get out of the while
//        }
//    }
//
//    // At this point, we've read the word "end_header".
//
//    // Now, we can read the vertices from the file...
//
//    struct sTempPlyFileVertex_MushroomFormat
//    {
//        float x;
//        float y;
//        float z;
//        float nx;
//        float ny;
//        float nz;
//        float s;
//        float t;
//    };
//
//    std::vector<sTempPlyFileVertex_MushroomFormat> vecVertsFromFile;
//
//    for (unsigned int count = 0; count != numVerticesLoaded; count++)
//    {
//        // -0.113944 0.168176 -0.404122 255 255 0 255 
//        sTempPlyFileVertex_MushroomFormat tempVertexLoaded;
//
//        fileToLoad >> tempVertexLoaded.x;
//        fileToLoad >> tempVertexLoaded.y;
//        fileToLoad >> tempVertexLoaded.z;
//        fileToLoad >> tempVertexLoaded.nx;
//        fileToLoad >> tempVertexLoaded.ny;
//        fileToLoad >> tempVertexLoaded.nx;
//        fileToLoad >> tempVertexLoaded.s;
//        fileToLoad >> tempVertexLoaded.t;
//
////        tempVertexLoaded.x *= 10.0f;
////        tempVertexLoaded.y *= 10.0f;
////        tempVertexLoaded.z *= 10.0f;
//
//        vecVertsFromFile.push_back(tempVertexLoaded);
//    }
//
//    // Now load the elements (aka triangles)
//    struct sTempPlyFileTriangle
//    {
//        unsigned int vertex_0;
//        unsigned int vertex_1;
//        unsigned int vertex_2;
//    };
//
//    std::vector<sTempPlyFileTriangle> vecTrianglesFromFile;
//
//    for (unsigned int count = 0; count != numTrianglesLoaded; count++)
//    {
//        // -0.113944 0.168176 -0.404122 255 255 0 255 
//        sTempPlyFileTriangle tempTriangleLoaded;
//
//        //  3 5 14 21
//        unsigned int discardThis = 0;
//        fileToLoad >> discardThis;      // Throw out the '3'
//
//        fileToLoad >> tempTriangleLoaded.vertex_0;
//        fileToLoad >> tempTriangleLoaded.vertex_1;
//        fileToLoad >> tempTriangleLoaded.vertex_2;
//
//        vecTrianglesFromFile.push_back(tempTriangleLoaded);
//    }
//
//    // Now I have to "translate" or convert this laoded data 
//    //  into a fomat that the OpenGL code we already have 
//    //  can draw to the screen.
//
//    // Google C++ dynamic array
//
//    unsigned int totalNumberOfVerticesToDraw = numTrianglesLoaded * 3;
//
//    // Boom goes the dynomite!
//    pVertexArray = new sVertexXYZ_RGB[totalNumberOfVerticesToDraw];
//
//// DO NOT USE malloc if you are using "new". And why on Earth would you ever use malloc in C++??
////    pVertexArray = (sVertexXYZ_RGB*)malloc(totalNumberOfVerticesToDraw * sizeof(sVertexXYZ_RGB));
//
//    // Note: this array will be filled with garbage values.
//    // If this is very offensive to you, you can clear it with a loop, or memset
////    memset(pVertexArray, 0, totalNumberOfVerticesToDraw * sizeof(sVertexXYZ_RGB));
////    for ( unsigned int index = 0; index != 0; index++ )
////    {
////        pVertexArray[index].x = 0.0f; 
////        pVertexArray[index].y = 0.0f;
////        // ... and so on
////    }
//
//
//    // Copy (or sort of "unroll" the PLY file into a 1D vertex list that we would like
//
//    unsigned int finalVertexArrayIndex = 0;
//
//    for (unsigned int triangleIndex = 0; triangleIndex != vecTrianglesFromFile.size(); triangleIndex++)
//    {
//        // Copy vertex 0...
//        unsigned int vertexIndex_0 = vecTrianglesFromFile[triangleIndex].vertex_0;
//
//        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_0].x;
//        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_0].y;
//        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_0].z;
//        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
//        pVertexArray[finalVertexArrayIndex].r = 1.0f;
//        pVertexArray[finalVertexArrayIndex].g = 1.0f;
//        pVertexArray[finalVertexArrayIndex].b = 1.0f;
//
//        finalVertexArrayIndex++;
//
//        // Copy vertex 1...
//        unsigned int vertexIndex_1 = vecTrianglesFromFile[triangleIndex].vertex_1;
//
//        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_1].x;
//        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_1].y;
//        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_1].z;
//        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
//        pVertexArray[finalVertexArrayIndex].r = 1.0f;
//        pVertexArray[finalVertexArrayIndex].g = 1.0f;
//        pVertexArray[finalVertexArrayIndex].b = 1.0f;
//
//        finalVertexArrayIndex++;
//
//        // Copy vertex 2...
//        unsigned int vertexIndex_2 = vecTrianglesFromFile[triangleIndex].vertex_1;
//
//        pVertexArray[finalVertexArrayIndex].x = vecVertsFromFile[vertexIndex_2].x;
//        pVertexArray[finalVertexArrayIndex].y = vecVertsFromFile[vertexIndex_2].y;
//        pVertexArray[finalVertexArrayIndex].z = vecVertsFromFile[vertexIndex_2].z;
//        // This one has a float from 0 to 1.0       This one has a char from 0 to 255
//        pVertexArray[finalVertexArrayIndex].r = 1.0f;
//        pVertexArray[finalVertexArrayIndex].g = 1.0f;
//        pVertexArray[finalVertexArrayIndex].b = 1.0f;
//
//        finalVertexArrayIndex++;
//    }
//
//    fileToLoad.close();
//
//    return true;
//}
//
//
//
//bool Load_Doom_spider_mastermind_PlyFile(std::string filename,
//                                         sVertexXYZ_RGB*& pVertexArray,
//                                         unsigned int& numVerticesLoaded,
//                                         unsigned int& numTrianglesLoaded)
//{
//    std::ifstream plyFile(filename);
//    if (!plyFile.is_open())
//    {
//        return false;
//    }
//
//
//
//    std::string tempToken;
//    while (plyFile >> tempToken)
//    {
//        if (tempToken == "vertex")
//        {
//            break;  // Get out of the while
//        }
//    }
//    // Read number of vertices
//    plyFile >> numVerticesLoaded;
//
//    while (plyFile >> tempToken)
//    {
//        if (tempToken == "face")
//        {
//            break;  // Get out of the while
//        }
//    }
//    // At this point, we've read the word "face".
//    plyFile >> numTrianglesLoaded;
//
//
//    while (plyFile >> tempToken)
//    {
//        if (tempToken == "end_header")
//        {
//            break;  // Get out of the while
//        }
//    }
//
//    // 0.8062328 1.704445 2.518811 -0.8739801 -0.4150925 -0.2526995 
//    struct sVertexFormatFromFile
//    {
//        float x, y, z;
//        float nx, ny, nz;
////        float theData[6];
//    };
//    std::vector<sVertexFormatFromFile> vecVertsFromFile;
//
//    for (unsigned int count = 0; count != numVerticesLoaded; count++)
//    {
//        // -0.113944 0.168176 -0.404122 255 255 0 255 
//        sVertexFormatFromFile tempVertexLoaded;
//
//        plyFile >> tempVertexLoaded.x;
//        plyFile >> tempVertexLoaded.y;
//        plyFile >> tempVertexLoaded.z;
//        plyFile >> tempVertexLoaded.nx;
//        plyFile >> tempVertexLoaded.ny;
//        plyFile >> tempVertexLoaded.nx;
//
//
////        tempVertexLoaded.x += 2.0f;
//// 
////        tempVertexLoaded.x *= 0.2f;
////        tempVertexLoaded.y *= 0.2f;
////        tempVertexLoaded.z *= 0.2f;
//
//
//
//        vecVertsFromFile.push_back(tempVertexLoaded);
//    }
//
//    // Now the triangles.
//    // Technically the vertex indices (index values into the vertex array)
//
//    std::vector<int> vecVertexIndices;
//
//    for (unsigned int triIndex = 0; triIndex != numTrianglesLoaded; triIndex++)
//    {
//        // 3 55942 55950 55951
//
//        int discard = 0;
//        plyFile >> discard;                     // 3
//
//        int nextNum = 0;
//        plyFile >> nextNum;                     // 55942
//        vecVertexIndices.push_back(nextNum);
//
//        plyFile >> nextNum;                     // 55950
//        vecVertexIndices.push_back(nextNum);
//
//        plyFile >> nextNum;                     // 55951
//        vecVertexIndices.push_back(nextNum);
//    }
//
//    // convert this into a format the the OpenGL code wants.
//    // Why? Because that's the code that we have and it works...
////sVertexXYZ_RGB* pSexyVertex = new sVertexXYZ_RGB[3];
////
////// On the stack
////sVertexXYZ_RGB pVertexArray[3] =
////{
////    //   X      Y     Z     r     g     b
////    { -0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f },            // Spawn a vertex shader instance
////    {  0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f },
////    {  0.0f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f }
//// 
////};
//    unsigned int totalVerticesToDraw = numTrianglesLoaded * 3;
//    // Or vecVertexIndices.size()
//
//    // Allocat this on the heap (dynamically)
// //   sVertexXYZ_RGB* pVertexArray = new sVertexXYZ_RGB[totalVerticesToDraw]
//
//    pVertexArray = new sVertexXYZ_RGB[totalVerticesToDraw];
//
//    // Copy the data from the vectors we loaded into this c style array
//    for (unsigned int index = 0; index != vecVertexIndices.size(); index++)
//    {
//        // Get the vertex at this location...
//        unsigned int vertexIndexINeed = vecVertexIndices[index];
//
//        sVertexFormatFromFile thisVertex = vecVertsFromFile[vertexIndexINeed];
//
//        // To OpenGL
//        // sVertexXYZ_RGB       sVertexFormatFromFile
//        pVertexArray[index].x = thisVertex.x;
//        pVertexArray[index].y = thisVertex.y;
//        pVertexArray[index].z = thisVertex.z;
//        // 
//        pVertexArray[index].r = 1.0f;
//        pVertexArray[index].g = 1.0f;
//        pVertexArray[index].b = 1.0f;
//
//    }
//
//    return true;
//}
//
//
//

