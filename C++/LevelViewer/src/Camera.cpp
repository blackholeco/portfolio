/*
 *	Camera.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "Camera.h"

Camera::Camera()
{

}


void Camera::InitCamera(int angle, float ratio, float nearPlane, float farPlane)
{
	assert(angle > 0 && angle < 180);

	assert(nearPlane < farPlane);

	origAngle = angle;
	origRatio = ratio;
	origNear = nearPlane;
	origFar = farPlane;

	glInterface.setProjection(Matrix4::perspective(origAngle, origRatio, origNear, origFar));
}


void Camera::Offset(GLfloat dX, GLfloat dY, GLfloat dZ)
{
	position.x += dX;
	position.y += dY;
	position.z += dZ;
}


void Camera::SetCameraPosition(GLfloat pX, GLfloat pY, GLfloat pZ)
{
	position.x = pX;
	position.y = pY;
	position.z = pZ;
}


void Camera::Update()
{
	Vector3f tPos;

	if (targetObject != nullptr)
	{
		Vector2f tmp = targetObject->GetPosition();

		tPos.x = tmp.x;
		tPos.y = 0;
		tPos.z = tmp.y;

	}
	else
		tPos = target;

	glInterface.setViewMatrix(Matrix4::view(
		position, tPos, Vector3f(0, 1, 0)
	));
}


void Camera::SetTargetPosition(GLfloat tX, GLfloat tY, GLfloat tZ)
{
	target.x = tX;
	target.y = tY;
	target.z = tZ;
}


void Camera::SetTargetObject(GameObject& object, float followdist)
{
	targetObject = &object;
}

