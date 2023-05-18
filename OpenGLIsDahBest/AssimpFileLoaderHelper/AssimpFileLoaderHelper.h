#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <string>

__declspec(dllexport) void fnAssimpFileLoaderHelper(std::string message);

class __declspec(dllexport) cAssimpHelper
{
public:
	cAssimpHelper();
	~cAssimpHelper();

//	void GetVertexPosistionsXYZ(std::vector< glm::vec3 >& vecPositions);
//	void GetVertexNormalsXYZ(std::vector< glm::vec3 >& vecPositions);
//	void GetVertexColorusRGBA(std::vector< glm::vec3 >& vecPositions);
//	void GetVertexTextureCoords(std::vector< glm::vec3 >& vecPositions);

	void LetsDoThis(std::string message);
};
