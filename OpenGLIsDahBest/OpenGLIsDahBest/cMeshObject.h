#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class cMeshObject
{
public:
	cMeshObject()
	{
		this->colour = glm::vec3(1.0f, 1.0f, 1.0f);
		this->orientation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->isWireframe = false;
		this->bDontLight = false;
		this->scale = 1.0f;
		this->bIsVisible = true;
	};
	std::string meshName;

	glm::vec3 position;
	glm::vec3 orientation;		// Angle around each axis.  0,90,0
	glm::vec3 colour;
	float scale;
	bool isWireframe;
	bool bIsVisible;

	// If this is on, then we don't caculate lighting
	bool bDontLight;		

	std::string friendlyName;
	unsigned int ID;
	//...
};
