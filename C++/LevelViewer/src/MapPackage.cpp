/*
 *	MapPackage.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "MapPackage.h"


MapPackage::MapPackage()
{
	layout = nullptr;
	Release();
}


MapPackage::MapPackage(const MapPackage& orig)
{
	layout = nullptr;
	Release();

	numcolumns = orig.numcolumns;

	numrows = orig.numrows;

	layout = new char*[numrows];
	assert(layout);

	for (unsigned int i = 0; i < numrows; i++)
	{
		layout[i] = new char[numcolumns];

		for (unsigned int j = 0; j < numcolumns; j++)
			layout[i][j] = orig.layout[i][j];
	}
}


MapPackage::~MapPackage()
{
	Release();
}


void MapPackage::operator =(const MapPackage& orig)
{
	numcolumns = orig.numcolumns;

	numrows = orig.numrows;

	layout = new char*[numrows];
	assert(layout);

	for (unsigned int i = 0; i < numrows; i++)
	{
		layout[i] = new char[numcolumns];

		for (unsigned int j = 0; j < numcolumns; j++)
			layout[i][j] = orig.layout[i][j];
	}
}


char MapPackage::GetCharAt(unsigned int y, unsigned int x) const
{
	return layout[y][x];
}


string MapPackage::GetMapName() const
{
	return string(packagename);
}


void MapPackage::Release()
{
	memset(&packagename, 0, MAX_NAME_LENGTH);

	if (layout)
	{
		for (unsigned int i = 0; i < numrows; i++)
		{
			delete[] layout[i];
			layout[i] = nullptr;
		}

		if (layout != NULL)
		{
			delete[] layout;
			layout = nullptr;
		}
	}

	layout = nullptr;
	numcolumns = numrows = 0;
}


unsigned long MapPackage::GetColumns() const
{
	return numcolumns;
}


unsigned long MapPackage::GetRows() const
{
	return numrows;
}


void MapPackage::Import()
{

}


bool MapPackage::LoadPackage(const string filename)
{
	ifstream stream;
	MAPPACKAGE package;

	if (filename.find(".bom") == string::npos)
		throw runtime_error("MAP ERROR: File is not of correct format (.bom)");

	stream.open(filename, ios::binary);

	if (!stream.is_open())
		throw runtime_error("MAP ERROR : Could not open : " + filename);

	memset(&package, 0, sizeof(MAPPACKAGE));

	stream.read((char*)&package, sizeof(MAPPACKAGE));

	strcpy_s(packagename, package.packagename);

	numcolumns = package.numcolumns;
	numrows = package.numrows;

	assert(numcolumns * numrows > 0);

	layout = new char*[numrows];

	assert(layout);

	for (unsigned int i = 0; i < numrows; i++)
	{
		layout[i] = new char[numcolumns];

		stream.read(layout[i], numcolumns);
	}

	stream.close();

	return true;
}
