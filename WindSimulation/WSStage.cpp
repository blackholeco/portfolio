#include "WSStage.h"

WSStage::WSStage()
	: rads(static_cast<float>(M_PI) / 180.0f)
{
	cameraAngle = 90;
	emitterAngle = 90;
	vCamera = 0;
	vEmitter = 0;

	frameCount = 0;
	timer = 0;

	power = 1.0f;

	eUpdate = true;

	shaderFile = "wind.glsl";
}


void WSStage::createScenes()
{
	terrainScene = nContext->getSceneFactory()->newSubScene();
	terrainScene->open(nContext->getImageFactory(), 1280, 800);
	terrainScene->setProjectionMatrix(
		NovaMatrixUtil::perspective(45, 1280.0f / 800.0f, 1, 150)
	);
	terrainScene->setBaseColour(NovaColour::BLACK);

	updateCamera();

	wind = nContext->getSceneFactory()->newSubScene();
	wind->open(nContext->getImageFactory(), 100, 100);
	wind->setProjectionMatrix(
		NovaMatrixUtil::ortho(-50, 50, -50, 50, 1, 2)
	);
	wind->setViewMatrix(
		NovaMatrixUtil::view(0, 0, 1, 0, 0, 0, 0, 1, 0)
	);
	wind->setBaseColour(NovaColour::NONE);

	terrain.transform.setScale(50, 50, 1)
		.setRotation(90, 1, 0, 0);

	windOrigin.createFromImage(windImage, 1, 1);
	windOrigin.setTransparent(NovaColour::NONE);
	windOrigin.setColour(NovaColour::fromFloat(1, 1, 1, 0.5f));

	windSceneTrans.setScale(3, 3, 1)
		.setPosition(1030 / 2, 250, 0);
}


void WSStage::loadResources()
{
	mPackage.load("WS_PACKAGE.nova");

	grass.shader = nContext->getShaderFactory()->newShader();

	grass.shader->load(shaderFile, nContext->getAssetFactory());
	grass.shader->build();
	grass.shader->cleanUp();

	windImage = mPackage.findImage("ARROW");
	windImage->build();
	windImage->cleanUp();

	windParticle = mPackage.findImage("PARTICLE");
	windParticle->build();
	windParticle->cleanUp();

	terrainTex = mPackage.findImage("FLOOR");
	terrainTex->build();
	terrainTex->cleanUp();

	terrain.model = nContext->getModelFactory()->newModel();
	terrain.model->createFromData(nContext->getRenderer()->getPlane(), terrainTex);
	terrain.model->build();
	terrain.model->cleanUp();
}


void WSStage::update(long millis)
{
	const float seconds = static_cast<float>(millis) / 1000.0f;

	frameCount++;

	timer += seconds;

	if (timer >= 6.0f)
	{
		NLOGV("Framerate %.2f fps", static_cast<float>(frameCount) / timer);

		frameCount = 0;
		timer = 0;
	}

	novaInputEvent input = nContext->getEvent();

	if (input != nullptr && input->getEventType() == NovaInput::NOVA_INPUT_KEYBOARD)
	{
		auto key = std::dynamic_pointer_cast<NovaInputKeyboardEvent>(input);

		switch (key->getKey())
		{
		case 'a':
			vEmitter = (key->isActive() ? 1.0f : 0);

			break;
		case 'd':
			vEmitter = (key->isActive() ? -1.0f : 0);

			break;
		case NovaInput::NOVA_UP:
		{
			if (key->isActive())
			{
				power += 0.1f;

				if (power > 1.0f)
					power = 1.0f;

				eUpdate = true;
			}

		}
		break;
		case NovaInput::NOVA_DOWN:
		{
			if (key->isActive())
			{
				power -= 0.1f;

				if (power < 0.2f)
					power = 0.2f;

				eUpdate = true;
			}

		}
		break;
		case NovaInput::NOVA_LEFT:

			vCamera = (key->isActive() ? 1.0f : 0.0f);

			break;

		case NovaInput::NOVA_RIGHT:

			vCamera = (key->isActive() ? -1.0f : 0.0f);

			break;
		}
	}

	if (vEmitter != 0 || eUpdate)
	{
		emitterAngle += 45.0f * seconds * vEmitter;

		if (emitterAngle > 360.0f)
			emitterAngle -= 360.0f;
		else if (emitterAngle < 0)
			emitterAngle += 360.0f;

		updateEmitter();

		eUpdate = false;
	}

	if (vCamera != 0)
	{
		cameraAngle += 45.0f * seconds * vCamera;

		if (cameraAngle > 360.0f)
			cameraAngle -= 360.0f;
		else if (cameraAngle < 0.0f)
			cameraAngle += 360.0f;

		updateCamera();
	}

	windEmitter.update(millis);
}


void WSStage::release()
{

}


