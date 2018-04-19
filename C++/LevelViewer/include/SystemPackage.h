/*
 *	SystemPackage.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include <iostream>

#include "Package.h"

using std::cout;
using std::endl;


/*
 *	Struct to hold all IDS/references for System Package resources
 */
typedef struct _syspackage
{
	array<unsigned int, 4> playertex;
	array<unsigned int, 4>  bombtex;
	unsigned int explosion;
	array<unsigned int, 3>  pickuptex;

	unsigned int playermesh;
	unsigned int bombmesh;
	unsigned int pickupmesh;
} SYSPACKAGE;

/*
 *	Package that contains all resources that are reused in every map,
	such as Player Models
 */
class SystemPackage : public Package
{
public:
	SystemPackage();

	/**
	 *	Get a copy of the resources stored by this Package

	 *	@return A SYSPACKAGE containing references to the resources stored
	 */
	SYSPACKAGE GetPackageContents();

	/**
	 *	Import the resources stored by this Package into graphics
	 */
	void Import();

	/**
	 *	Load this Package's data from a Package file

	 *	@param filename : Path and name of the Package file

	 *	@return true if Package is loaded successfully
	 */
	bool LoadPackage(const string filename);

	/**
	 *	Delete all data stored by this Package. The Package will not be
		usable after calling Release(), so this should only be used when
		you no longer need the Package
	 */
	void Release();

protected:
	friend class GameMap;

	array<shared_ptr<Texture>, 4> playerTexture;
	array<shared_ptr<Texture>, 4> bombTexture;

	shared_ptr<Texture> explosiontex;
	shared_ptr<Texture> puspeedtex;
	shared_ptr<Texture> pubombtex;
	shared_ptr<Texture> puexptex;

	shared_ptr<Model> playermesh;
	shared_ptr<Model> bombmesh;
	shared_ptr<Model> pickupmesh;

	SYSPACKAGE syspackage;
};

