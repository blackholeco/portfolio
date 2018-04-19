/*
 *	Texture.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include <fstream>
#include <stdexcept>
#include <cassert>
#include <string>
#include <memory>

using std::ifstream;
using std::runtime_error;
using std::ios;
using std::string;
using std::shared_ptr;

/*
 *	Struct for holding and passing around (imported) Texture data
 */
typedef struct _texture
{
	unsigned long imgwidth;		// if these are 0, use default texture for Material
	unsigned long imgheight;
	unsigned long id;			// this texture's identifier
} TEXTURE;

/*
 *	Class to handle loading and storing images into memory
 */
class Texture
{
public:
	Texture();
	~Texture();

	/**
	 *	Copy one Texture to another
	 */
	void operator=(const Texture&);

	long GetTextureId() const;

	/**
	 *	Deletes all data stored by this Texture.  This will leave the Texture
		in an unusable state, so only call Release() if you have no further need
		for this Texture
	 */
	void Release();

	/**
	 *	Load this Texture from a Package file.  The Texture should be
		the next thing to read from the stream

	 *	@param stream :	File stream open to the package to read

	 *	@return true if the texture is loaded successfully
	 */
	bool Unpack(ifstream& stream);

	/**
	 *	Static version of the Unpack method

	 *	@param stream : A filestream which is already open to the package file containing this Model's data
	 *	@param model : The Model to unpack the data to
	 */
	static bool Unpack(ifstream& stream, shared_ptr<Texture>& texture);

protected:
	friend class GLInterface;

	char* data;
	unsigned long imgWidth;
	unsigned long imgHeight;
	unsigned int imgId;
};

