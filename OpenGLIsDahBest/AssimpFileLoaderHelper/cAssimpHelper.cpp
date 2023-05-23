#include "AssimpFileLoaderHelper.h"


// We include the ACTUAL header here, since the compiler needs
//  to know how large the class really is in order to create one. 
#include "cAssimpHelper_Imp.h"

cAssimpHelper::cAssimpHelper()
{
    // Create the implementation class...
    this->m_pImp = new cAssimpHelper_Imp();
}

cAssimpHelper::~cAssimpHelper()
{
    // Delete the implementation class
    delete this->m_pImp;

}

bool cAssimpHelper::Load3DModelFile(std::string filename)
{
    return this->m_pImp->Load3DModelFile(filename);
}

void cAssimpHelper::SetBasePath(std::string basepath_no_end_slash)
{
    this->m_pImp->SetBasePath(basepath_no_end_slash);
    return;
}

std::string cAssimpHelper::getLastError(bool bAndClearErrors /*=true*/)
{
    return this->m_pImp->getLastError();
}


