#include "cMeshObject.h"

cMeshObject::cMeshObject()
{
	this->diffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
	// Most of the time, materials have the same specular highlight colour
	//	as the light, and since lights are almost always bright yellow or white
	//	you'll not notice a 'white' coloured specular highlight
	// BUT if it's a metal, then the colour is NOT white
	this->specularColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
	// This is how 'shiny' the object is
	// Shinier = smaller, tighter hightlights
	// Starts at 1.
	// Not shiny = 1
	// A little bit shiny = 10
	// More shiny = 100
	// REALLY shiny = 1,000 or 10,000 or higher
	float specularPower_or_shininess = 1.0f;

	// Default: everything is solid
	this->alphaTransparency = 1.0f;


	this->orientation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->isWireframe = false;
	this->bDontLight = false;
	this->scale = 1.0f;
	this->bIsVisible = true;
};
//
//cMeshObject::cMeshObject(cMeshObject& n)		// Copy constructor
//{
//	this->colour = glm::vec3(1.0f, 1.0f, 1.0f);
//	this->orientation = glm::vec3(0.0f, 0.0f, 0.0f);
//	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
//	this->isWireframe = false;
//	this->bDontLight = false;
//	this->scale = 1.0f;
//	this->bIsVisible = true;
//};
