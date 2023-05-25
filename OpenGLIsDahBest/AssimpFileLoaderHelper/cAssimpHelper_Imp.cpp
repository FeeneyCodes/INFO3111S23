#include "cAssimpHelper_Imp.h"

//#include <iostream>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <sstream>


cAssimpHelper_Imp::cAssimpHelper_Imp()
{

}

cAssimpHelper_Imp::~cAssimpHelper_Imp()
{

}

bool cAssimpHelper_Imp::Load3DModelFile(std::string filename, cAssimpHelper::sPostProcessFlags postProcessOptions)
{
    std::string filename_and_path = filename;

    if (this->m_basePath_no_end_slash != "")
    {
        filename_and_path = this->m_basePath_no_end_slash + '/' + filename;
    }

	// Translate the boolean helper flags to the assimp post processing flags
    unsigned int assimpPostProcessingFlags = this->m_loadAssimpPostProcessingFlags(postProcessOptions);

    // This is from the assimp help documentation
    // Create an instance of the Importer class
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll 
    // propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(filename_and_path.c_str(), assimpPostProcessingFlags);

//    const aiScene* scene = importer.ReadFile(filename_and_path.c_str(),
//                                             aiProcess_CalcTangentSpace |
//                                             aiProcess_Triangulate |
//                                             aiProcess_JoinIdenticalVertices |
//                                             aiProcess_SortByPType);
    // aiProcess_CalcTangentSpace:      Calculates the tangents and bitangents for the imported meshes.
    // aiProcess_Triangulate:           Triangulates all faces of all meshes.
    // aiProcess_JoinIdenticalVertices: Identifies and joins identical vertex data sets within all imported meshes.
    // aiProcess_SortByPType:           This step splits meshes with more than one primitive type in homogeneous sub-meshes.,
    // aiProcess_GenNormals:            Generates normals for all faces of all meshes.
    //                                  (is ignored if normals are already there at the time this flag is evaluated.)
    //                                  (can *NOT* be used with aiProcess_GenSmoothNormals)
    // aiProcess_GenSmoothNormals:      Generates smooth normals for all vertices in the mesh.
    //                                  (...is ignored if normals are already there at the time this flag is evaluated.)
    //                                  (can *NOT* be used with aiProcess_GenNormals)
    // aiProcess_FixInfacingNormals:    This step tries to determine which meshes have normal vectors that are facing inwards and inverts them.

                                       // If the import failed, report it
    if (!scene)
    {
        std::string errorString(importer.GetErrorString());
        this->m_AppendErrorString(errorString);
        return false;
    }
    // Now we can access the file's contents. 
//    DoTheSceneProcessing(scene);
    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

unsigned int cAssimpHelper_Imp::m_loadAssimpPostProcessingFlags(cAssimpHelper::sPostProcessFlags postProcessOptions)
{
	unsigned int assimpPostProcessingFlags = 0;

	if (postProcessOptions.bProcess_CalcTangentSpace)
	{
		assimpPostProcessingFlags |= aiProcess_CalcTangentSpace;
	}
	if (postProcessOptions.bProcess_CalcTangentSpace)
	{
		assimpPostProcessingFlags |= aiProcess_CalcTangentSpace;
	}
	if (postProcessOptions.bProcess_JoinIdenticalVertices)
	{
		assimpPostProcessingFlags |= aiProcess_JoinIdenticalVertices;
	}
	if (postProcessOptions.bProcess_MakeLeftHanded)
	{
		assimpPostProcessingFlags |= aiProcess_MakeLeftHanded;
	}
	if (postProcessOptions.bProcess_Triangulate)
	{
		assimpPostProcessingFlags |= aiProcess_Triangulate;
	}
	if (postProcessOptions.bProcess_RemoveComponent)
	{
		assimpPostProcessingFlags |= aiProcess_RemoveComponent;
	}
	if (postProcessOptions.bProcess_GenNormals)
	{
		assimpPostProcessingFlags |= aiProcess_GenNormals;
	}
	if (postProcessOptions.bProcess_GenSmoothNormals)
	{
		assimpPostProcessingFlags |= aiProcess_GenSmoothNormals;
	}
	if (postProcessOptions.bProcess_SplitLargeMeshes)
	{
		assimpPostProcessingFlags |= aiProcess_SplitLargeMeshes;
	}
	if (postProcessOptions.bProcess_PreTransformVertices)
	{
		assimpPostProcessingFlags |= aiProcess_PreTransformVertices;
	}
	if (postProcessOptions.bProcess_LimitBoneWeights)
	{
		assimpPostProcessingFlags |= aiProcess_LimitBoneWeights;
	}
	if (postProcessOptions.bProcess_ValidateDataStructure)
	{
		assimpPostProcessingFlags |= aiProcess_ValidateDataStructure;
	}
	if (postProcessOptions.bProcess_ImproveCacheLocality)
	{
		assimpPostProcessingFlags |= aiProcess_ImproveCacheLocality;
	}
	if (postProcessOptions.bProcess_RemoveRedundantMaterials)
	{
		assimpPostProcessingFlags |= aiProcess_RemoveRedundantMaterials;
	}
	if (postProcessOptions.bProcess_FixInfacingNormals)
	{
		assimpPostProcessingFlags |= aiProcess_FixInfacingNormals;
	}
	if (postProcessOptions.bProcess_SortByPType)
	{
		assimpPostProcessingFlags |= aiProcess_SortByPType;
	}
	if (postProcessOptions.bProcess_FindDegenerates)
	{
		assimpPostProcessingFlags |= aiProcess_FindDegenerates;
	}
	if (postProcessOptions.bProcess_FindInvalidData)
	{
		assimpPostProcessingFlags |= aiProcess_FindInvalidData;
	}
	if (postProcessOptions.bProcess_GenUVCoords)
	{
		assimpPostProcessingFlags |= aiProcess_GenUVCoords;
	}
	if (postProcessOptions.bProcess_TransformUVCoords)
	{
		assimpPostProcessingFlags |= aiProcess_TransformUVCoords;
	}
	if (postProcessOptions.bProcess_FindInstances)
	{
		assimpPostProcessingFlags |= aiProcess_FindInstances;
	}
	if (postProcessOptions.bProcess_OptimizeMeshes)
	{
		assimpPostProcessingFlags |= aiProcess_OptimizeMeshes;
	}
	if (postProcessOptions.bProcess_OptimizeGraph)
	{
		assimpPostProcessingFlags |= aiProcess_OptimizeGraph;
	}
	if (postProcessOptions.bProcess_FlipUVs)
	{
		assimpPostProcessingFlags |= aiProcess_FlipUVs;
	}
	if (postProcessOptions.bProcess_FlipWindingOrder)
	{
		assimpPostProcessingFlags |= aiProcess_FlipWindingOrder;
	}
	if (postProcessOptions.bProcess_SplitByBoneCount)
	{
		assimpPostProcessingFlags |= aiProcess_SplitByBoneCount;
	}
	if (postProcessOptions.bProcess_Debone)
	{
		assimpPostProcessingFlags |= aiProcess_Debone;
	}

	return assimpPostProcessingFlags;
}


std::string cAssimpHelper_Imp::getLastError(bool bAndClearErrors /*=true*/)
{
    std::string error = this->m_LastError;
    
    if ( bAndClearErrors )
    {
        this->m_LastError = "";
    }
    return error;
}

void cAssimpHelper_Imp::m_AppendErrorString(std::string errorString)
{
    std::stringstream ssError;
    if ( ! this->m_LastError.empty() ) 
    {
        ssError << this->m_LastError;
        ssError << std::endl;
    }
    ssError << errorString;
    return;
}


void cAssimpHelper_Imp::SetBasePath(std::string basepath_no_end_slash)
{
	this->m_basePath_no_end_slash = basepath_no_end_slash;
	return;
}

