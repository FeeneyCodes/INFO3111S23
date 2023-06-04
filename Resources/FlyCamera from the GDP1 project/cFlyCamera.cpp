#include "cFlyCamera.h"

//#include <glm/glm.hpp>
//#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/quaternion.hpp>
#include <sstream>

#include "helpfulGlobalFunctions.h"

cFlyCamera::cFlyCamera()
{
	this->eye = glm::vec3(0.0f);

	// This will be constant
	this->m_frontOfCamera = glm::vec3(0.0f, 0.0f, 1.0f);

	this->m_upIsYVector = glm::vec3(0.0f, 1.0f, 0.0f);


	this->movementSpeed = 10.0f;


	// Set initial orientation (all zero on Euler axes)
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	//// Initial "at" is 1 unit in front of the camera, along z
	//this->at = this->m_frontOfCamera;

	// If we are using the mouse
	this->setMouseXY(0.0, 0.0);
	this->m_MouseWheel = 0.0f;

	this->m_Mouse_Initial_State_Is_Set = false;

	this->bKeepCameraFacingUp = true;

	this->nearPlane = 10.0f;
	this->farPlane = 10'000'000.0f;
	this->FOV = 0.6f;			// Field of view

	this->m_CurrentState = cFlyCamera::DEFAULT_OR_UNSPECIFIED;

	this->setMinimumDeltaTimeStep(cFlyCamera::m_DEFAULT_MIN_DELTA_TIME_STEP);

	return;
}

//static 
const double cFlyCamera::m_DEFAULT_MIN_DELTA_TIME_STEP = 0.1;


glm::vec3 cFlyCamera::getAtInWorldSpace(void)
{
	// The "At" is relative to the where the camera is...
	// So just add the "At" to the current camera location
	return this->eye + this->m_at;
}

glm::vec3 cFlyCamera::getCameraDirection(void)
{
	this->m_UpdateAtFromOrientation();

	return this->m_at;
}



void cFlyCamera::MoveForward_Z(float amount)
{
	// We take the vector from the at to eye (i.e. direction)
	// Add this forward velocity along this direction.

	glm::vec3 direction = this->getAtInWorldSpace() - this->eye;

	// Make direction a "unit length"
	direction = glm::normalize(direction);

	// Generate a "forward" adjustment value 
	glm::vec3 amountToMove = direction * amount;

	// Add this to the eye
	this->eye += amountToMove;

	return;
}

void cFlyCamera::MoveUpDown_Y(float amount)
{
	// We use the Up vector to determine which direction (in world space) we need to go

	glm::vec3 vecAmountToMove = glm::normalize(this->getUpVector()) * amount;

	this->eye += vecAmountToMove;

	return;
}

void cFlyCamera::MoveLeftRight_X(float amount)
{
	// left and right is a little tricky, as we can "roll", 
	// because left and right change in world space.
	// 
	// If you perform a cross product between the "forward" and "up" vectors, you'll
	//	get a vector perpendicular to both of these, so left and right.
	// Example: Assuming "forward" is the Z axis, and "up" is the Y axis, then 
	//	performing the cross product of these gives you the X axis. 

	glm::vec3 vecLeft = glm::cross(this->getCameraDirection(), this->getUpVector());

	glm::vec3 vecAmountToMove = glm::normalize(vecLeft) * amount;

	this->eye += vecAmountToMove;

	return;
}


void cFlyCamera::Pitch_UpDown(float angleDegrees)	// around X
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(angleDegrees, 0.0f, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::Yaw_LeftRight(float angleDegrees)	// around y
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, angleDegrees, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::Roll_CW_CCW(float angleDegrees)	// around z
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, 0.0f, angleDegrees), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x),
								   glm::radians(newAnglesEuler.y),
								   glm::radians(newAnglesEuler.z));
	}

	this->qOrientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void cFlyCamera::setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->setMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cFlyCamera::adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x),
								  glm::radians(adjAngleEuler.y),
								  glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Per frame update (SLERP)
	//glm::quat rotatThisFrame = glm::slerp(rotationAdjust, deltaTime );

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->qOrientation *= rotationAdjust;
	this->qOrientation = this->qOrientation * rotationAdjust;

	return;
}

