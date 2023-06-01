#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

class cMeshObject
{
public:
	cMeshObject();
//	cMeshObject(cMeshObject &n);		// Copy constructor

	std::string meshName;

	glm::vec3 position;
	glm::vec3 orientation;		// Angle around each axis.  0,90,0

	glm::vec3 diffuseColour;				// Was just 'colour'
	glm::vec3 specularColourRGB;
	float specularPower_or_shininess;

	// 0 is completely transparent
	// 1 is completely solid
	float alphaTransparency;

	float scale;
	bool isWireframe;
	bool bIsVisible;

	// If this is on, then we don't caculate lighting
	bool bDontLight;		

	std::string friendlyName;
	unsigned int ID;
	//...

	std::vector< cMeshObject* > vec_pChildMeshes;
	// Can't do this because the linker needs to know
	//	how big this cMeshObject is, but in order to do that, 
	//	it needs to know how big this vector will be, but 
	//	in order to do that, it needs to know how bit the cMeshObject is...
	// ...and so on... never ending. How sad for the compiler. 
//	std::vector< cMeshObject > vecChildMeshes;
};
