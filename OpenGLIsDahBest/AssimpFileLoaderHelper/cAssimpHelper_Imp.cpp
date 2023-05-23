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

bool cAssimpHelper_Imp::Load3DModelFile(std::string filename)
{
    std::string filename_and_path = filename;

    if (this->m_basePath_no_end_slash != "")
    {
        filename_and_path = this->m_basePath_no_end_slash + '/' + filename;
    }

    // This is from the assimp help documentation
    // Create an instance of the Importer class
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll 
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(filename_and_path.c_str(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);

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

