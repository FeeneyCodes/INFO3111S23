#include "../AssimpFileLoaderHelper/AssimpFileLoaderHelper.h"

#include <iostream>

int main()
{
	AssimpHelper::cFileLoader* pHelper = new AssimpHelper::cFileLoader();

	//AssimpHelper::cString hey("Hey there");
	//std::string there(hey.c_str());
	//std::cout << there.c_str() << std::endl;

	AssimpHelper::cFileLoader::sPostProcessFlags loadFlags;
//	if ( ! pHelper->Load3DModelFile("models/bun_zipper.ply", loadFlags) )
	if ( ! pHelper->Load3DModelFile("models/Sketchfab_2017_12_19_11_31_59 (exported).fbx", loadFlags) )
//	if ( ! pHelper->Load3DModelFile("models/Sketchfab_2017_12_19_11_31_59.blend", loadFlags) )
	{
		std::cout << "Error:" << pHelper->getLastError().c_str();
	}


	delete pHelper;

	return 0;
}
