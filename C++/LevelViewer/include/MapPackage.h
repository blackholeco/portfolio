/*
 *	MapPackage.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include "Package.h"

/*
 *	Struct used for storing, passing and writing Map Packge information
 */
typedef struct _mappackage
{
	char packagename[MAX_NAME_LENGTH + 1];
	unsigned long numcolumns;
	unsigned long numrows;
} MAPPACKAGE;

/*
 *	Class to handle the loading, writing and storing of Map Package data.

 *	Map Packages combine with Area Packages to create a Level
 */
class MapPackage : public Package
{
public:
	MapPackage();
	MapPackage(const MapPackage&);
	~MapPackage();

	void operator=(const MapPackage&);

	/**
	 *	Get the character at a point in the layout array

	 *	@param y : y co-ordinate to sample the layout at
	 *	@param x : x co-ordinate to sample the layout at

	 *	@return the charater from the sampled layout location
	 */
	char GetCharAt(const unsigned int y, const unsigned int x) const;

	/**
	 *	Get the Map's name

	 *	@return the Map's name
	 */
	string GetMapName() const;

	/**
	 *	Delete all data that this Package stores.  This leaves the Package unusable,
		so only call Release when you no longer need the Package
	 */
	void Release();

	/**
	 *	Get the number of columns in the layout array

	 *	@return the number of columns in the layout array
	 */
	unsigned long GetColumns() const;

	/**
	 *	Get the number of rows in the layout array

	 *	@return the number of rows in the layout array
	 */
	unsigned long GetRows() const;

	/**
	 *	Maps are a special case.  Since they store no resources, they do not need
		importing.  Therefore this implementation does nothing
	 */
	void Import();

	/**
	 *	Loads a map package into memory

	 *	@param filename : Path and name of the package file to load

	 *	@return true if the package is successfully loaded

	 *	@throws	Runtime Error:	Map could not be opened or file is not of correct format
	 */
	bool LoadPackage(const string filename);

protected:
	friend class GameMap;

	unsigned long numcolumns;
	unsigned long numrows;

	char** layout;
};