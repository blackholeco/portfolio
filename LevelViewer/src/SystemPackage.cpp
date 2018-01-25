/*
 *	SystemPackage.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "SystemPackage.h"


SystemPackage::SystemPackage()
{
	memset(&syspackage, 0, sizeof(SYSPACKAGE));

	Release();
}


void SystemPackage::Release()
{
	for (shared_ptr<Texture> texture : playerTexture)
	{
		if (texture != nullptr)
			texture->Release();
	}

	for (shared_ptr<Texture> texture : bombTexture)
	{
		if (texture != nullptr)
			texture->Release();
	}

	if (explosiontex != nullptr)
		explosiontex->Release();

	if (puspeedtex != nullptr)
		puspeedtex->Release();

	if (pubombtex != nullptr)
		pubombtex->Release();

	if (puexptex != nullptr)
		puexptex->Release();

	if (playermesh != nullptr)
		playermesh->Release();

	if (bombmesh != nullptr)
		bombmesh->Release();

	if (pickupmesh != nullptr)
		pickupmesh->Release();
}


SYSPACKAGE SystemPackage::GetPackageContents()
{
	return syspackage;
}


void SystemPackage::Import()
{
	// now create the system package struct
	for (shared_ptr<Texture> texture : playerTexture)
		glInterface.BuildTexture(texture);

	for (shared_ptr<Texture> texture : bombTexture)
		glInterface.BuildTexture(texture);

	glInterface.BuildTexture(explosiontex);
	glInterface.BuildTexture(puspeedtex);
	glInterface.BuildTexture(pubombtex);
	glInterface.BuildTexture(puexptex);

	glInterface.BuildModel(playermesh);
	glInterface.BuildModel(bombmesh);
	glInterface.BuildModel(pickupmesh);
}


bool SystemPackage::LoadPackage(const string package)
{
	ifstream stream;

	if (package.find(".game") == string::npos)
	{
		cout << "ERROR: System Package filetype incorrect" << endl << "Should be .game" << endl;
		return false;
	}

	stream.open(package, ios::binary);

	if (!stream.is_open())
		return false;

	Model::Unpack(stream, playermesh);

	Model::Unpack(stream, bombmesh);

	Model::Unpack(stream, pickupmesh);

	for (shared_ptr<Texture>& texture : playerTexture)
		Texture::Unpack(stream, texture);

	for (shared_ptr<Texture>& texture : bombTexture)
		Texture::Unpack(stream, texture);

	Texture::Unpack(stream, explosiontex);

	Texture::Unpack(stream, puexptex);
	Texture::Unpack(stream, pubombtex);
	Texture::Unpack(stream, puspeedtex);

	stream.close();

	return true;
}

