#pragma once
#ifndef _cFlyCamera_HG_
#define _cFlyCamera_HG_

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL		// To get glm quaternion stuff to compile
#include <glm/gtx/quaternion.hpp>	// Note strange folder

#include <vector>

#include "iCamera.h"

class cFlyCamera : public iCamera
{
public:
	cFlyCamera();

	// **************************************
	// From iCamera
	virtual glm::vec3 getEye(void);
	virtual void setEye(glm::vec3 newEye);
	virtual glm::vec3 getAt(void);
	virtual void setAt(glm::vec3 newAt);
	virtual glm::vec3 getUp(void);
	virtual void setUp(glm::vec3 newUp);

	virtual bool Update(double deltaTime);
	// Used to set various commands on the camera
	virtual bool Update(std::string command, float data);
	virtual bool Update(std::string command, glm::vec2 data);
	virtual bool Update(std::string command, glm::vec3 data);

	virtual glm::vec3 getData(std::string command);

	// This will return an XYZ coordinate that's "in front" 
	//	of the camera (i.e. where the camera is facing) at
	//	a distance of distancetoPoint away.
	glm::vec3 getPointInFrontOfCamera(float distancetoPoint);

	// **************************************

	// These are used for the perspective projection 
	float nearPlane;
	float farPlane;
	float FOV;			// Field of view

	// Use these to allow us to use the LookAt() transform
	glm::vec3 eye;
	bool bKeepCameraFacingUp;
private:
	glm::vec3 m_at;			// A direction, not a location
	glm::vec3 m_up;
	// THESE DON'T LIKELY CHANGE
	glm::vec3 m_frontOfCamera;// = glm::vec3(0,0,1);
	glm::vec3 m_upIsYVector;	// = glm::vec3(0,1,0);

	// This is used by the update to tokenize or parse the command string
	void m_tokenizeString(std::string theString, std::vector<std::string>& vecTokens);

public:
	glm::vec3 getAtInWorldSpace(void);
	glm::vec3 getCameraDirection(void);
	glm::vec3 getUpVector(void);


	// If we are using the mouse to change direction...
private:
	float m_lastMouse_X;
	float m_lastMouse_Y;
	float m_lastMouse_Wheel;
	float m_Mouse_X;
	float m_Mouse_Y;
	float m_MouseWheel;
	// We don't know where the mouse is at the start, 
	//	so things like getDeltaMouse() methods will return 
	//	crazy things... this flag indicates the values are OK
	bool m_Mouse_Initial_State_Is_Set;
public:
	// These set methods will also clear the "last" mouse positions
	void setMouseXY(double newX, double newY);
	// The mouse wheel (on windows) only returns the delta, or the amount you scrolled
	void setMouseWheelDelta(double deltaWheel);

	float getMouseX(void);				// ...but we only need floats
	float getMouseY(void);

	float getDeltaMouseX(void);
	float getDeltaMouseY(void);
	float getMouseWheel(void);
	void clearMouseWheelValue(void);

	float movementSpeed;

	// z is "forward"
	// x is "left and right" (horizontal)
	// y is "up and down" (vertical)

	void MoveForward_Z(float amount);
	void MoveLeftRight_X(float amount);
	void MoveUpDown_Y(float amount);

	void Pitch_UpDown(float angleDegrees);	// around X
	void Yaw_LeftRight(float angleDegrees);	// around y
	void Roll_CW_CCW(float angleDegrees);	// around z

	// Same as above, but interpolates amount with deltaTime
	// (Making the motion smoother)
	// (FYI: These call the methods above)
	void MoveForward_Z(float amount, double deltaTime);
	void MoveLeftRight_X(float amount, double deltaTime);
	void MoveUpDown_Y(float amount, double deltaTime);

	void Pitch_UpDown(float angleDegrees, double deltaTime);	// around X
	void Yaw_LeftRight(float angleDegrees, double deltaTime);	// around y
	void Roll_CW_CCW(float angleDegrees, double deltaTime);		// around z

	// Note: This will never go below m_DEFAULT_MIN_DELTA_TIME_STEP
	void setMinimumDeltaTimeStep(double newMinDeltaTimeStep);
	double getMinimumDeltaTimeStep(void);

// ***********************************************************************
// THIS ++DOESN'T++ WORK AT THE MOMENT
	void setOrientationFromLookAtXYZLocation( glm::vec3 atXYZ );
	// This assumes that Y is up and it's locked to the up angle
	// Yaw: Y, Pitch: X, Roll: Z
	glm::vec3 getEulerAngleFromOrientation(void);
private:
	// This assumes that Y is up and it's locked to the up angle
	// Yaw: Y, Pitch: X, Roll: Z
	glm::vec3 m_getEulerAngleFromOrientation(glm::vec3 atXYZ);
// ***********************************************************************

public:
	glm::quat getQOrientation(void) { return this->qOrientation; };
	// Assuming "up" is +ve Y, then:
	// +ve around Y axis is a "right" turn Yaw
	// +ve around X axis is a "down" Pitch (-ve is "pitching up")
	// +ve around Z axis is a "clockwise" rotation or "roll"
	void setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);
private:
	// This will lead to direction, etc.
	glm::quat qOrientation;

	void m_UpdateAtFromOrientation(void);
	void m_UpdateUpFromOrientation(void);

	// This is used with some functions to clamp the maximum delta time that can be passed.
	// Defaults to 0.1s or 100 ms (which is WAY too high at 10 FPS)
	double m_MinimumDeltaTimeStep;
	static const double m_DEFAULT_MIN_DELTA_TIME_STEP;// = 0.1;

	// This is used for commands that are asynchronous, like the "look at tracking" command
	enum eState
	{
		DEFAULT_OR_UNSPECIFIED,		// The usual state
		TRACKING_AT_TO_TARGET
	};

	eState m_CurrentState;

};

#endif 
