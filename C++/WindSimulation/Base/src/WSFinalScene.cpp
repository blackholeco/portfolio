/*
 *		WSFinalScene.h
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

#include "WSFinalScene.h"

using Nova::Math::Random;
using Nova::Math::Transform;

namespace WindSim
{
	void FinalScene::setupField()
	{
		shaderFile = "WindFinal.glsl";

		// Create our terrain - Blades in X, Blades in Y, Dimensions of field, culling
		createGrass(125, 125, 50, 50, 0.6f);
	}


	Transform FinalScene::setupBlade(unsigned int x, unsigned int y)
	{
		Transform ret;

		const float bHeight = Random::randomFloat(0.5f, 1.00f);

		const float bWidth = Random::randomFloat(0.5f, 1.0f);

		ret.setRotation(
			Random::randomFloat(-55.0f, 55.0f),
			0, 1, 0);

		float offset = Random::randomFloat(-0.25f, 0.25f);

		ret.setPosition(offset, 0, offset);

		ret.setScale(bWidth, bHeight, 1);

		return ret;
	}
}
