/*
 *	Texture.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "Texture.h"

Texture::Texture()
{
	data = nullptr;
	Release();
}


Texture::~Texture()
{
	Release();
};


void Texture::operator=(const Texture& orig)
{
	imgWidth = orig.imgWidth;
	imgHeight = orig.imgHeight;
	imgId = orig.imgId;

	data = new char[imgWidth * imgHeight * 3];
	assert(data);

	memcpy_s(data, imgWidth * imgHeight * 3, orig.data, imgWidth * imgHeight * 3);
}


long Texture::GetTextureId() const
{
	return imgId;
}


void Texture::Release()
{
	if (data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}

	imgWidth = 0;
	imgHeight = 0;
}


bool Texture::Unpack(ifstream& stream, shared_ptr<Texture>& texture)
{
	texture.reset(new Texture());

	return texture->Unpack(stream);
}


bool Texture::Unpack(ifstream& stream)
{
	TEXTURE tmpTexture;

	assert(stream.is_open());

	memset(&tmpTexture, 0, sizeof(TEXTURE));

	Release();

	stream.read((char*)&tmpTexture, sizeof(TEXTURE));

	imgWidth = tmpTexture.imgwidth;
	imgHeight = tmpTexture.imgheight;

	assert(imgWidth * imgHeight > 0);

	data = new char[imgWidth * imgHeight * 3];
	assert(data);

	stream.read(data, imgWidth * imgHeight * 3);

	return true;
}

