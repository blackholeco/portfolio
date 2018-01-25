/*
 *	Camera.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include "GameObject.h"

class Camera
{
public:
	Camera();

	/**
	 *	Initialize the Camera's projection

	 *	@param angle : Viewing angle
	 *	@param ratio : Camera's aspect ratio
	 *	@param nearPlane : Distance from the camera of the near clipping plane
	 *	@param farPlane : Distance from the camera of the far clipping plane
	 */
	void InitCamera(int angle, float ratio, float nearPlane, float farPlane);

	/**
	 *	Set the Camera's position

	 *	@param x : X position of the camera
	 *	@param y : Y position of the camera
	 *	@param z : Z position of the camera
	 */
	void SetCameraPosition(GLfloat x, GLfloat y, GLfloat z);

	/**
	 *	Move the Camera

	 *	@param dX : Amount to move the camera in x
	 *	@param dY : Amount to move the camera in y
	 *	@param dZ : Amount to move the camera in z
	 */
	void Offset(GLfloat dX, GLfloat dY, GLfloat dZ);

	/**
	 *	Set an object to use as this Camera's target.  The Camera will
		then always be looking at that object, until a new target is set.

	 *	@param object : Reference to an object to act as the target
	 *	@param followdist : How far the Target can be before the Camera moves to follow (currently unimplemented)
	 */
	void SetTargetObject(GameObject& object, float followdist);
	
	/**
	 *	Set a location in the scene for the Camera to look at.  The Camera will
		then always look at that point until a new target is set.

	 *	@param tX : X co-ordinate of the target
	 *	@param tY : y co-ordinate of the target
	 *	@param tZ : z co-ordinate of the target
	 */
	void SetTargetPosition(GLfloat tX, GLfloat tY, GLfloat tZ);

	/**
	 *	Update the camera, ensuring that it follows the target if it has moved
	 */
	void Update();

protected:
	int origAngle;
	GLfloat origRatio;
	float origNear;
	float origFar;

	Vector3f position;
	Vector3f target;

	GameObject* targetObject;
};