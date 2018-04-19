/*
 *	Pickup.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include "GameObject.h"

class Pickup : public DynamicObject
{
public:
	Pickup();

	/**
	 *	Create a new Pickup object

	 *	@param type : The type of Pickup to create
	 *	@param pos : The position of the Pickup in the game wold
	 *	@param model : The Model to use to render the Pickup
	 *	@param texture : The Texture to apply to the Model when rendering
	 */
	Pickup(int type, Vector2f pos, shared_ptr<Model> model, shared_ptr<Texture> texture);

	/**
	 *	Update this object

	 *	@param millis : Time in milliseconds since the last update
	 */
	void Update(const int millis);

	/**
	 *	Render this Pickup to the scene

	 */
	virtual void Draw();

protected:
	float angle;

	shared_ptr<Texture> mTexture;

	Vector3i stats;
};