void cFlyCamera::adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->adjMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cFlyCamera::adjMeshOrientationQ(glm::quat adjOrientQ)
{
	this->qOrientation *= adjOrientQ;
	return;
}

void cFlyCamera::m_UpdateAtFromOrientation(void)
{
	// Have a rotation around the origin (eye)

	// vec4 new = mat4Transform * (vec4)Location

	// c'tor of mat4 can take a quaternion and generate 
	//	a rotation matrix from it... 
	glm::mat4 matRotation = glm::mat4(this->qOrientation);

	// Need to make these 4x4
	// NOTE: You could just keep these as 3x3 matrix values
	// (so you could keep them as vec3's....)

	glm::vec4 frontOfCamera = glm::vec4(this->m_frontOfCamera, 1.0f);

	glm::vec4 newAt = matRotation * frontOfCamera;

	// Update the "At"
	this->m_at = glm::vec3(newAt);


	return;
}

void cFlyCamera::m_UpdateUpFromOrientation(void)
{
	if (this->bKeepCameraFacingUp)
	{
		this->m_up = this->m_upIsYVector;
	}
	else
	{
		glm::mat4 matRotation = glm::mat4(this->qOrientation);

		glm::vec4 upVector = glm::vec4(this->m_upIsYVector, 1.0f);

		glm::vec4 newUp = matRotation * upVector;

		// Update the "At"
		this->m_up = glm::vec3(newUp);
	}

	return;
}

glm::vec3 cFlyCamera::getUpVector(void)
{
	return this->m_up;
}

void cFlyCamera::setMouseXY(double newX, double newY)
{
	this->m_lastMouse_X = this->m_Mouse_X;
	this->m_Mouse_X = (float)newX;

	this->m_lastMouse_Y = this->m_Mouse_Y;
	this->m_Mouse_Y = (float)newY;

	// Can return value numbers
	this->m_Mouse_Initial_State_Is_Set = true;

	return;
}

void cFlyCamera::setMouseWheelDelta(double deltaWheel)
{
	this->m_lastMouse_Wheel = this->m_MouseWheel;
	this->m_MouseWheel += (float)deltaWheel;
	return;
}

float cFlyCamera::getMouseX(void)
{
	return this->m_Mouse_X;
}

float cFlyCamera::getMouseY(void)
{
	return this->m_Mouse_Y;
}

float cFlyCamera::getDeltaMouseX(void)
{
	if (!this->m_Mouse_Initial_State_Is_Set)
	{
		return 0.0f;
	}
	return this->m_Mouse_X - this->m_lastMouse_X;
}

float cFlyCamera::getDeltaMouseY(void)
{
	if (!this->m_Mouse_Initial_State_Is_Set)
	{
		return 0.0f;
	}
	return this->m_Mouse_Y - this->m_lastMouse_Y;
}

float cFlyCamera::getMouseWheel(void)
{
	return this->m_MouseWheel;
}

void cFlyCamera::clearMouseWheelValue(void)
{
	this->m_MouseWheel = 0.0f;
	return;
}



// From iCamera
glm::vec3 cFlyCamera::getEye(void)
{
	return this->eye;
}

void cFlyCamera::setEye(glm::vec3 newEye)
{
	this->eye = newEye;
	return;
}


glm::vec3 cFlyCamera::getAt(void)
{
	return this->getAtInWorldSpace();
}

void cFlyCamera::setAt(glm::vec3 newAt)
{
	this->m_at = newAt;
	return;
}

glm::vec3 cFlyCamera::getUp(void)
{
	return this->getUpVector();
}

void cFlyCamera::setUp(glm::vec3 newUp)
{
	this->m_up = newUp;
	return;
}


