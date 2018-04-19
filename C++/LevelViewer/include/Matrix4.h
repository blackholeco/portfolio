/*
 *	Matrix4.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include <array>
#include <cassert>

#include "Vector2.h"

using std::array;

/**
 *	Class to handle a 4x4 matrix, create useful matrices
	such as the transformation matrices, and the view and
	projection matrices, as well as define mathematical
	functions (such as matrix and vector multiplication)

	Default Matrix created with this class is the identity matrix
 */
class Matrix4
{
public:

	Matrix4();

	/**
	 *	Get the Matrix as a length-16 float array (row-leading)

	 *	@return A float array containing this Matrix's data
	 */
	const float* data() const;

	/**
	 *	Get a copy of the Identity Matrix

	 *	@return A new Matrix set to the Identity Matrix
	 */
	static Matrix4 identity()
	{
		return Matrix4();
	}

	/**
	 *	Generate a translation matrix that moves a point by the
		given vector

	 *	@param vec : A vector to move by

	 *	@return : The translation matrix to move a point by the given vector
	 */
	static Matrix4 translate(Vector3f vec);

	/**
	 *	Generate a translation matrix that moves a point by the
		given vector

	 *	@param x : Amount to translate along the x axis
	 *	@param y : Amount to translate along the y axis
	 *	@param z : Amount to translate along the z axis

	 *	@return : The translation matrix to move a point by the given amounts
	 */
	static Matrix4 translate(float x, float y, float z);

	 /**
	  *	Create a matrix that will apply a scale transformation to
		 a point

	  *	@param x : scale factor in the x axis
	  *	@param y : scale factor in the y axis
	  *	@param z : scale factor in the z axis

	  *	@return : A matrix that applies a scale transformation by the given scale factors
	  */
	static Matrix4 scale(float x, float y, float z);

	/**
	 *	Create a new Matrix describing a frustum projection.  Using this for your projection
		matrix will result in your scene appearing 3D

	 *	@param left : Position of the left clipping plane
	 *	@param right : Position of the right clipping plane
	 *	@param bottom : Position of the bottom clipping plane
	 *	@param top : Position of the top clipping plane
	 *	@param near : Position of the near clipping plane
	 *	@param far : Position of the far clipping plane

	 *	@return A new Matrix describing a frustum
	 */
	static Matrix4 frustum(float left, float right, float bottom, float top, float near, float far);

	/**
	 *	Create a Matrix which defines where the "camera" is in scene

	 *	@param eye : Position of the camera in scene
	 *	@param center : Point that the camera is looking at
	 *	@param up : Up vector for the view, shows which way is up
	 */
	static Matrix4 view(Vector3f eye, Vector3f center, Vector3f up);

	/**
	 *	Create a new Matrix describing an orthographic projection.  Using this
		for the projection matrix will result in your scene appearing 2D

	 *	@param left : Position of the left clipping plane
	 *	@param right : Position of the right clipping plane
	 *	@param bottom : Position of the bottom clipping plane
	 *	@param top : Position of the top clipping plane
	 *	@param near : Position of the near clipping plane
	 *	@param far : Position of the far clipping plane

	 *	@return A new Matrix describing an orthographic projection
	 */
	static Matrix4 ortho(float left, float right, float bottom, float top, float near, float far);

	/**
	 *	Create a new Matrix which defines a perspective transform.
		Resulting Matrix describes a frustum.

	 *	@param degrees : Angle of view
	 *	@param aspect : Aspect ratio of the view
	 *	@param near : Distance from camera to the near clipping plane
	 *	@param far : Distance from camera to the far clipping plane

	 *	@return A new Matrix describing a perspective transform
	 */
	static Matrix4 perspective(float degrees, float aspect, float near, float far);

	/**
	 *	Multiply two matrices together.  Resulting Matrix is this Matrix
		multiplied by the input Matrix

	 *	@param mat : The Matrix to multiply this Matrix by

	 *	@return The Matrix resulting from this * mat
	 */
	Matrix4 operator*(Matrix4 mat);

	/**
	 *	Multiply a Vector by a Matrix

	 *	@param vec : The Vector to multiply by this Matrix

	 *	@return The resulting Vector after applying this Matrix
	 */
	Vector3f operator*(Vector3f vec);

protected:
	float& operator[](unsigned int index);

	array<float, 16> matrix;
};

/*	//Three rotation matrices multiplied together
		float sinx = sin(degrees * x);
		float siny = sin(degrees * y);
		float sinz = sin(degrees * z);

		float cosx = cos(degrees * x);
		float cosy = cos(degrees * y);
		float cosz = cos(degrees * z);

		rotation[0] = cosy * cosz;
		rotation[1] = -cosy * sinz;
		rotation[2] = siny;
		rotation[3] = 0.0f;

		rotation[4] = (cosx * sinz) + (sinx * siny * cosz);
		rotation[5] = (cosx * cosz) - (sinx * siny * sinz);
		rotation[6] = -sinx * cosy;
		rotation[7] = 0.0f;

		rotation[8] = (sinx * sinz) - (cosx * siny * cosz);
		rotation[9] = (sinx * cosz) + (cosx * siny * sinz);
		rotation[10] = cosx * cosy;
		rotation[11] = 0.0f;

		rotation[12] = 0.0f;
		rotation[13] = 0.0f;
		rotation[14] = 0.0f;
		rotation[15] = 1.0f;*/