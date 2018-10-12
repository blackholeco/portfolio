#pragma once

/*
 *		WSStage.h
 *
 *	Author: Chris Allen
 *	Copyright Chris Allen 2018, all rights reserved
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
