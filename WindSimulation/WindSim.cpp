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
	nContext->setStage(DBG_NEW WSFinalStage);

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