bool cFlyCamera::Update(std::string command, float data)
{
	std::vector<std::string> vecTokens;
	this->m_tokenizeString(command, vecTokens);

	if (command == "")
	{	// Nothing to do.
		return false;
	}

	switch ((unsigned int)vecTokens.size())
	{
	case 0:
		// Nothing to do. The tokenized vector is empty
		return false;
		break;

	case 1:	// 1 word commands 

		if (vecTokens[0] == "setMouseWheelDelta" )	// if (command == "setMouseWheelDelta")
		{
			this->setMouseWheelDelta(data);
			return true;
		}
		else if (vecTokens[0] == "Yaw_LeftRight")
		{
			this->Yaw_LeftRight(data);
			return true;
		}
		else if (vecTokens[0] == "Pitch_UpDown")
		{
			this->Pitch_UpDown(data);
			return true;
		}
		else if (vecTokens[0] == "setMovementSpeed")
		{
			this->movementSpeed = data;
			return true;
		}
		else if (vecTokens[0] == "MoveForward_Z")
		{
			this->MoveForward_Z(data);
			return true;
		}
		else if (vecTokens[0] == "MoveLeftRight_X")
		{
			this->MoveLeftRight_X(data);
			return true;
		}
		else if (vecTokens[0] == "MoveUpDown_Y")
		{
			this->MoveUpDown_Y(data);
			return true;
		}
		else if (vecTokens[0] == "Roll_CW_CCW")
		{
			this->Roll_CW_CCW(data);
			return true;
		}
		break;

	case 2:	// 2 word commands 

		break;
	}

	

	// What? (unknown command)
	return false;
}

bool cFlyCamera::Update(std::string command, glm::vec2 data)
{
	if (command == "setMouseXY")
	{
		this->setMouseXY(data.x, data.y);
		return true;
	}

	// What?
	return false;
}

bool cFlyCamera::Update(std::string command, glm::vec3 data)
{
	std::vector<std::string> vecTokens;
	this->m_tokenizeString(command, vecTokens);

	if (command == "")
	{	// Nothing to do.
		return false;
	}

	switch ((unsigned int)vecTokens.size())
	{
	case 0:
		// Nothing to do. The tokenized vector is empty
		return false;
		break;

//	case 1:	// 1 word commands 
//		// No commands like this
//		return true;
//		break;

	case 2:	// 2 word commands 

		// Here's a setEye command that can optionally track
		// command = "Track Eye"
		// data = location of the target
		if (vecTokens[0] == "Track" && vecTokens[0] == "At")
		{

		}
		break;
	}


	return true;
}

bool cFlyCamera::Update(double deltaTime)
{
	return true;
}

glm::vec3 cFlyCamera::getData(std::string command)
{
	if (command == "getDeltaMouseX")
	{
		return glm::vec3(this->getDeltaMouseX(), 0.0f, 0.0f);
	}
	else if (command == "getDeltaMouseY")
	{
		return glm::vec3(this->getDeltaMouseY(), 0.0f, 0.0f);
	}
	else if (command == "getMovementSpeed")
	{
		return glm::vec3(this->movementSpeed, 0.0f, 0.0f);
	}

	// What?
	return glm::vec3(0.0f);
}

void cFlyCamera::MoveForward_Z(float amount, double deltaTime)
{
	// Clamp timestep
	deltaTime = (deltaTime > this->m_MinimumDeltaTimeStep ? this->m_MinimumDeltaTimeStep : deltaTime);

	amount = amount * (float)deltaTime;

	this->MoveForward_Z(amount);

	return;
}

void cFlyCamera::MoveLeftRight_X(float amount, double deltaTime)
{
	// Clamp timestep
	deltaTime = (deltaTime > this->m_MinimumDeltaTimeStep ? this->m_MinimumDeltaTimeStep : deltaTime);

	amount = amount * (float)deltaTime;

	this->MoveLeftRight_X(amount);

	return;
}

void cFlyCamera::MoveUpDown_Y(float amount, double deltaTime)
{
	// Clamp timestep
	deltaTime = (deltaTime > this->m_MinimumDeltaTimeStep ? this->m_MinimumDeltaTimeStep : deltaTime);

	amount = amount * (float)deltaTime;

	this->MoveUpDown_Y(amount);

	return;
}

