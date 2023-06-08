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

	static const unsigned int MAX_TEXTURES = 8;
	std::string textureName[MAX_TEXTURES];


	// 0 is completely transparent
	// 1 is completely solid
	float alphaTransparency;

	float scale;
	bool isWireframe;
	bool bIsVisible;

	// If this is on, then we don't caculate lighting
	bool bDontLight;		

	std::string friendlyName;
	unsigned int getUniqueID(void);
private:
	unsigned int m_ID;
	// Static variables can't be initialized here. 
	// See the cpp file for how to do it
	static unsigned int m_s_NextID;// = 1;
	// Const static CAN be set to something in the header
	static const unsigned int INITIAL_UNIQUE_ID = 1;
public:
	//...

	// Child objects are pointers to objects, not stack copies
	std::vector< cMeshObject* > vec_pChildMeshes;

	// Can't do this because the linker needs to know
	//	how big this cMeshObject is, but in order to do that, 
	//	it needs to know how big this vector will be, but 
	//	in order to do that, it needs to know how bit the cMeshObject is...
	// ...and so on... never ending. How sad for the compiler. 
//	std::vector< cMeshObject > vecChildMeshes;

	struct sMeshState
	{
		float scale;
		bool bDontLight;
		bool isWireFrame;
		bool bIsVisible;
		glm::vec3 position;
		glm::vec3 diffuseColour;
		float alphaTransparency;

		// Saves the above from mesh...
		void SaveState(cMeshObject* pTheMesh);
		// Loads the above to the mesh...
		void RestoreState(cMeshObject* pTheMesh);
	};

};


