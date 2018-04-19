#pragma once

//
//	WindSim.h by Chris Allen
//
//	This file is provided "as-is", for the sole purpose of a demonstration of my
//	work.  It is not intended to be copied or used in an external or third-party
//	project, and no support will be given for that use.
//
//	You may not use or copy this file, in whole or in part, to use in your own, or
//	other projects.  All rights reserved over this file.
//

#include <NovaWinGLApp.h>

#include "WSAlphaStage.h"
#include "WSBetaStage.h"
#include "WSFinalStage.h"
#include "WSDemoStage.h"

/**
 *	Class for our Wind Sim Application

 *	Controls the main Window, Stage, and Application
 */
class WindSimApp : public NovaWinGLApp
{
public:
	WindSimApp(int argc, char** argv, string title);

	void run();
	void onReshape(int width, int height);
};

