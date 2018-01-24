/*
 *	WSDemoStage.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "WSDemoStage.h"

/**
 *	Create our world, loading resources, setting up render targets,
	and building our terrain and grass.
 */
void WSDemoStage::setupStage()
{
	shaderFile = "WindDemo.glsl";

	loadResources();

	createScenes();

	// Create our terrain - Blades in X, Blades in Z, Dimensions of field
	createGrass(100, 100, 50, 50, 0.6f);

	// Setup our wind effect
	setupWind();
}

/**
 *	For each Blade, set up a transformation to apply any variation to
	the Blade's appearance

 *	@param x : Original x position of the blade in our scene
 *	@param y : Original y position of the blade in our scene

 *	@return : A Model Transform matrix to transform the blade of grass with
 */
NovaTransform WSDemoStage::setupBlade(unsigned int x, unsigned int y)
{
	NovaTransform ret;

	ret.setScale(1, NovaRandom::randomFloat(0.8, 1.0f), 1)
		.setRotation(NovaRandom::randomFloat(-55.0f, 55.0f),
			0, 1, 0)
		.setPosition(NovaRandom::randomFloat(-0.25f, 0.25), 0, NovaRandom::randomFloat(-0.25f, 0.25));

	return ret;
}

/**
 *	Add all objects to the scene for rendering
 */
void WSDemoStage::addToScene()
{
	// Render the wind effect to render target
	windEmitter.addToScene(wind);

	// Render wind target
	wind->render(nContext->getRenderer());

	// Add the terrain and grass to the terrain scene target
	terrainScene->add({ &terrain, &grass });

	// Arrow graphic showing wind direction
	windOrigin.addToScene(wOTrans, terrainScene);

	// Render main scene target
	terrainScene->render(nContext->getRenderer());

	// Add Main Scene and wind targets to the display for rendering
	terrainScene->addToScene(mainSceneTrans, nContext->getMainScene());

	// Shows the wind's render target contents
	wind->addToScene(windSceneTrans, nContext->getMainScene());
}
