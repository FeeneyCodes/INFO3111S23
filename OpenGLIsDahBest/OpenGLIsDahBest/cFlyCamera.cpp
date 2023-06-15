#include "cFlyCamera.h"

#include <glm/gtc/constants.hpp>		// For PI

#include <iostream>

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

void cFlyCamera::TurnToLookAt(glm::vec3 newTarget, float maxDeltaAngleChangePerStepDegrees, 
							  float minimumCloseEnoughDistance)
{
	this->m_bIsTurning = true;
	this->m_maxmaxAngleChangePerSecondPerSecond = maxDeltaAngleChangePerStepDegrees;
	this->m_CloseEnoughToTargetEpsilon = minimumCloseEnoughDistance;
	this->m_newTarget = newTarget;

	return;
}

void cFlyCamera::Update(double deltaTime)
{
	const double MAX_DELTA_TIME = 1.0 / 60.0;		// 60Hz step is most
	if ( deltaTime > MAX_DELTA_TIME )
	{
		deltaTime = MAX_DELTA_TIME;
	}

	// Are we truning to face something? 
	if ( this->m_bIsTurning )
	{
		// Get the distance from the object we want and the current camera
		float distanceToTarget = glm::distance(this->m_newTarget, this->m_Eye);

		// Where would our current "at" be if extended to that location?
		glm::vec3 currentCameraViewVector = this->getAt() - this->m_Eye;

		// Extend this the length of where our new target is
		glm::vec3 currentTargetLocationYXZ = currentCameraViewVector * distanceToTarget;
		// Correct for eye placement
		currentTargetLocationYXZ += this->m_Eye;

		// get the distance between the desired target and our current "at"
		// this is always positive.
		float distanceFromCurrentAtToDesiredTarget = glm::distance(currentTargetLocationYXZ, this->m_newTarget);

		std::cout << "Turning: Distance to target: " << distanceFromCurrentAtToDesiredTarget << std::endl;

		// See if we are "close enough" to stop turning
		if ( distanceFromCurrentAtToDesiredTarget <= this->m_CloseEnoughToTargetEpsilon )
		{
			// Yup, we're close enough
			// Stop turning
			this->m_bIsTurning = false;
			return;
		}

		// We aren't looking at the target, yet

		// Slightly update the current angle so that it's "closer" to the new target
		// First, let's update the yaw (y axis angle)
		// One direction will get us closer...
		float deltaAngleChangeThisFrame = this->m_maxmaxAngleChangePerSecondPerSecond * deltaTime;

		{ // Yaw first, do the yaw angle
			// Check if turning +ve moves us "closer"
			float newTestYawValue = this->m_cameraYaw_in_degrees + deltaAngleChangeThisFrame;
			// With this new angle, this is what I'd be looking at...
			// ... but this is only one unit away
			glm::vec3 testTarget = this->m_getAtTestNormalizedDirection(newTestYawValue, this->m_cameraPitch_in_degrees);
			// Move this test target to the distance of our desired target
			glm::vec3 testTargetWorldXYZ = testTarget * distanceToTarget;
			// Adjust for eye
			testTargetWorldXYZ += this->m_Eye;

			// Get the new test distance from this test location to the actual thing we want to look at 
			float distanceTestAtToTarget = glm::distance(testTargetWorldXYZ, this->m_newTarget);

			// Is this larger or smaller than the current distance
			// distanceFromCurrentAtToDesiredTarget : current distance
			// distanceTestAtToTarget : test distance with new angle
			if ( distanceTestAtToTarget < distanceFromCurrentAtToDesiredTarget )
			{
				// Yes, this is closer, so we want to turn this way 
				// Update the yaw angle
				this->m_cameraYaw_in_degrees = newTestYawValue;
			}
			else 
			{
				// We need to turn the other way -ve
				this->m_cameraYaw_in_degrees = this->m_cameraYaw_in_degrees - deltaAngleChangeThisFrame;
			}
		}// Yaw first, do the yaw angle

		{ // Now Pitch
			// Check if turning +ve moves us "closer"
			float newTestPitchValue = this->m_cameraYaw_in_degrees + deltaAngleChangeThisFrame;
			// With this new angle, this is what I'd be looking at...
			// ... but this is only one unit away
			glm::vec3 testTarget = this->m_getAtTestNormalizedDirection(newTestPitchValue, this->m_cameraPitch_in_degrees);
			// Move this test target to the distance of our desired target
			glm::vec3 testTargetWorldXYZ = testTarget * distanceToTarget;
			// Adjust for eye
			testTargetWorldXYZ += this->m_Eye;

			// Get the new test distance from this test location to the actual thing we want to look at 
			float distanceTestAtToTarget = glm::distance(testTargetWorldXYZ, this->m_newTarget);

			// Is this larger or smaller than the current distance
			// distanceFromCurrentAtToDesiredTarget : current distance
			// distanceTestAtToTarget : test distance with new angle
			if (distanceTestAtToTarget < distanceFromCurrentAtToDesiredTarget)
			{
				// Yes, this is closer, so we want to turn this way 
				// Update the yaw angle
				this->m_cameraPitch_in_degrees = newTestPitchValue;
			}
			else
			{
				// We need to turn the other way -ve
				this->m_cameraPitch_in_degrees = this->m_cameraPitch_in_degrees - deltaAngleChangeThisFrame;
			}
		}// Yaw first, do the yaw angle

	}//	if ( this->m_bIsTurning )


	return;
}


