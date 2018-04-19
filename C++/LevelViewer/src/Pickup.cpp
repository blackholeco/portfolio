/*
 *	Pickup.cpp by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#include "Pickup.h"


Pickup::Pickup()
{
	angle = 0;
	expired = true;
}


Pickup::Pickup(int type, Vector2f pos, shared_ptr<Model> model, shared_ptr<Texture> texture)
{
	switch (type)
	{
	case 0:
		stats.x = 1;
		stats.y = stats.z = 0;
		break;
	case 1:
		stats.y = 1;
		stats.x = stats.z = 0;
		break;
	case 2:
		stats.z = 1;
		stats.x = stats.y = 0;
		break;
	}

	position = pos;
	mModel = model;
	mTexture = texture;
	expired = false;
	angle = 0;
}


void Pickup::Update(int time)
{
	// just add to the rotation
	if (!expired)
	{
		angle += 10.0f / (float)time;

		// ensure angle stays between 0 and 360;
		while (angle >= 360)
			angle -= 360;
	}
}


void Pickup::Draw()
{
	if (!expired)
	{
		glInterface.setModelMatrix(Matrix4::translate(position.x, 0, position.y));
		glInterface.DrawModel(mModel, mTexture);
	}
}

