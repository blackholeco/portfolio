/*
 *	AreaPackage.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include "Package.h"

/*
 *	Class to handle the loading, storing and writing of Area Package data

 *	Areas are like themes.  When a Level is created, an Area is applied to
	a Map, assigning its graphical resources to be used.  This means that the
	Map looks different across different sessions despite having the same layout
 */
class AreaPackage : public Package
{
public:
	AreaPackage();
	AreaPackage(const AreaPackage& orig);

	void operator =(const AreaPackage& orig);

	/**
	 *	Delete all data stored by this Package.  This leaves the Package
		in an unusable state, so only call Release() when you don't need
		this Package anymore
	 */
	void Release();

	/**
	 *	Reads an Area Package file into memory

	 *	@param filename : Path/name of the package file to load

	 *	@return true if package is successfully loaded

	 *	@throws Runtime Error : Incorrect package format in filename, or unable to open the package

	 *	@throws Bad Alloc Error	: Unable to allocate memory for textures
	 */
	bool LoadPackage(const string filename);

	/**
	 *	Build all resources stored by this Package into graphics
	 */
	void Import();

protected:
	friend class GameMap;

	shared_ptr<Texture> floortex;
	shared_ptr<Model> outerWall;
	shared_ptr<Model> innerWall;
};
