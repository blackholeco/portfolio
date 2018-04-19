#pragma once

//
//	WSDemoStage.h by Chris Allen
//
//	This file is provided "as-is", for the sole purpose of a demonstration of my
//	work.  It is not intended to be copied or used in an external or third-party
//	project, and no support will be given for that use.
//
//	You may not use or copy this file, in whole or in part, to use in your own, or
//	other projects.  All rights reserved over this file.
//

#include "WSStage.h"

/**
 *	Demo stage for the app

 *	This mimics the final stage of development, but was used in the
	portfolio video to show development from the first stage to the final
	project
 */
class WSDemoStage : public WSStage
{
public:
	/**
	 *	Lays our stage out, creating the grass, particle effect, and render targets
	 */
	void setupStage();

	/**
	 *	Add this stage, and its contents, to the scene for rendering
	 */
	virtual void addToScene();

protected:
	/**
	 *	Called for each blade, provides a Transformation Matrix to alter its appearance randomly,
		creating a more natural effect

	 *	@param x : Base x-position of the blade of grass in the field
	 *	@param y : Base y-position of the blade of grass in the field

	 *	@return A Transformation Matrix which randomly alters the blade's position, y-rotation, and height
	 */
	NovaTransform setupBlade(unsigned int x, unsigned int y);

};
