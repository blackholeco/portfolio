/*
 *	GameMap.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include "Camera.h"
#include "MapPackage.h"
#include "AreaPackage.h"
#include "SystemPackage.h"
#include "Pickup.h"

using std::out_of_range;

class GameMap
{
public:
	GameMap();
	~GameMap();

	/**
	 *	Renders the current map to scene
	 */
	void Draw();

	/**
	 *	Get the Map's name

	 *	@return the Map's name
	 */
	string GetMapName() const;

	/**
	 *	Set up this map with a layout, theme, and game resources.  Packages should
		already be loaded before passing to this function

	 *	@param mpkg : Map Package to create the Map with (provides the layout)
	 *	@param apkg : Area Package to create the Map with (provides the resources for the theme)
	 *	@param system : System package (provides the game's main resources)

	 *	@return true if the Map is properly initialized
	 */
	bool InitializeMap(MapPackage mpkg, AreaPackage apkg, SystemPackage system);

	/**
	 *	Delete all data stored by this Map.  The Map will be unusable after calling Release(),
		so it should only be called when the Map is no longer required.
	 */
	void Release();

	/**
	 *	Update the view after the App's Window is resized

	 *	@param width : The Window's new width
	 *	@param height : The Window's new height
	 */
	void onReshape(int width, int height);

private:
	string mapname;

	unsigned int mapwidth;
	unsigned int mapheight;

	char** staticmap;

	GLuint outerwall;
	GLuint innerwall;
	GLuint floortex;

	SystemPackage mSystem;
	AreaPackage mArea;
	MapPackage mMap;

	vector<Scenery> scenery;
	deque<Bomb> bombs;
	deque<Explosion> explosions;
	deque<Pickup> pickups;

	array<PlayerCharacter, 4> players;

	Camera camera;

	Floor mFloor;
};

