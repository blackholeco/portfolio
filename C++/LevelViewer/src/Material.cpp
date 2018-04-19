/*
 *	Material.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "Material.h"


Material::Material()
{
	mTextureRef = -1;
	mShine = 0.0f;
	mSpecular.fill(0);
}


Material::Material(const Material& orig)
{
	*this = orig;
}


Material::~Material()
{
	if (mTexture != nullptr)
		mTexture->Release();
}


void Material::operator=(const Material& orig)
{
	mTextureRef = orig.mTextureRef;
	mShine = orig.mShine;

	mSpecular = orig.mSpecular;

	mTexture = orig.mTexture;
}


void Material::Create(const float sne, const array<float, 3> spec, const unsigned int ref)
{
	mShine = sne;

	mSpecular = spec;

	mTextureRef = ref;
}


float Material::GetShine()
{
	return mShine;
}


array<float, 3>& Material::GetSpecular()
{
	return mSpecular;
}


bool Material::Unpack(ifstream& stream)
{
	MATERIAL material;

	assert(stream.is_open());

	Texture::Unpack(stream, mTexture);

	stream.read((char*)&material, sizeof(MATERIAL));

	mShine = material.shine;

	mSpecular[0] = material.specular[0];
	mSpecular[1] = material.specular[1];
	mSpecular[2] = material.specular[2];

	return true;
}

