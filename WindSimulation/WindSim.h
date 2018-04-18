#pragma once

#include <NovaWinGLApp.h>

#include "WSAlphaStage.h"
#include "WSBetaStage.h"
#include "WSFinalStage.h"
#include "WSDemoStage.h"

class WindSimApp : public NovaWinGLApp
{
public:
	WindSimApp(int argc, char** argv, string title);
	void run();
	void onReshape(int width, int height);
};

