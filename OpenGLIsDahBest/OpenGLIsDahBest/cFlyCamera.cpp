#include "cFlyCamera.h"

#include <glm/gtc/constants.hpp>		// For PI

cFlyCamera::cFlyCamera()
{
	// This is the same eye location that we started with.
	// (with the GLFW 'triangle of death' RGB triangle starting code)
	this->m_Eye = glm::vec3(0.0f, 0.0f, -4.0f);
}


void cFlyCamera::setEye(glm::vec3 newEye)
{
	this->m_Eye = newEye;
	return;
}

void cFlyCamera::setEye(float x, float y, float z)
{
	this->m_Eye.x = x;
	this->m_Eye.y = y;
	this->m_Eye.z = z;
	return;
}

glm::vec3 cFlyCamera::getEye(void)
{
	return this->m_Eye;
}

void cFlyCamera::setMovementSpeed(float newSpeed)
{
	this->m_movementSpeed = newSpeed;
	return;
}

float cFlyCamera::getMovementSpeed(void)
{
	return this->m_movementSpeed;
}

void cFlyCamera::setTurnSensitivity(float newTurnSensitivity)
{
	this->m_turnSensitivity = newTurnSensitivity;
	return;
}

float cFlyCamera::getTurnSensitivity(void)
{
	return this->m_turnSensitivity;
}

glm::vec3 cFlyCamera::getAt(void)
{
	// original code:
	//	gluLookAt(
	//		cameraX, cameraY, cameraZ,
	//		cameraX + sin(cameraYaw), cameraY + sin(cameraPitch), cameraZ - cos(cameraYaw),
	//		0.0f, 1.0f, 0.0f		<--- Looks like it's using Y as "up"
	//	);
	glm::vec3 cameraTarget;
	cameraTarget.x = this->m_Eye.x + sin( glm::radians(this->m_cameraYaw_in_degrees) );
	cameraTarget.y = this->m_Eye.y + sin( glm::radians(this->m_cameraPitch_in_degrees) );
	cameraTarget.z = this->m_Eye.z - cos( glm::radians(this->m_cameraYaw_in_degrees) );

	return cameraTarget;
}


void cFlyCamera::MoveForwards(void)
{
	// Original code:
	//			// Move forward
	//        case 'w':
	//			cameraX -= movementSpeed * sin(cameraYaw);
	//			cameraZ += movementSpeed * cos(cameraYaw);
	//			break;

	this->m_Eye.x -= this->m_movementSpeed * glm::sin( glm::radians(this->m_cameraYaw_in_degrees) );
	this->m_Eye.z += this->m_movementSpeed * glm::cos( glm::radians(this->m_cameraYaw_in_degrees) );

	return;
}

//void MoveForwards(float amount);
void cFlyCamera::MoveBackwards(void)
{
	// Oritinal code:
	//			// Move backward
	//        case 's':
	//			cameraX += movementSpeed * sin(cameraYaw);
	//			cameraZ -= movementSpeed * cos(cameraYaw);
	//			break;

	this->m_Eye.x += this->m_movementSpeed * glm::sin( glm::radians( this->m_cameraYaw_in_degrees) );
	this->m_Eye.z -= this->m_movementSpeed * glm::cos( glm::radians( this->m_cameraYaw_in_degrees) );

	return;
}
//void cFlyCamera::MoveBackwards(float amount);

void cFlyCamera::StrafeRight(void)
{
	// Original code:
	//			// Strafe right
	//        case 'd':
	//			cameraX += movementSpeed * sin(cameraYaw + 3.14 / 2);
	//			cameraZ -= movementSpeed * cos(cameraYaw + 3.14 / 2);
	//			break;

	this->m_Eye.x += this->m_movementSpeed * glm::sin( glm::radians(this->m_cameraYaw_in_degrees + 90.0f) );
	this->m_Eye.z -= this->m_movementSpeed * glm::cos( glm::radians(this->m_cameraYaw_in_degrees + 90.0f) );

	return;
}
//void cFlyCamera::StrafeRight(float amount);

void cFlyCamera::StrafeLeft(void)
{
	// Original code:
	//			// Strafe left
	//        case 'a':
	//			cameraX -= movementSpeed * sin(cameraYaw + 3.14 / 2);
	//			cameraZ += movementSpeed * cos(cameraYaw + 3.14 / 2);
	//			break;

	this->m_Eye.x -= this->m_movementSpeed * glm::sin( glm::radians(this->m_cameraYaw_in_degrees + 90.0f) );
	this->m_Eye.z += this->m_movementSpeed * glm::cos( glm::radians(this->m_cameraYaw_in_degrees + 90.0f) );

	return;
}
//void cFlyCamera::StrafeLeft(float amount);

void cFlyCamera::RotateOrYawLeft(float angleChangeInDegrees)
{
	// Original code:
	//			// Rotate left
	//        case 'j':
	//			cameraYaw -= rotationSpeed;
	//			break;
	this->m_cameraYaw_in_degrees -= (this->m_turnSensitivity * angleChangeInDegrees);

	return;
}


void cFlyCamera::RotateOrYawRight(float angleChangeInDegrees)
{
	// Original code:
	//		// Rotate right
	//		case 'l':
	//			cameraYaw += rotationSpeed;
	//			break;
	this->m_cameraYaw_in_degrees += (this->m_turnSensitivity * angleChangeInDegrees);

	return;
}

void cFlyCamera::PitchUp(float angleChangeInDegrees)
{
	// Original code:
	//		// Rotate up
	//		case 'i':
	//			cameraPitch -= rotationSpeed;
	//			break;
	this->m_cameraPitch_in_degrees -= (this->m_turnSensitivity * angleChangeInDegrees);

	return;
}

void cFlyCamera::PitchDown(float angleChangeInDegrees)
{
	// Original code:
//		// Rotate down
//		case 'k':
//			cameraPitch += rotationSpeed;
//			break;
	this->m_cameraPitch_in_degrees += (this->m_turnSensitivity * angleChangeInDegrees);

	return;
}

void cFlyCamera::MoveUp(void)
{
	this->m_Eye.y += this->m_movementSpeed;
	return;
}

void cFlyCamera::MoveDown(void)
{
	this->m_Eye.y -= this->m_movementSpeed;
	return;
}

