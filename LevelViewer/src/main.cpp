/*
 *	main.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include <iostream>

#include "GameMap.h"
#include "Camera.h"

static GameMap gameMap;


void reshape(int w, int h)
{
	gameMap.onReshape(w, h);
}


void render()
{
	gameMap.Draw();

	glutPostRedisplay();
}


/**
 *	Loads System, Area and Map Packages, and displays a demo of the map
 */
int main(int argc, char** argv)
{
	glInterface.Initialize(argc, argv);

	if (argc != 7)	// Check for incorrect usage
	{
		cout << "Usage: " << endl
			<< "LevelViewer -s <system package> -a <area package> -m <map package>" << endl
			<< "Where package arguments are filenames of the packages used to create a level" << endl;

		system("pause");
		return 0;
	}

	// otherwise, load the packages
	SystemPackage system;
	AreaPackage area;
	MapPackage mMap;


	for (int i = 0; i < 3; i++)
	{
		if (strncmp(argv[(i * 2) + 1], "-a", 2) == 0)
		{
			if (!area.LoadPackage(argv[(i * 2) + 2]))
				return -1;
		}
		else if (strncmp(argv[(i * 2) + 1], "-s", 2) == 0)
		{
			// Check we haven't loaded this already
			if (!system.LoadPackage(argv[(i * 2) + 2]))
				return -1;
		}

		else if (strncmp(argv[(i * 2) + 1], "-m", 2) == 0)
		{
			if (!mMap.LoadPackage(argv[(i * 2) + 2]))
				return -1;
		}
	}

	string title = "LevelViewer - ";

	title.append(mMap.GetMapName());

	// Open Level Window
	if (!glInterface.createWindow(title))
	{
		cout << "Error Initializing GLEW" << endl;
		return -1;
	}

	glutReshapeFunc(reshape);
	glutDisplayFunc(render);

	// Should import all resources into OpenGL
	gameMap.InitializeMap(mMap, area, system);

	glutMainLoop();

	return 0;
}