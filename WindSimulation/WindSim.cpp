//
//	WindSim.cpp by Chris Allen
//
//	This file is provided "as-is", for the sole purpose of a demonstration of my
//	work.  It is not intended to be copied or used in an external or third-party
//	project, and no support will be given for that use.
//
//	You may not use or copy this file, in whole or in part, to use in your own, or
//	other projects.  All rights reserved over this file.
//


#include "WindSim.h"


WindSimApp::WindSimApp(int argc, char** argv, string title)
{
	initialize(argc, argv, title);

	nContext->getMainScene()->setBaseColour(NovaColour::BLACK);
}


void WindSimApp::onReshape(int width, int height)
{
	const float hWidth = static_cast<float>(width) / 2.0f;
	const float hHeight = static_cast<float>(height) / 2.0f;

	nContext->getMainScene()->resize(width, height);

	nContext->getMainScene()->setProjectionMatrix(
		NovaMatrixUtil::ortho(-hWidth, hWidth, -hHeight, hHeight, 1, 2)
	);
}


void WindSimApp::run()
{
	nContext->setStage(DBG_NEW WSDemoStage);

	nContext->getMainScene()->setViewMatrix(
		NovaMatrixUtil::view(0, 0, 1, 0, 0, 0, 0, 1, 0)
	);

	nContext->setupScene();
}


int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	NovaWindowsApp::open(DBG_NEW WindSimApp(argc, argv, "Wind Sim by Chris Allen - Developed with Nova"));

	return 0;
}