void cFlyCamera::Pitch_UpDown(float angleDegrees, double deltaTime)		// around X
{
	// Clamp timestep
	deltaTime = (deltaTime > this->m_MinimumDeltaTimeStep ? this->m_MinimumDeltaTimeStep : deltaTime);

	angleDegrees = angleDegrees * (float)deltaTime;

	this->Pitch_UpDown(angleDegrees);

	return;
}

void cFlyCamera::Yaw_LeftRight(float angleDegrees, double deltaTime)	// around y
{
	// Clamp timestep
	deltaTime = (deltaTime > this->m_MinimumDeltaTimeStep ? this->m_MinimumDeltaTimeStep : deltaTime);

	angleDegrees = angleDegrees * (float)deltaTime;

	this->Yaw_LeftRight(angleDegrees);

	return;
}

void cFlyCamera::Roll_CW_CCW(float angleDegrees, double deltaTime)		// around z
{
	// Clamp timestep
	deltaTime = (deltaTime > this->m_MinimumDeltaTimeStep ? this->m_MinimumDeltaTimeStep : deltaTime);

	angleDegrees = angleDegrees * (float)deltaTime;

	this->Roll_CW_CCW(angleDegrees);

	return;
}



// Note: This will never go below m_DEFAULT_MIN_DELTA_TIME_STEP
void cFlyCamera::setMinimumDeltaTimeStep(double newMinDeltaTimeStep)
{
	if ( newMinDeltaTimeStep > cFlyCamera::m_DEFAULT_MIN_DELTA_TIME_STEP )
	{
		newMinDeltaTimeStep = cFlyCamera::m_DEFAULT_MIN_DELTA_TIME_STEP;
	}
	this->m_MinimumDeltaTimeStep = newMinDeltaTimeStep;
	return;
}

double cFlyCamera::getMinimumDeltaTimeStep(void)
{
	return this->m_MinimumDeltaTimeStep;
}

// This is used by the update to tokenize or parse the command string
void cFlyCamera::m_tokenizeString(std::string theString, std::vector<std::string>& vecTokens)
{
	std::stringstream ssString;
	ssString.str() = theString;

	// This is just like cin or ifstream:
	//	We 'stream' (operator>>) each word (separated by white space) into
	//	a string, until there's nothing left to stream.

	std::string token;
	while (ssString >> token)
	{
		vecTokens.push_back(token);
	}
	return;
}


// This will return an XYZ coordinate that's "in front" 
//	of the camera (i.e. where the camera is facing) at
//	a distance of distancetoPoint away.
glm::vec3 cFlyCamera::getPointInFrontOfCamera(float distancetoPoint)
{
	// Just like if we had a mat4, we can use the quaternion value
	//	to calculate a transformation in space of a point. 

	// 1. Make a matrix from the quaternion
	// 2. Multiply a point by this matrix

	// In glm you can convert a quaternion to a mat4 matrix
	glm::mat3 matRotation = glm::toMat3( this->getQOrientation() );

	// Your point is distancetoPoint "in front of" the camera.
	// Pretend the camera is at the origin (0,0,0)
	// and it faces +ve along the z. 
	// So if I'd never moved the camera, this point would be:
	// 
	// ( 0, 0, distancetoPoint )
	glm::vec3 frontOfCameraOriginal = glm::vec3( 0.0f, 0.0f, distancetoPoint );

	// Apply the transform
	glm::vec3 frontOfCameraNow = matRotation * frontOfCameraOriginal;

	// Move this point to where it is in world space (i.e. where the EYE is)
	frontOfCameraNow += this->eye;

	return glm::vec3( frontOfCameraNow );
}







#include <iostream>

