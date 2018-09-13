//
// Created by Chris Allen on 21/03/2018.
//

#include "WSAndroidApp.h"

#include "WSFinalScene.h"

using Nova::Math::MatrixUtil;
using Nova::Colour;

namespace WindSim
{
	namespace Android
	{
		App::App(JNIEnv* env, jobject assets)
		{
			setup(env, assets);
		}


		void App::onReshape(int width, int height)
		{
			const float hWidth = static_cast<float>(width) / 2.0f;
			const float hHeight = static_cast<float>(height) / 2.0f;

			getStage()->resize(width, height);

			getStage()->setProjectionMatrix(
					Nova::Math::MatrixUtil::ortho(-hWidth, hWidth, -hHeight, hHeight, 1, 2)
			);
		}

		void App::onStart()
		{
			getStage()->setViewMatrix(
					Nova::Math::MatrixUtil::view(0, 0, 1, 0, 0, 0, 0, 1, 0)
			);

			getStage()->setBaseColour(Nova::Colour::BLACK);

			getTimer().start();
		}
	}
}