void cFlyCamera::LookAtImmediate(glm::vec3 newTarget, float minimumCloseEnoughDistance)
{
	bool bKeepAdjusting = true;

	// give up after this many steps
	const unsigned int MAX_ITERATIONS = 10'000;

	unsigned int iterationsSoFar = 0;

	while ( bKeepAdjusting && (iterationsSoFar < MAX_ITERATIONS))
	{

		iterationsSoFar++;

		// Get the distance from the object we want and the current camera
		float distanceToTarget = glm::distance(this->m_newTarget, this->m_Eye);

		// Where would our current "at" be if extended to that location?
		glm::vec3 currentCameraViewVector = this->getAt() - this->m_Eye;

		// Extend this the length of where our new target is
		glm::vec3 currentTargetLocationYXZ = currentCameraViewVector * distanceToTarget;
		// Correct for eye placement
		currentTargetLocationYXZ += this->m_Eye;

		// get the distance between the desired target and our current "at"
		// this is always positive.
		float distanceFromCurrentAtToDesiredTarget = glm::distance(currentTargetLocationYXZ, this->m_newTarget);

//		std::cout << "Turning: Distance to target: " << distanceFromCurrentAtToDesiredTarget << std::endl;

		// See if we are "close enough" to stop turning
		if (distanceFromCurrentAtToDesiredTarget <= this->m_CloseEnoughToTargetEpsilon)
		{
			// Yup, we're close enough
			// Stop turning
			bKeepAdjusting = false;
			return;
		}


		// We aren't looking at the target, yet

		// Slightly update the current angle so that it's "closer" to the new target
		// First, let's update the yaw (y axis angle)
		// One direction will get us closer...
		const float teenyWeenyCameraAngleChange = 1.0f / 100.0f;

		float deltaAngleChangeThisFrame = teenyWeenyCameraAngleChange;

		{ // Yaw first, do the yaw angle
			// Check if turning +ve moves us "closer"
			float newTestYawValue = this->m_cameraYaw_in_degrees + deltaAngleChangeThisFrame;
			// With this new angle, this is what I'd be looking at...
			// ... but this is only one unit away
			glm::vec3 testTarget = this->m_getAtTestNormalizedDirection(newTestYawValue, this->m_cameraPitch_in_degrees);
			// Move this test target to the distance of our desired target
			glm::vec3 testTargetWorldXYZ = testTarget * distanceToTarget;
			// Adjust for eye
			testTargetWorldXYZ += this->m_Eye;

			// Get the new test distance from this test location to the actual thing we want to look at 
			float distanceTestAtToTarget = glm::distance(testTargetWorldXYZ, this->m_newTarget);

			// Is this larger or smaller than the current distance
			// distanceFromCurrentAtToDesiredTarget : current distance
			// distanceTestAtToTarget : test distance with new angle
			if (distanceTestAtToTarget < distanceFromCurrentAtToDesiredTarget)
			{
				// Yes, this is closer, so we want to turn this way 
				// Update the yaw angle
				this->m_cameraYaw_in_degrees = newTestYawValue;
			}
			else
			{
				// We need to turn the other way -ve
				this->m_cameraYaw_in_degrees = this->m_cameraYaw_in_degrees - deltaAngleChangeThisFrame;
			}
		}// Yaw first, do the yaw angle

		{ // Now Pitch
			// Check if turning +ve moves us "closer"
			float newTestPitchValue = this->m_cameraYaw_in_degrees + deltaAngleChangeThisFrame;
			// With this new angle, this is what I'd be looking at...
			// ... but this is only one unit away
			glm::vec3 testTarget = this->m_getAtTestNormalizedDirection(newTestPitchValue, this->m_cameraPitch_in_degrees);
			// Move this test target to the distance of our desired target
			glm::vec3 testTargetWorldXYZ = testTarget * distanceToTarget;
			// Adjust for eye
			testTargetWorldXYZ += this->m_Eye;

			// Get the new test distance from this test location to the actual thing we want to look at 
			float distanceTestAtToTarget = glm::distance(testTargetWorldXYZ, this->m_newTarget);

			// Is this larger or smaller than the current distance
			// distanceFromCurrentAtToDesiredTarget : current distance
			// distanceTestAtToTarget : test distance with new angle
			if (distanceTestAtToTarget < distanceFromCurrentAtToDesiredTarget)
			{
				// Yes, this is closer, so we want to turn this way 
				// Update the yaw angle
				this->m_cameraPitch_in_degrees = newTestPitchValue;
			}
			else
			{
				// We need to turn the other way -ve
				this->m_cameraPitch_in_degrees = this->m_cameraPitch_in_degrees - deltaAngleChangeThisFrame;
			}
		}// Yaw first, do the yaw angle



	}//while ( bKeepAdjusting )

	return;
}




glm::vec3 cFlyCamera::m_getAtTestNormalizedDirection(float testYaw, float testPitch)
{
	// original code:
	//	gluLookAt(
	//		cameraX, cameraY, cameraZ,
	//		cameraX + sin(cameraYaw), cameraY + sin(cameraPitch), cameraZ - cos(cameraYaw),
	//		0.0f, 1.0f, 0.0f		<--- Looks like it's using Y as "up"
	//	);
	glm::vec3 cameraTarget;
	// Assume the eye is at the origin for this test 
	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.f);
	cameraTarget.x = origin.x + sin(glm::radians(testYaw));
	cameraTarget.y = origin.y + sin(glm::radians(testPitch));
	cameraTarget.z = origin.z - cos(glm::radians(testYaw));

	return cameraTarget;
}