void cFlyCamera::setOrientationFromLookAtXYZLocation(glm::vec3 atXYZ)
{
	// Here is a way to do that. 
	// Note that they are assuming that z is "up" not y, like we are. 
	// Also note the "correct up" code in the camera is doing part of this, 
	//	specifically finding out what is "UP" with respect to the world space
	//	and not camera space. 
	// https://in.mathworks.com/matlabcentral/answers/298940-how-to-calculate-roll-pitch-and-yaw-from-xyz-coordinates-of-3-planar-points

	glm::vec3 eulerAngleXYZ = this->m_getEulerAngleFromOrientation(atXYZ);

	this->setMeshOrientationEulerAngles(eulerAngleXYZ);

	return;
}

glm::vec3 cFlyCamera::getEulerAngleFromOrientation(void)
{
	return this->m_getEulerAngleFromOrientation( this->getAt() );
}

glm::vec3 cFlyCamera::m_getEulerAngleFromOrientation(glm::vec3 atXYZ)
{
	glm::vec3 orientationEulerAnglesXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

//	glm::vec3 pointInFrontOfCamera_atXYZ = this->getPointInFrontOfCamera( 1.0f );
	glm::vec3 pointInFrontOfCamera_atXYZ = atXYZ;

	glm::vec3 cameraDirection = glm::normalize( atXYZ - this->getEye() );

	// This ray is 90 degrees (i.e. normal) to the plane that is rotated around the 
	//	y-axis and is also along the "look at" ray. i.e. this gives us the "yaw" from 
	//	world z axis (-90 degrees). 
	glm::vec3 normalToUpAtPlane = glm::cross( this->getUp(), cameraDirection );

	// Getting the dot product of that will give me the cos() or that angle
	// Normally this is +1 along the z (so 0,0,1), but the cross is perpendicular to this,
	//	so is facing in the -ve x axis
	glm::vec3 vecPosZ = glm::vec3( 0.0f, 0.0f, 1.0f);
	float dotProd_posZ = glm::dot(normalToUpAtPlane, vecPosZ );

	glm::vec3 vecPosX = glm::vec3( 1.0f, 0.0f, 0.0f);
	float dotProd_posX = glm::dot(normalToUpAtPlane, vecPosX );


	float angleYaw_acos_posZ = glm::degrees(acos(dotProd_posZ));
	float angleYaw_acos_posX = glm::degrees(acos(dotProd_posX));

	// See what quadrant we are in:  (apparently there's an atan2() that does this, I guess??)
	// 
	//	acos(+Z), acos(+X), asin(+Z)
	// 
	//  Q4                          Q1
	//     (180 to 90) |  (90 to 0)   
	//       (90 to 0) |  (0 to 90)
	//      (-90 to 0) |  (0 to 90)
	//                 |
	//       270-360   |  0 to 90
	//	--------------------------
	//    (90 to 180)  |   (0 to 90)
	//    (180 to 90)  |  (90 to 180)
	//     (0 to -90)  |   (90 to 0)
	//                 |
	//       180-270  |  90 to 180
	//  Q3                          Q2
	float angleYaw = 0.0f;

	if ( angleYaw_acos_posZ < 90.0f )
	{	// Q1 or Q2
		angleYaw = angleYaw_acos_posX;
	}
	else
	{	// Q3 or Q4
		angleYaw = 360.0f - angleYaw_acos_posX;

	}

	// Now the "pitch" of the camera, normally along the x axis
	// We cross the normal we got before with the "look at" vector
	glm::vec3 normalToXPlane = glm::cross( normalToUpAtPlane, cameraDirection );
	float dotProd_Xaxis_to_Up = glm::dot( normalToXPlane, this->getUp() );
	float anglePitch_cos = glm::degrees(acos(dotProd_Xaxis_to_Up));

	float anglePitch = 180.0f - anglePitch_cos;
	if (pointInFrontOfCamera_atXYZ.y > this->getEye().y)
	{
		// Nose pointing "up", so -ve angle
		anglePitch = -anglePitch;
	}

	// Yaw: Y, Pitch: X, Roll: Z
	orientationEulerAnglesXYZ.y = angleYaw;
	orientationEulerAnglesXYZ.x = anglePitch;
	orientationEulerAnglesXYZ.z = 0.0f;
	
	return orientationEulerAnglesXYZ;
}
