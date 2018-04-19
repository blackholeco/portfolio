/*
 *	Matrix4.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "Matrix4.h"


Matrix4::Matrix4()
{
	matrix.fill(0);

	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
};


const float* Matrix4::data() const
{
	return matrix.data();
}


Matrix4 Matrix4::translate(Vector3f vec)
{
	Matrix4 ret;

	ret[12] = vec.x;
	ret[13] = vec.y;
	ret[14] = vec.z;

	return ret;
}


Matrix4 Matrix4::translate(float x, float y, float z)
{
	Matrix4 ret;

	ret[12] = x;
	ret[13] = y;
	ret[14] = z;

	return ret;
}


Matrix4 Matrix4::scale(float x, float y, float z)
{
	Matrix4 ret;

	ret[0] = x;
	ret[5] = y;
	ret[10] = z;

	return ret;
}


Matrix4 Matrix4::frustum(float left, float right, float bottom, float top, float near, float far)
{
	Matrix4 ret;

	ret[0] = (2 * near) / (right - left);
	ret[1] = 0.0f;
	ret[2] = 0.0f;
	ret[3] = 0.0f;

	ret[4] = 0.0f;
	ret[5] = (2 * near) / (top - bottom);
	ret[6] = 0.0f;
	ret[7] = 0.0f;

	ret[8] = (right + left) / (right - left);
	ret[9] = (top + bottom) / (top - bottom);
	ret[10] = (near + far) / (near - far);
	ret[11] = -1.0f;

	ret[12] = 0.0f;
	ret[13] = 0.0f;
	ret[14] = (2 * near * far) / (near - far);
	ret[15] = 0.0f;

	return ret;
}

Matrix4 Matrix4::view(Vector3f eye, Vector3f center, Vector3f up)
{
	Vector3f vec = center - eye;
	Vector3f forward = Vector3f::normalize(vec);

	// get the angle between the vectors
	float angle = Vector3f::angle(forward, up);

	// Convert the angle to radians
	angle = angle / ((float)M_PI / 180.0f);

	Vector3f side;

	// correct for looking directly down the y axis
	// Without correcting, side would be (0, 0, 0), which prevents rendering)
	if (abs(angle) == 180.0f)
	{
		if (forward.y <= 0)
			side = Vector3f(1, 0, 0);
		else
			side = Vector3f(-1, 0, 0);
	}
	else
		side = Vector3f::cross(forward, up);

	side = Vector3f::normalize(side);

	Vector3f up2 = Vector3f::cross(side, forward);

	Matrix4 matrix;

	matrix[0] = side.x;
	matrix[1] = up2.x;
	matrix[2] = -forward.x;
	matrix[3] = 0.0f;

	matrix[4] = side.y;
	matrix[5] = up2.y;
	matrix[6] = -forward.y;
	matrix[7] = 0.0f;

	matrix[8] = side.z;
	matrix[9] = up2.z;
	matrix[10] = -forward.z;
	matrix[11] = 0.0f;

	matrix[12] = -Vector3f::dotProduct(side, eye);
	matrix[13] = -Vector3f::dotProduct(up2, eye);
	matrix[14] = Vector3f::dotProduct(forward, eye);
	matrix[15] = 1.0f;

	return matrix;
}


Matrix4 Matrix4::perspective(float degrees, float aspect, float near, float far)
{
	degrees = degrees * ((float)M_PI / 180.0f);	// convert to radians

	float y = 1.0f * tan(degrees / 2.0f);	// only need half the cone to get to y

	float x = y * aspect;

	return frustum(-x, x, -y, y, near, far);
};


float& Matrix4::operator[](unsigned int index)
{
	assert(index < matrix.size());

	return matrix[index];
}


Matrix4 Matrix4::ortho(float left, float right, float bottom, float top, float near, float far)
{
	Matrix4 ret;

	ret[0] = 2.0f / (right - left);
	ret[5] = 2.0f / (top - bottom);
	ret[10] = 2.0f / (near - far);
	ret[12] = (left + right) / (left - right);
	ret[13] = (bottom + top) / (bottom - top);
	ret[14] = (near + far) / (near - far);

	return ret;
}


Matrix4 Matrix4::operator*(Matrix4 mat)
{
	Matrix4 ret;

	// along a, down b
	ret[0] = (matrix[0] * mat[0]) + (matrix[1] * mat[4]) + (matrix[2] * mat[8]) + (matrix[3] * mat[12]);

	ret.matrix[1] = (matrix[0] * mat.matrix[1])
		+ (matrix[1] * mat.matrix[5])
		+ (matrix[2] * mat.matrix[9])
		+ (matrix[3] * mat.matrix[13]);

	ret.matrix[2] = (matrix[0] * mat.matrix[2])
		+ (matrix[1] * mat.matrix[6])
		+ (matrix[2] * mat.matrix[10])
		+ (matrix[3] * mat.matrix[14]);

	ret.matrix[3] = (matrix[0] * mat.matrix[3])
		+ (matrix[1] * mat.matrix[7])
		+ (matrix[2] * mat.matrix[11])
		+ (matrix[3] * mat.matrix[15]);

	ret.matrix[4] = (matrix[4] * mat.matrix[0])
		+ (matrix[5] * mat.matrix[4])
		+ (matrix[6] * mat.matrix[8])
		+ (matrix[7] * mat.matrix[12]);

	ret.matrix[5] = (matrix[4] * mat.matrix[1])
		+ (matrix[5] * mat.matrix[5])
		+ (matrix[6] * mat.matrix[9])
		+ (matrix[7] * mat.matrix[13]);

	ret.matrix[6] = (matrix[4] * mat.matrix[2])
		+ (matrix[5] * mat.matrix[6])
		+ (matrix[6] * mat.matrix[10])
		+ (matrix[7] * mat.matrix[14]);

	ret.matrix[7] = (matrix[4] * mat.matrix[3])
		+ (matrix[5] * mat.matrix[7])
		+ (matrix[6] * mat.matrix[11])
		+ (matrix[7] * mat.matrix[15]);

	ret.matrix[8] = (matrix[8] * mat.matrix[0])
		+ (matrix[9] * mat.matrix[4])
		+ (matrix[10] * mat.matrix[8])
		+ (matrix[11] * mat.matrix[12]);

	ret.matrix[9] = (matrix[8] * mat.matrix[1])
		+ (matrix[9] * mat.matrix[5])
		+ (matrix[10] * mat.matrix[9])
		+ (matrix[11] * mat.matrix[13]);

	ret.matrix[10] = (matrix[8] * mat.matrix[2])
		+ (matrix[9] * mat.matrix[6])
		+ (matrix[10] * mat.matrix[10])
		+ (matrix[11] * mat.matrix[14]);

	ret.matrix[11] = (matrix[8] * mat.matrix[3])
		+ (matrix[9] * mat.matrix[7])
		+ (matrix[10] * mat.matrix[11])
		+ (matrix[11] * mat.matrix[15]);

	ret.matrix[12] = (matrix[12] * mat.matrix[0])
		+ (matrix[13] * mat.matrix[4])
		+ (matrix[14] * mat.matrix[8])
		+ (matrix[15] * mat.matrix[12]);

	ret.matrix[13] = (matrix[12] * mat.matrix[1])
		+ (matrix[13] * mat.matrix[5])
		+ (matrix[14] * mat.matrix[9])
		+ (matrix[15] * mat.matrix[13]);

	ret.matrix[14] = (matrix[12] * mat.matrix[2])
		+ (matrix[13] * mat.matrix[6])
		+ (matrix[14] * mat.matrix[10])
		+ (matrix[15] * mat.matrix[14]);

	ret.matrix[15] = (matrix[12] * mat.matrix[3])
		+ (matrix[13] * mat.matrix[7])
		+ (matrix[14] * mat.matrix[11])
		+ (matrix[15] * mat.matrix[15]);

	return ret;
}


Vector3f Matrix4::operator*(Vector3f vec)
{
	Vector3f ret;

	//accross the Vector, down the matrix
	ret.x = (vec.x * matrix[0]) + (vec.y * matrix[4]) + (vec.z * matrix[8]) + matrix[12];
	ret.y = (vec.x * matrix[1]) + (vec.y * matrix[5]) + (vec.z * matrix[9]) + matrix[13];
	ret.z = (vec.x * matrix[2]) + (vec.y * matrix[6]) + (vec.z * matrix[10]) + matrix[14];

	return ret;
};

