/*
 *	Vector2.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include <sstream>

using std::string;
using std::stringstream;

/**
 *	Class to represent a 2-dimensional direction, or point in space
 */
template <typename T>
class Vector2
{
public:
	Vector2(void)
	{
		x = 0;
		y = 0;
	};

	Vector2(T tx, T ty)
	{
		x = tx;
		y = ty;
	};

	/**
	 *	Get this Vector2's information as a string

	 *	@return This Vector2's data as a string
	 */
	string toString()
	{
		stringstream s;

		s << x << ',' << y;

		return s.str();
	}

	/**
	 *	Get the length of the Vector

	 *	@return the length of this Vector
	 */
	float magnitude() const
	{
		return sqrt((float)(x * x) + (float)(y * y));
	}


	Vector2<T> operator +(const Vector2<T>& a)
	{
		return Vector2<T>(x + a.x, y + a.y);
	};


	Vector2<T>& operator +=(const Vector2<T> a)
	{
		x += a.x;
		y += a.y;

		return *this;
	}

	template<typename S>
	Vector2<T> operator=(Vector2<S> a)
	{
		x = a.x;
		y = a.y;

		return *this;
	}

	T x;
	T y;
};

template<typename T>
Vector2<T> operator +(Vector2<T> a, Vector2<T> b)
{
	return Vector2<T>(a.x + b.x, a.y + b.y);
};

template<typename T, typename S>
Vector2<T> operator +(Vector2<T> a, Vector2<S> b)
{
	return Vector2<T>(a.x + b.x, a.y + b.y);
};

template<typename T>
Vector2<T> operator -(Vector2<T> a, Vector2<T> b)
{
	return Vector2<T>(a.x - b.x, a.y - b.y);
};

template<typename T, typename S>
Vector2<T> operator -(Vector2<T> a, Vector2<S> b)
{
	return Vector2<T>(a.x - b.x, a.y - b.y);
};

template<typename T>
bool operator ==(Vector2<T> a, Vector2<T> b)
{
	return a.x == b.x && a.y == b.y;
};

template<typename T>
Vector2<T> operator *(Vector2<T> a, T b)
{
	return Vector2<T>(a.x*b, a.y*b);
};

/**
 *	Class to represent a 3-dimensional direction or point
 */
template <typename T>
class Vector3
{
public:
	Vector3()
	{
		x = y = z = 0;
	}

	Vector3(const T tx, const T ty, const T tz)
	{
		x = tx;
		y = ty;
		z = tz;
	}

	/**
	 *	Get this Vector3's data as a string

	 *	@return A string containing this Vector3's data
	 */
	string toString()
	{
		stringstream ret;

		ret << x << ',' << y << ',' << z;

		return ret.str();
	}

	/**
	 *	Get the angle between two Vectors

	 *	@param a : First vector to find the angle between
	 *	@param b : Second vector to find the angle between

	 *	@return the angle between a and b
	 */
	static float angle(const Vector3<T> a, const Vector3<T> b)
	{
		float cosx = 0;

		if (a == b)
			return 0;

		// normalizing the vectors before getting the dot product simply
		// gives us cos(x) as a.b == |a|*|b| * cos(x).  Normalizing the
		// vectors gives us magnitudes of 1, so a.b = cos(x)
		cosx = dotProduct(normalize(a), normalize(b));

		return acos(cosx);
	}

	/**
	 *	Get the dot product of two Vectors

	 *	@param a : Left hand side Vector for the Dot Product
	 *	@param b : Right hand side Vector for the dot product

	 *	@return the dot product of a and b
	 */
	static float dotProduct(const Vector3<T> a, const Vector3<T> b)
	{
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}

	/**
	 *	Get the cross product of two vectors

	 *	@param a : The left hand vector in the cross product
	 *	@param b : The right hand vector in the cross product

	 *	@return a Vector representing the cross product of a and b
	 */
	static Vector3<T> cross(Vector3<T> a, Vector3<T> b)
	{
		Vector3<T> ret;

		ret.x = (a.y * b.z) - (a.z * b.y);
		ret.y = (a.z * b.x) - (a.x * b.z);
		ret.z = (a.x * b.y) - (a.y * b.x);

		return ret;
	}

	/**
	 *	Normalize the input Vector.  A Normalized Vector has length 1

	 *	@param orig : The original Vector to normalize

	 *	@return a new Vector which represents the input Vector normalized
	 */
	static Vector3<T> normalize(const Vector3<T> orig)
	{
		float magnitude;

		magnitude = orig.magnitude();

		if (magnitude == 0 || magnitude == 1)
			return orig;

		return Vector3<T>(orig.x / magnitude, orig.y / magnitude, orig.z / magnitude);
	}

	/**
	 *	Get the magnitude/length of a Vector

	 *	@param orig : The Vector to find the length of

	 *	@return The magnitude/length of the input Vector
	 */
	static float magnitude(const Vector3<T> orig)
	{
		return sqrtf((float)(orig.x*orig.x) + (float)(orig.y * orig.y) + (float)(orig.z * orig.z));
	}

	/**
	 *	Get the length of this Vector

	 *	@return the length of this Vector
	 */
	float magnitude() const
	{
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	T x;
	T y;
	T z;
};


template<typename T>
Vector3<T>& operator+(Vector3<T> a, Vector3<T> b)
{
	return Vector3<T>(a.x + b.x, a.y + b.y, b.z + b.z);
}


template<typename T>
Vector3<T> operator -(Vector3<T> a, Vector3<T> b)
{
	return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
};


template<typename T>
bool operator==(Vector3<T> a, Vector3<T> b)
{
	return a.y == b.x && a.y == b.y && a.z == b.z;
}


typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;

