/*
 *	GameMap.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "GameMap.h"


GameMap::GameMap()
{
	staticmap = nullptr;
	mapname.clear();
	scenery.clear();
}


GameMap::~GameMap()
{
	Release();
}


void GameMap::Draw()
{
	camera.Update();

	glInterface.beginRender();

	//Draw the floor
	mFloor.Draw();

	// Draw the scenery
	for (auto iter = scenery.begin(); iter != scenery.end(); ++iter)
		iter->Draw();

	for (PlayerCharacter player : players)
		player.Draw();

	glInterface.endRender();
}


string GameMap::GetMapName() const
{
	return mapname;
}


bool GameMap::InitializeMap(MapPackage mpkg, AreaPackage apkg, SystemPackage system)
{
	mMap = mpkg;

	mSystem = system;

	mArea = apkg;

	staticmap = new char*[mMap.GetRows()];

	assert(staticmap);

	for (unsigned int i = 0; i < mMap.GetRows(); i++)
	{
		staticmap[i] = new char[mMap.GetColumns()];
		assert(staticmap[i]);
	}

	mSystem.Import();

	// Import Area resources
	glInterface.BuildTexture(mArea.floortex);
	glInterface.BuildModel(mArea.outerWall);
	glInterface.BuildModel(mArea.innerWall);

	for (unsigned int i = 0; i < mMap.GetRows(); i++)
	{
		for (unsigned int j = 0; j < mMap.GetColumns(); j++)
		{
			staticmap[i][j] = mMap.layout[i][j];

			Vector2f position(static_cast<float>(j), static_cast<float>(i));

			switch (mMap.layout[i][j])
			{
			case 'W':
				scenery.push_back(Scenery(mArea.outerWall, position));
				break;
			case 'w':
				scenery.push_back(Scenery(mArea.innerWall, position));
				break;
			}
		}
	}

	mFloor.Initialize(mMap.GetColumns(), mMap.GetRows(), mArea.floortex);

	mFloor.Import();

	for (int i = 0; i < 4; i++)
	{
		Vector2f position;
		PlayerCharacter& player = players[i];

		player.SetUp(mSystem.playermesh, mSystem.playerTexture[i], mSystem.bombTexture[i]);

		switch (i)
		{
		case 0:
			position.x = 1;
			position.y = 1;
			break;
		case 1:
			position.x = 14;
			position.y = 1;
			break;
		case 2:
			position.x = 1;
			position.y = 14;
			break;
		case 3:
			position.x = 14;
			position.y = 14;
			break;
		}

		player.SetPosition(position);
	}

	return true;
}


void GameMap::Release()
{
	mapwidth = mapheight = 0;

	mapname.clear();

	if (staticmap)
	{
		for (unsigned int i = 0; i < mapheight; i++)
			delete[] staticmap[i];

		delete[] staticmap;
	}

	staticmap = nullptr;

	glInterface.DeleteTexture(floortex);
}


void GameMap::onReshape(int width, int height)
{
	//set the viewport to the current window specifications
	glViewport(0, 0, width, height);

	camera.InitCamera(60, static_cast<float>(width) / static_cast<float>(height), 1, 25);
	
	// set the camera to the middle of the map
	camera.SetCameraPosition((mMap.GetColumns() / 2.0f) - 0.5f, 15, mMap.GetRows() + 2.5f);

	camera.SetTargetPosition((mMap.GetColumns() / 2.0f) - 0.5f, 0, mMap.GetRows() / 2.0f);
}