void WSStage::createGrass(unsigned int bladesX, unsigned int bladesY, unsigned int width, unsigned int height, float cull)
{
	nModelData data;

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

	data.elementCount = 12 * bladesX * bladesY;
	data.indexCount = 30 * bladesX * bladesY;

	unsigned int vIndex = 0;
	unsigned int uIndex = 0;
	unsigned int iIndex = 0;
	unsigned int eIndex = 0;

	const float uvOffset = 0.25f;

	NovaVector3 vertex;
	NovaTransform transform;
	NovaMatrix bTransform;

	for (unsigned int y = 0; y < bladesY; y++)
	{
		for (unsigned int x = 0; x < bladesX; x++)
		{
			if (NovaRandom::randomFloat(0.0f, 1.0f) < cull)
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

				vertex = NovaMatrixUtil::multiply(vertex, bTransform);

				data.vertices[vIndex + (i * 3)] = vertex.x;
				data.vertices[vIndex + (i * 3) + 1] = vertex.y;
				data.vertices[vIndex + (i * 3) + 2] = vertex.z;

				data.uvs[uIndex + (i * 2)] = uvOffset + ((static_cast<float>(x) / static_cast<float>(bladesX)) / 2.0f);
				data.uvs[uIndex + (i * 2) + 1] = uvOffset + ((1.0f - (static_cast<float>(y) / static_cast<float>(bladesY))) / 2.0f);
			}

			data.indices[iIndex] = eIndex;
			data.indices[iIndex + 1] = eIndex + 1;
			data.indices[iIndex + 2] = eIndex + 2;

			data.indices[iIndex + 3] = eIndex + 3;
			data.indices[iIndex + 4] = eIndex + 2;
			data.indices[iIndex + 5] = eIndex + 1;

			data.indices[iIndex + 6] = eIndex + 2;
			data.indices[iIndex + 7] = eIndex + 3;
			data.indices[iIndex + 8] = eIndex + 4;

			data.indices[iIndex + 9] = eIndex + 5;
			data.indices[iIndex + 10] = eIndex + 4;
			data.indices[iIndex + 11] = eIndex + 3;

			data.indices[iIndex + 12] = eIndex + 6;
			data.indices[iIndex + 13] = eIndex + 4;
			data.indices[iIndex + 14] = eIndex + 5;

			data.indices[iIndex + 15] = eIndex + 7;
			data.indices[iIndex + 16] = eIndex + 6;
			data.indices[iIndex + 17] = eIndex + 5;

			data.indices[iIndex + 18] = eIndex + 6;
			data.indices[iIndex + 19] = eIndex + 7;
			data.indices[iIndex + 20] = eIndex + 8;

			data.indices[iIndex + 21] = eIndex + 9;
			data.indices[iIndex + 22] = eIndex + 8;
			data.indices[iIndex + 23] = eIndex + 7;

			data.indices[iIndex + 24] = eIndex + 8;
			data.indices[iIndex + 25] = eIndex + 9;
			data.indices[iIndex + 26] = eIndex + 10;

			data.indices[iIndex + 27] = eIndex + 11;
			data.indices[iIndex + 28] = eIndex + 10;
			data.indices[iIndex + 29] = eIndex + 9;

			vIndex += 36;
			uIndex += 24;
			iIndex += 30;
			eIndex += 12;
		}
	}

	data.elementCount = eIndex;
	data.indexCount = iIndex;

	grass.graphic.model = nContext->getModelFactory()->newModel();
	grass.graphic.model->createFromData(data, wind->getScene());
	grass.graphic.colour = NovaColour::GREEN;
	grass.graphic.transform.setPosition(-24.5f, 0, -24.5f);
	grass.graphic.model->build();
	grass.graphic.model->cleanUp();

	/*delete[] data.vertices;
	delete[] data.uvs;
	delete[] data.indices;*/
}


void WSStage::addToScene()
{
	NovaTransform scale;

	scale.setScale(1, 1, 1);

	windEmitter.addToScene(wind);

	wind->render(nContext->getRenderer());

	terrainScene->add({ &terrain, &grass });

	windOrigin.addToScene(wOTrans, terrainScene);

	terrainScene->render(nContext->getRenderer());

	terrainScene->addToScene(scale, nContext->getMainScene());

	wind->addToScene(windSceneTrans, nContext->getMainScene());
}


void WSStage::setupWind()
{
	EmitterSettings settings = NovaParticleEmitter::createSettings();

	settings.spawnConeAngle = 10;
	settings.spawnBatchSize = 3;
	settings.spread = 60;
	settings.maxLife = 5000;
	settings.particleImage = windParticle;
	settings.speedRange[0] = 30;
	settings.speedRange[1] = 40;
	settings.sizeRange[0] = 35;
	settings.sizeRange[1] = 50;
	settings.spawnFreq = 750;
	settings.maxParticles = 1000;
	settings.transparency = NovaColour::NONE;

	windEmitter.setup(settings);

	updateEmitter();
}


void WSStage::updateCamera()
{
	terrainScene->setViewMatrix(
		NovaMatrixUtil::view
		(
			50.0f * static_cast<float>(cosf(cameraAngle * rads)), 25, 50.0f * static_cast<float>(sinf(cameraAngle * rads)),
			0, 0, 0,
			0, 1, 0
		)
	);
}


void WSStage::updateEmitter()
{
	EmitterSettings settings = windEmitter.getSettings();

	float c = static_cast<float>(cosf(emitterAngle * rads));
	float s = static_cast<float>(sinf(emitterAngle * rads));

	windEmitter.getTransform().setPosition(-75 * s, -75 * c, 0)
		.setRotation(emitterAngle, 0, 0, 1);

	wOTrans.setPosition(0, 3.0f, 0)
		.setScale(0.1f, 0.1f, 1)
		.setRotation(90, 1, 0, 0)
		.rotate(emitterAngle, 0, 1, 0);

	// Adjust result to be between 0 and 1
	c = (c / 2.0f) + 0.5f;
	s = (s / 2.0f) + 0.5f;

	settings.colourRange[0] = NovaColour::fromFloat(s, 0, c, power - 0.2f);
	settings.colourRange[1] = NovaColour::fromFloat(s, 0, c, power);

	windEmitter.setup(settings);
}

