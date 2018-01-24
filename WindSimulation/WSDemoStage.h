/*
 *	WSDemoStage.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include "WSStage.h"

/**
 *	WSStage implementation for the demo version of this project
 */
class WSDemoStage : public WSStage
{
public:
	/**
	 *	Setup the objects that will appear in our stage
	 */
	void setupStage();

	/**
	 *	Add all objects to the scene for rendering
	 */
	virtual void addToScene();

protected:

	/**
	 *	For each blade of grass, create a transformation matrix to alter how it looks

	 *	@param x : Original X position of the blade
	 *	@param y : Original Y position of the blade

	 *	@return a Model Transformation matrix to transform a blade of grass with
	 */
	NovaTransform setupBlade(unsigned int x, unsigned int y);
};
