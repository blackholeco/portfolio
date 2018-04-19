#pragma once

//
//	WSStage.h by Chris Allen
//
//	This file is provided "as-is", for the sole purpose of a demonstration of my
//	work.  It is not intended to be copied or used in an external or third-party
//	project, and no support will be given for that use.
//
//	You may not use or copy this file, in whole or in part, to use in your own, or
//	other projects.  All rights reserved over this file.
//

#include <NovaStage.h>
#include <NovaContext.h>
#include <NovaParticle.h>
#include <NovaSprite.h>
#include <NovaPackage.h>

/**
 *	Base class for our Wind Sim Stages

 *	The base Stage contains the field of grass, the wind particle effect, and the rendertargets
	required by the program.

 *	This class allows for different stages of development, while maintaining
	code that is shared between each stage
 */
class WSStage : public NovaStage
{
public:
	WSStage();

	void update(long millis);

	void release();

	virtual void addToScene();

protected:
	void createScenes();
	void createGrass(unsigned int bladesX, unsigned int bladesY, unsigned int width, unsigned int height, float cull = 0.0f);
	void loadResources();

	virtual NovaTransform setupBlade(unsigned int x, unsigned int y) = 0;

	void setupWind();

	void updateCamera();
	void updateEmitter();

	float cameraAngle;

	float emitterAngle;

	float vCamera;
	float vEmitter;

	// Render target for our wind
	nSubScene wind;
	nSubScene terrainScene;

	NovaPackage mPackage;

	nImage terrainTex;
	nImage windParticle;
	nImage windImage;

	Model terrain;

	NovaSprite windOrigin;
	
	Custom grass;

	NovaParticleGun windEmitter;

	NovaTransform wOTrans;
	NovaTransform windSceneTrans;
	NovaTransform mainSceneTrans;

	float power;
	const float rads;

	bool cUpdate;
	bool eUpdate;

	// Used for calculating framerate
	unsigned int frameCount;
	float timer;

	string shaderFile;
};
