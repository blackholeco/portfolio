/*
 *	Material.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include <array>

#include "Texture.h"

using std::array;

/* Struct to simplify Material loading and saving to package */
typedef struct _material
{
	float shine;
	array<float, 3> specular;
} MATERIAL;

/*
 *	Class to handle Material data for 3D models

 *	Manages the reading, storing and writing of Material data

 *	Materials define what a Model is made of, altering how it looks when
	rendered.  Currently, this is a simple version which handles a diffuse
	Texture, and defines the shine and specular of the Material
 */
class Material
{
public:
	Material();
	Material(const Material& orig);
	~Material();

	void operator =(const Material& orig);

	/**
	 *	Get the specular colour of this Material

	 *	@return The specular colour of this Material
	 */
	array<float, 3>& GetSpecular();

	/**
	 *	Get the level of shine for this Material

	 *	@return The shininess of this Material
	 */
	float GetShine();

	/**
	 *	Set up this Material from input information

	 *	@param shine : Shininess of the Material
	 *	@param specular : Specular colour of the Material
	 *	@param texture : Reference of the diffuse Texture for this Material
	 */
	void Create(const float shine, const array<float, 3> specular, const unsigned int texture);

	/**
	 *	Read this Material from a Package file.  The next thing to be read from the
		input stream should be this Material

	 *	@param stream : A filestream open to the Package to read

	 *	@return true if the Material loads successfully
	 */
	bool Unpack(ifstream& stream);

protected:
	friend class GLInterface;

	unsigned int mTextureRef;
	float mShine;
	array<float, 3> mSpecular;

	shared_ptr<Texture> mTexture;
};

