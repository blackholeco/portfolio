/*
 *	Package.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include "GLInterface.h"

// Package : An overridable object to define types of packages (map, e.g.)
#ifndef MAX_NAME_LENGTH
#define MAX_NAME_LENGTH 21
#endif

/**
 *	Base (abstract) class for all Packages defined by this project

 *	Packages will handle the loading, writing and storing of their relevant data
 */
class Package
{
public:
	Package() {};
	/**
	 *	Load this Package's contents from a Package file

	 *	@param filename : Path and name of the Package file to read

	 *	@return true if the Package loads successfully
	 */
	virtual bool LoadPackage(const string filename) = 0;

	/**
	 *	Delete all data stored by the Package.  This will make the Package
		unusable, so Release() should only be called when the Package is no
		longer required
	 */
	virtual void Release() = 0;

protected:
	char packagename[MAX_NAME_LENGTH];
};
