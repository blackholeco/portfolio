//
//  Created by Chris Allen
//

#include "WSFinalScene.h"

using Nova::Math::Random;
using Nova::Math::Transform;

using std::shared_ptr;

namespace WindSim
{
	void FinalScene::setup(Nova::Context_p& context, Nova::MainStage_p& mainStage)
	{
		shaderFile = "WindFinal.glsl";

		loadResources(context);

		createScenes(context, mainStage);

		// Create our terrain - Blades in X, Blades in Y, Dimensions of field, culling
		createGrass(100, 100, 50, 50, 0.6f);

		setupWind();
	}


	Transform FinalScene::setupBlade(unsigned int x, unsigned int y)
	{
		Transform ret;

		float bHeight = Random::randomFloat(0.75f, 1.00f);

		ret.setRotation(
			Random::randomFloat(-55.0f, 55.0f),
			0, 1, 0);

		float offset = Random::randomFloat(-0.25f, 0.25f);

		ret.setPosition(offset, 0, offset);

		ret.setScale(1, bHeight, 1);

		return ret;
	}
}
