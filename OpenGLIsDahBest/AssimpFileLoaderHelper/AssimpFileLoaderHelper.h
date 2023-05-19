#pragma once

// NOTE: Only include THIS file. Not any of the other headers. 

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <string>

#include "cAssimpHelper.h"

__declspec(dllexport) void fnAssimpFileLoaderHelper(std::string message);

