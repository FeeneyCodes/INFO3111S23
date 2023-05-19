#include "cLightManager.h"

#include "globalStuff.h"

//
extern glm::vec3 g_cameraEye;// = glm::vec3(0.0, 0.0, -4.0f);
extern glm::vec3 g_cameraTarget;// = glm::vec3(0.0f, 0.0f, 0.0f);
extern glm::vec3 g_upVector;// = glm::vec3(0.0f, +1.0f, 0.0f);
	// The OpenGL call stuff

cLightManager::cLightManager()
{

}
cLightManager::~cLightManager()
{
}

void cLightManager::UpdateLightInfoToShader(unsigned int shaderProgram)
{
	//struct sLight
	//{
	//	vec4 position;	// xyz, ignoring w (4th parameter)	
	//	vec4 diffuse;
	//	vec4 specular;	// rgb = highlight colour, w = power
	//	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	//	vec4 direction;	// Spot, directional lights, ignoring w (4th parameter)	
	//	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	//					// 0 = pointlight
	//					// 1 = spot light
	//					// 2 = directional light
	//	vec4 param2;	// x = 0 for off, 1 for on
	//};
	//// Size of array MUST be known at compile time
	//const int NUMBEROFLIGHTS = 1;
	//uniform sLight theLights[NUMBEROFLIGHTS];

	for ( unsigned int index = 0; index != cLightManager::NUMBEROFLIGHTS; index++ )
	{

		glUniform4f(this->myLights[index].position_UL,
					this->myLights[index].position.x, 
					this->myLights[index].position.y, 
					this->myLights[index].position.z, 
					1.0f);		// If there's a 4th value and you're not sure, make it 1.0f

		glUniform4f(this->myLights[index].diffuse_UL,
					this->myLights[index].diffuse.x, this->myLights[index].diffuse.y, 
					this->myLights[index].diffuse.z, this->myLights[index].diffuse.w);

		glUniform4f(this->myLights[index].specular_UL,
					this->myLights[index].specular.x, this->myLights[index].specular.y, 
					this->myLights[index].specular.z, this->myLights[index].specular.w);

		glUniform4f(this->myLights[index].atten_UL,
					this->myLights[index].atten.x, this->myLights[index].atten.y, 
					this->myLights[index].atten.z, this->myLights[index].atten.w);

		glUniform4f(this->myLights[index].direction_UL,
					this->myLights[index].direction.x, this->myLights[index].direction.y,
					this->myLights[index].direction.z, this->myLights[index].direction.w);

		glUniform4f(this->myLights[index].param1_UL,
					this->myLights[index].param1.x, this->myLights[index].param1.y,
					this->myLights[index].param1.z, this->myLights[index].param1.w);

		glUniform4f(this->myLights[index].param2_UL,
					this->myLights[index].param2.x, this->myLights[index].param2.y,
					this->myLights[index].param2.z, this->myLights[index].param2.w);

	}//for ( unsigned int index...
	return;
}

void cLightManager::LoadUniformLocationsFromShader(GLuint shaderProgram)
{
	this->myLights[0].position_UL =  glGetUniformLocation(shaderProgram, "theLights[0].position");
	this->myLights[0].diffuse_UL =  glGetUniformLocation(shaderProgram, "theLights[0].diffuse");
	this->myLights[0].specular_UL =  glGetUniformLocation(shaderProgram, "theLights[0].specular");
	this->myLights[0].atten_UL =  glGetUniformLocation(shaderProgram, "theLights[0].atten");
	this->myLights[0].direction_UL =  glGetUniformLocation(shaderProgram, "theLights[0].direction");
	this->myLights[0].param1_UL =  glGetUniformLocation(shaderProgram, "theLights[0].param1");
	this->myLights[0].param2_UL =  glGetUniformLocation(shaderProgram, "theLights[0].param2");

	this->myLights[1].position_UL =  glGetUniformLocation(shaderProgram, "theLights[1].position");
	this->myLights[1].diffuse_UL =  glGetUniformLocation(shaderProgram, "theLights[1].diffuse");
	this->myLights[1].specular_UL =  glGetUniformLocation(shaderProgram, "theLights[1].specular");
	this->myLights[1].atten_UL =  glGetUniformLocation(shaderProgram, "theLights[1].atten");
	this->myLights[1].direction_UL =  glGetUniformLocation(shaderProgram, "theLights[1].direction");
	this->myLights[1].param1_UL =  glGetUniformLocation(shaderProgram, "theLights[1].param1");
	this->myLights[1].param2_UL =  glGetUniformLocation(shaderProgram, "theLights[1].param2");

	// ... and so on for the other lights...

	return;
}