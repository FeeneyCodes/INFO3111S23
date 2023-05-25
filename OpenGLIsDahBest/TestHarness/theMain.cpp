#include "../AssimpFileLoaderHelper/AssimpFileLoaderHelper.h"

#include <iostream>

int main()
{
	cAssimpHelper* pHelper = new cAssimpHelper();

	cAssimpHelper::sPostProcessFlags loadFlags;
//	if ( ! pHelper->Load3DModelFile("models/bun_zipper.ply", loadFlags) )
	if ( ! pHelper->Load3DModelFile("models/Sketchfab_2017_12_19_11_31_59 (exported).fbx", loadFlags) )
//	if ( ! pHelper->Load3DModelFile("models/Sketchfab_2017_12_19_11_31_59.blend", loadFlags) )
	{
		std::cout << "Error:" << pHelper->getLastError();
	}


	delete pHelper;

	return 0;
}
