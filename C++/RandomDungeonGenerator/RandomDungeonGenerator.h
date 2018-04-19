#pragma once

//
//	RandomDungeonGenerator.h by Chris Allen
//
//	This file is provided "as-is", for the sole purpose of a demonstration of my
//	work.  It is not intended to be copied or used in an external or third-party
//	project, and no support will be given for that use.
//
//	You may not use or copy this file, in whole or in part, to use for your own
//	projects.  All rights reserved over this file.
//

#include <NovaWinGLApp.h>

#include "RDGDungeon.h"

/**
 *	Application class for the RandomDungeonGenerator sample.

 *	RandomDungeonGenerator creates a single level, which contains
	a randomly generated dungeon layout.

 *	Layout generation uses a depth-first search algorithym
 */
class RandomDungeonGenerator : public NovaWinGLApp
{
public:
	RandomDungeonGenerator(int argc, char** argv);

	void run();
	void onReshape(int width, int height);
};