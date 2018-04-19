/*
 *	AreaPackage.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "AreaPackage.h"


AreaPackage::AreaPackage()
{
	Release();
}


AreaPackage::AreaPackage(const AreaPackage& orig)
{
	floortex = orig.floortex;

	outerWall = orig.outerWall;
	innerWall = orig.innerWall;
}


void AreaPackage::operator=(const AreaPackage& orig)
{
	floortex = orig.floortex;
	outerWall = orig.outerWall;
	innerWall = orig.innerWall;
}


void AreaPackage::Release()
{
	memset(&packagename, 0, MAX_NAME_LENGTH);

	if (floortex != nullptr)
		floortex->Release();

	if (outerWall != nullptr)
		outerWall->Release();

	if (innerWall != nullptr)
		innerWall->Release();
}


void AreaPackage::Import()
{
	glInterface.BuildTexture(floortex);

	glInterface.BuildModel(outerWall);
	glInterface.BuildModel(innerWall);
}


bool AreaPackage::LoadPackage(const string filename)
{
	ifstream stream;

	if (filename.find(".area") == string::npos)
		throw runtime_error("FILE ERROR: Incorrect package format for area : " + filename);

	stream.open(filename, ios::binary);

	if (!stream.is_open())
		throw runtime_error("FILE ERROR : Unable to open " + filename);	// forces return if exception not caught

	int length;

	stream.read(reinterpret_cast<char*>(&length), sizeof(int));

	stream.read(packagename, length);

	Texture::Unpack(stream, floortex);

	Model::Unpack(stream, outerWall);
	Model::Unpack(stream, innerWall);

	stream.close();

	return true;
};

