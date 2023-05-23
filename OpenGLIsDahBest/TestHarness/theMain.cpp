#include "../AssimpFileLoaderHelper/AssimpFileLoaderHelper.h"

#include <iostream>

int main()
{
	cAssimpHelper* pHelper = new cAssimpHelper();

	if ( ! pHelper->Load3DModelFile("bun_zipper.ply") )
	{
		std::cout << "Error:" << pHelper->getLastError();
	}


	delete pHelper;

	return 0;
}
