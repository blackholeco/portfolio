/*
 *		WSScene.cpp
 *
 *	Author: Chris Allen
 *	Copyright Chris Allen 2018, all rights reserved
 *
 *	Committed at $Rev: 18 $ on $Date: 2019-04-02 15:52:59 +0100 (Tue, 02 Apr 2019) $
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

#include "WSScene.h"

using Nova::Colour;
using Nova::Input_p;
using Nova::Model;
using Nova::SimpleActor;

using namespace Nova::Math;

using std::shared_ptr;

namespace WindSim
{
	Scene::Scene()
		: rads(static_cast<float>(M_PI) / 180.0f)
	{
		cameraAngle = 90;
		emitterAngle = 90;
		vCamera = 0;
		vEmitter = 0;
		dPower = 0;

		power = 0.9f;

		eUpdate = false;
		cUpdate = false;

		shaderFile = "wind.glsl";
	}


	void Scene::resize(Nova::MainStage_p& mainStage, int width, int height)
	{
		auto fWidth = static_cast<float>(width);
		auto fHeight = static_cast<float>(height);

		mainStage->setProjectionMatrix(
			Matrix::perspective(45, fWidth / fHeight, 1, 150)
		);
	}


	void Scene::createScenes(const Nova::Context_p& context, Nova::MainStage_p& mainStage)
	{
		// Render texture to store the wind effect
		wind = context->getStageFactory()->newSubStage();
		wind->open(context->getImageFactory(), 100, 100);
		wind->setProjectionMatrix(Matrix::ortho(-50, 50, -50, 50, 1, 2))
			.setViewMatrix(Matrix::view(0, 0, 1, 0, 0, 0, 0, 1, 0))
			.setBaseColour(Colour::NONE);
	}


	void Scene::setupUI()
	{
		framerateText = Nova::create<Nova::Debug::FPSCounter>();
		framerateText->getText().setColour(Colour::WHITE)
			.setFontSize(16)
			.setOrigin(0.5f, 1.0f);
		framerateText->setPosition(0.5f, 0.95f, 0);
		framerateText->setFrequency(5000);


		textPower.setText("Wind Power: "
			+ std::to_string(static_cast<int>((power - 0.1f) * 100))
			+ " to "
			+ std::to_string(static_cast<int>((power + 0.1f) * 100)))
			.setFontSize(25)
			.setColour(Nova::Colour::WHITE)
			.setOrigin(0.0f, 1.0f);
		textPower.getTransform().setPosition(0.05f, 0.95f, 0);


		framerateText->addToStage(mainStage->getUI());
		addContent(framerateText);

		mainStage->getUI()->add(&textPower);

		windEffect.setImage(wind->getRenderTexture(), 450, 450);
		windEffect.getTransform().setPosition(1.0f, 1.0f, 0);
		windEffect.setOrigin(1.0f, 1.0f);

		//mainStage->getUI()->add(&windEffect);
	}


	void Scene::setupContent(const Nova::Context_p& context)
	{
		setupCamera(mainStage);

		mPackage.load("WS_PACKAGE.nova");

		// Setup the main and offscreen scenes
		createScenes(context, mainStage);

		setupUI();

		createTerrain();

		setupField();

		setupWind();
	}


	void Scene::createTerrain()
	{
		Nova::Image_p image = mPackage.findImage("FLOOR");
		image->build();
		image->cleanUp();

		// Create the square floor
		terrain = Nova::create<Nova::SimpleActor>();
		auto terrainProp = DBG_NEW Nova::ModelProp();
		Nova::Model_p terrainModel = getContext()->getModelFactory()->newModel();

		terrainModel->createFromData(getContext()->getRenderer()->getPlane());
		terrainModel->build();
		terrainModel->cleanUp();

		terrainProp->setModel(terrainModel);
		terrainProp->setTexture(image);

		terrain->setProp(terrainProp);

		terrain->getTransform().setRotation(90, 1, 0, 0);

		addContent(terrain);
	}


	void Scene::update(long millis)
	{
		const float seconds = static_cast<float>(millis) / 1000.0f;

		vEmitter = 0.15f;

		if (eUpdate || vEmitter != 0 || dPower != 0)
		{
			updateEmitter(seconds);
			textPower.setText("Wind Power: "
				+ std::to_string(static_cast<int>((power - 0.1f) * 100))
				+ " to "
				+ std::to_string(static_cast<int>((power + 0.1f) * 100)));

			eUpdate = false;
		}

		windEmitter->updatePhysics(millis);

		wind->add(windEmitter.get());

		wind->render(getContext()->getRenderer());

		wind->clear();

		//windEffect.invalidate();

		/*if (vCamera != 0)
		{
			cameraAngle += 45.0f * seconds * vCamera;

			if (cameraAngle > 360.0f)
				cameraAngle -= 360.0f;
			else if (cameraAngle < 0.0f)
				cameraAngle += 360.0f;

			updateCamera();
		}*/
	}


	void Scene::release()
	{

	}


	void Scene::createGrass(unsigned int bladesX, unsigned int bladesY, unsigned int width,
		unsigned int height, float cull)
	{
		std::shared_ptr<Nova::SimpleActor> grass = Nova::create<Nova::SimpleActor>();
		Nova::Model_p grassModel = getContext()->getModelFactory()->newModel();
		Model::Data data{};
		auto grassProp = DBG_NEW Nova::ModelProp();

		terrain->getTransform().setScale(static_cast<float>(width), static_cast<float>(height), 1.0f);

		// blades = 100, width = 50, 50 / 100 = 0.5
		const float spacingX = static_cast<float>(width) / static_cast<float>(bladesX);
		const float spacingY = static_cast<float>(height) / static_cast<float>(bladesY);
		// 12 vertices per blade
		// 3 points per face
		// 100 blades
		// = 3300

		data.vertices = DBG_NEW float[36 * bladesX * bladesY];
		data.uvs = DBG_NEW float[24 * bladesX * bladesY];
		data.indices = DBG_NEW unsigned int[30 * bladesX * bladesY];

		data.info.cElements = 0;
		data.info.cIndices = 0;
		unsigned int vIndex = 0;
		unsigned int uIndex = 0;

		const float uvOffset = 0.25f;

		Vector vertex;
		Transform transform;
		Matrix bTransform;

		// Creating each blade separately, but they all form part of the same model
		for (unsigned int y = 0; y < bladesY; y++)
		{
			for (unsigned int x = 0; x < bladesX; x++)
			{
				if (Random::randomFloat(0.0f, 1.0f) < cull)
					continue;

				transform = setupBlade(x, y);

				transform.move(x * spacingX, 0, y * spacingY);

				bTransform = transform.getMatrix();

				// Height divided by 5 segments
				float hSpacing = transform.getScale().y / 5.0f;

				for (int i = 0; i < 12; i++)
				{
					vertex.x = (i % 2 == 0 ? -0.15f : 0.15f);
					vertex.y = hSpacing * (i % 2 == 0 ? i : i - 1);
					vertex.z = 0.0f;

					vertex = bTransform * vertex;

					data.vertices[vIndex + (i * 3)] = vertex.x;
					data.vertices[vIndex + (i * 3) + 1] = vertex.y;
					data.vertices[vIndex + (i * 3) + 2] = vertex.z;

					data.uvs[uIndex + (i * 2)] = uvOffset + ((static_cast<float>(x) /
						static_cast<float>(bladesX)) / 2.0f);
					data.uvs[uIndex + (i * 2) + 1] = uvOffset + ((1.0f - (static_cast<float>(y) /
						static_cast<float>(bladesY))) /
						2.0f);
				}

				data.indices[data.info.cIndices] = data.info.cElements;
				data.indices[data.info.cIndices + 1] = data.info.cElements + 1;
				data.indices[data.info.cIndices + 2] = data.info.cElements + 2;

				data.indices[data.info.cIndices + 3] = data.info.cElements + 3;
				data.indices[data.info.cIndices + 4] = data.info.cElements + 2;
				data.indices[data.info.cIndices + 5] = data.info.cElements + 1;

				data.indices[data.info.cIndices + 6] = data.info.cElements + 2;
				data.indices[data.info.cIndices + 7] = data.info.cElements + 3;
				data.indices[data.info.cIndices + 8] = data.info.cElements + 4;

				data.indices[data.info.cIndices + 9] = data.info.cElements + 5;
				data.indices[data.info.cIndices + 10] = data.info.cElements + 4;
				data.indices[data.info.cIndices + 11] = data.info.cElements + 3;

				data.indices[data.info.cIndices + 12] = data.info.cElements + 6;
				data.indices[data.info.cIndices + 13] = data.info.cElements + 4;
				data.indices[data.info.cIndices + 14] = data.info.cElements + 5;

				data.indices[data.info.cIndices + 15] = data.info.cElements + 7;
				data.indices[data.info.cIndices + 16] = data.info.cElements + 6;
				data.indices[data.info.cIndices + 17] = data.info.cElements + 5;

				data.indices[data.info.cIndices + 18] = data.info.cElements + 6;
				data.indices[data.info.cIndices + 19] = data.info.cElements + 7;
				data.indices[data.info.cIndices + 20] = data.info.cElements + 8;

				data.indices[data.info.cIndices + 21] = data.info.cElements + 9;
				data.indices[data.info.cIndices + 22] = data.info.cElements + 8;
				data.indices[data.info.cIndices + 23] = data.info.cElements + 7;

				data.indices[data.info.cIndices + 24] = data.info.cElements + 8;
				data.indices[data.info.cIndices + 25] = data.info.cElements + 9;
				data.indices[data.info.cIndices + 26] = data.info.cElements + 10;

				data.indices[data.info.cIndices + 27] = data.info.cElements + 11;
				data.indices[data.info.cIndices + 28] = data.info.cElements + 10;
				data.indices[data.info.cIndices + 29] = data.info.cElements + 9;

				vIndex += 36;
				uIndex += 24;
				data.info.cIndices += 30;
				data.info.cElements += 12;
			}
		}

		Nova::Shader_p shader = getContext()->getShaderFactory()->newShader();
		shader->load(shaderFile, getContext()->getAssetFactory());
		shader->build();
		shader->cleanUp();

		grassModel->createFromData(data);
		grassModel->build();
		grassModel->cleanUp();

		grassProp->setModel(grassModel);
		grassProp->setTexture(wind->getRenderTexture());
		grassProp->getData().colour = Colour::GREEN;
		grassProp->getData().transMask = Colour::NONE;
		grassProp->setShader(shader);

		grass->setProp(grassProp);
		grass->getTransform().setPosition(-(width / 2.0f), 0, -(height / 2.0f));

		addContent(grass);

		delete[] data.vertices;
		delete[] data.uvs;
		delete[] data.indices;
	}


	void Scene::setupWind()
	{
		// Initial resource loading
		Nova::Image_p arrow = mPackage.findImage("ARROW");
		arrow->build();
		arrow->cleanUp();

		Nova::Image_p particle = mPackage.findImage("PARTICLE");
		particle->build();
		particle->cleanUp();

		// Emitter which produces the wind effect
		windEmitter = Nova::create<Nova::SimpleActor>();
		auto prop = DBG_NEW Nova::GunEmitterProp();
		Nova::EmitterProp::Settings settings = prop->getSettings();

		settings.spawnConeAngle = 10;
		settings.spawnBatchSize = 3;
		settings.spread = 60;
		settings.maxLife = 5000;
		settings.particleImage = particle;
		settings.speedRange[0] = 30;
		settings.speedRange[1] = 40;
		settings.sizeRange[0] = 35;
		settings.sizeRange[1] = 50;
		settings.spawnFreq = 750;
		settings.maxParticles = 1000;
		settings.transparency = Colour::NONE;

		prop->setup(settings);

		windEmitter->setProp(prop);

		// Setup the Sprite showing wind direction
		windDirection = Nova::create<SimpleActor>();
		auto wdProp = DBG_NEW Nova::SpriteProp();

		wdProp->createFromImage(arrow, 1, 1);
		wdProp->getData().colour = Colour::fromFloat(1, 1, 1, 0.5f);

		windDirection->setProp(wdProp);

		windDirection->getTransform().setPosition(0, 3.0f, 0)
			.setScale(0.1f, 0.1f, 1)
			.setRotation(90, 1, 0, 0)
			.rotate(0, 0, 1, 0);


		// Setup the emitter and wind direction to face the right way when the scene starts
		updateEmitter(0);

		// Adds the wind effect overlay to scene

		windOverlay = Nova::create<Nova::SimpleActor>();

        auto overlay = DBG_NEW Nova::SpriteProp();

		overlay->createFromImage(wind->getRenderTexture(), 1, 1);

		windOverlay->getTransform().setPosition(0, 0.5f, 0)
			.setRotation(90, 1, 0, 0);

		windOverlay->setProp(overlay);

		//addContent(windOverlay);

		addContent(windDirection);
	}


	void Scene::setupCamera(Nova::MainStage_p& mainStage)
	{
		mainStage->setViewMatrix(
			Matrix::view
			(
				50.0f * cosf(cameraAngle * rads), 50, 50.0f * sinf(cameraAngle * rads),
				0, 0, 0,
				0, 1, 0
			)
		);
	}


	void Scene::updateEmitter(float seconds)
	{
		auto prop = (Nova::EmitterProp*)windEmitter->getProp();
		Nova::EmitterProp::Settings settings = prop->getSettings();

		if (vEmitter != 0)
		{
			emitterAngle += vEmitter;

			if (emitterAngle >= 360.0f)
				emitterAngle -= 360.0f;

			if (emitterAngle < 0.0f)
				emitterAngle += 360.0f;
		}

		float c = cosf(emitterAngle * rads);
		float s = sinf(emitterAngle * rads);

		windEmitter->getTransform().setPosition(-75 * s, -75 * c, 0)
			.setRotation(emitterAngle, 0, 0, 1);

		// Update the wind direction arrow sprite to point in the correct direction
		windDirection->getTransform().setPosition(0, 3.0f, 0)
			.setScale(0.1f, 0.1f, 1)
			.setRotation(90, 1, 0, 0)
			.rotate(emitterAngle, 0, 1, 0);

		// Adjust result to be between 0 and 1
		c = (c / 2.0f) + 0.5f;
		s = (s / 2.0f) + 0.5f;

		// Base particle texture colour is white, so by setting the particle colour based
		// on the direction of travel, we can use that to decide how to affect our grass.
		// We can also use the alpha value as the wind's power
		if (dPower != 0)
		{
			power += (dPower * seconds * 0.1f);

			if (power < 0.1f)
				power = 0.1f;

			if (power > 0.9f)
				power = 0.9f;
		}

		settings.colourRange[0] = Colour::fromFloat(s, 0, c, power - 0.1f);
		settings.colourRange[1] = Colour::fromFloat(s, 0, c, power + 0.1f);

		prop->setup(settings);
	}
}
