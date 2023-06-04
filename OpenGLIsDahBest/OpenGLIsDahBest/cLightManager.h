#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct sLight
{
	sLight() 
	{
		this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->atten = glm::vec4(0.0f, 0.001f, 0.0001, 1000000.0f);
		this->diffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		this->param1 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		this->param2 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	glm::vec4 position;	// xyz, ignoring w (4th parameter)	
	glm::vec4 diffuse;
	glm::vec4 specular;	// rgb = highlight colour, w = power
	glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	glm::vec4 direction;	// Spot, directional lights, ignoring w (4th parameter)	
	glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
						// 0 = pointlight
						// 1 = spot light
						// 2 = directional light
	glm::vec4 param2;	// x = 0 for off, 1 for on

//	void SetLinearAttenuation(float newLinear);

	void TurnOff(void)
	{
		this->param2.x = 0.0f;
	}

	void TurnOn(void)
	{
		this->param2.x = 1.0f;
	}
	// Here's the uniform locations for these lights
	// i.e. the register locations inside the shader
	int position_UL = -1;
	int diffuse_UL = -1;
	int specular_UL = -1;
	int atten_UL = -1;
	int direction_UL = -1;
	int param1_UL = -1;
	int param2_UL = -1;
};

class cLightManager 
{
public:
	cLightManager();
	~cLightManager();

	static const int NUMBEROFLIGHTS = 10;
	sLight myLights[NUMBEROFLIGHTS];

	void LoadUniformLocationsFromShader(unsigned int shaderProgram);
	// Copy light info to shader
	void UpdateLightInfoToShader(unsigned int shaderProgram);
};