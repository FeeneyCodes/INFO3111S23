#pragma once

#include <string>

// Forward declaration of the implementation class
class cAssimpHelper_Imp;


class __declspec(dllexport) cAssimpHelper
{
public:
	cAssimpHelper();
	~cAssimpHelper();

	bool Load3DModelFile(std::string filename);
	void SetBasePath(std::string basepath_no_end_slash);

	std::string getLastError(bool bAndClearErrors = true);

//	void GetVertexPosistionsXYZ(std::vector< glm::vec3 >& vecPositions);
//	void GetVertexNormalsXYZ(std::vector< glm::vec3 >& vecPositions);
//	void GetVertexColorusRGBA(std::vector< glm::vec3 >& vecPositions);
//	void GetVertexTextureCoords(std::vector< glm::vec3 >& vecPositions);

//	void LetsDoThis(std::string message);
private:
	// The actual implementation class:
	// Note that this HAS to be a pointer.
	// If it wasn't, then the compiler would need to know the size, 
	//	and we'd have to include the actual header.
	// (Note we AREN'T doing this - we are only doing a forward declaration)
	// It's OK with the forward declaration because we're using a pointer, 
	//	and it knows how large a pointer is (it's just an int).
	cAssimpHelper_Imp* m_pImp;
};
