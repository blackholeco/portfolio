//
//	RandomDungeonGenerator.cpp by Chris Allen
//
//	This file is provided "as-is", for the sole purpose of a demonstration of my
//	work.  It is not intended to be copied or used in an external or third-party
//	project, and no support will be given for that use.
//
//	You may not use or copy this file, in whole or in part, to use for your own
//	projects.  All rights reserved over this file.
//

#include "RandomDungeonGenerator.h"


RandomDungeonGenerator::RandomDungeonGenerator(int argc, char** argv)
{
	initialize(argc, argv, "Random Dungeon Generator by Chris Allen");
}


void RandomDungeonGenerator::onReshape(int width, int height)
{
	nContext->getMainScene()->resize(width, height);
	nContext->getMainScene()->setProjectionMatrix(
		NovaMatrixUtil::ortho(-10.5f, 10.5f, -10.5f, 10.5f, 1, 200)
	);
}


void RandomDungeonGenerator::run()
{
	nContext->setStage(DBG_NEW RDGDungeon(20, 20));

	nContext->getMainScene()->setViewMatrix(
		NovaMatrixUtil::view(0, 25.0f, 0, 0, 0, 0, 0, 1, 0));
	nContext->getMainScene()->setBaseColour(NovaColour::BLACK);
	nContext->setupScene();
}


int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	NovaWindowsApp::open(DBG_NEW RandomDungeonGenerator(argc, argv));

	return 0;
}

