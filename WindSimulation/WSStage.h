#pragma once

#include <NovaStage.h>
#include <NovaContext.h>
#include <NovaParticle.h>
#include <NovaSprite.h>
#include <NovaPackage.h>

/**
 *	Base class for our Wind Sim Stages
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
