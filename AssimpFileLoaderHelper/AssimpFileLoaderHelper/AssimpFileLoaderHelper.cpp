// AssimpFileLoaderHelper.cpp : Defines the functions for the static library.
//

#include "AssimpFileLoaderHelper.h"

#include <iostream>

// TODO: This is an example of a library function
__declspec(dllexport) void fnAssimpFileLoaderHelper(std::string message)
{
	std::cout << "\'" << message << "\' from the library file." << std::endl;
	return;
}
