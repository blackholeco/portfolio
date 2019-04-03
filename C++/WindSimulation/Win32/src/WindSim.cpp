/*
 *		WindSim.cpp
 *
 *	Author: Chris Allen
 *	Copyright Chris Allen 2018, all rights reserved
 *
 *	Committed at $Rev: 11 $ on $Date: 2019-02-27 13:13:59 +0000 (Wed, 27 Feb 2019) $
 *
 *	This file forms part of the Wind Simulation Project.
 *	It is intended to form part of my portfolio, for demonstration purposes ONLY.
 *
 *	You may NOT edit/alter this file in any way.
 *	You may NOT make any copies of this file for purposes other than its original intention (i.e. for demonstration purposes).
 *	You may NOT use or claim this file as your own work, either partially or wholly
 *
 *	This file is provided as-is.  No support will be provided for editing or using this file beyond its original intention.
 */

#include "WindSim.h"

using Nova::Colour;
using Nova::Math::Matrix;

using std::string;

namespace WindSim::Windows
{
	App::App(int argc, char** argv)
	{
		setup(argc, argv);
	}


	void App::onStart()
	{
		const float rads = 90.0f * static_cast<float>(M_PI) / 180.0f;

		getStage()->setViewMatrix(
			Matrix::view(
				50.0f * cosf(rads), 25, 50.0f * sinf(rads),
				0, 0, 0,
				0, 1, 0))
			.setBaseColour(Nova::Colour::BLACK);
	}
}


int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Nova::App::WindowSettings settings;

	settings.width = 1280;
	settings.height = 800;
	settings.title = "Wind Sim by Chris Allen - Powered by Nova";

	Nova::App::open(DBG_NEW WindSim::Windows::App(argc, argv), settings);

	Nova::App::getSceneManager().putScene("MAIN_SCENE", Nova::Scene_p(DBG_NEW WindSim::FinalScene()));

	Nova::App::getContext()->loadDefaultFont("Montserrat-Regular.ttf");

	Nova::App::start("MAIN_SCENE");

	return 0;
}
