/*
 *		WSFinalScene.h
 *
 *	Author: Chris Allen
 *	Copyright Chris Allen 2018, all rights reserved
 *
 *	Committed at $Rev: 10 $ on $Date: 2018-10-15 21:13:31 +0100 (Mon, 15 Oct 2018) $
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

#pragma once

#include "WSScene.h"

namespace WindSim
{
	/**
	 * Scene showing the final result of the project, including the improvements
	 * made since the demo (e.g. denser field, culling, random variance in the grass,
	 * blades bending in the wind)
	 */
	class FinalScene : public WindSim::Scene
	{
	public:

		/**
		 * Setup the Scene, creating the field and the wind effect
		 *
		 * @param context : The App's Context
		 * @param mainStage : The App's Main Rendering Stage
		 */
		void setup(Nova::Context_p& context, Nova::MainStage_p& mainStage);

	protected:

		/**
		 * This Scene's implementation of setupBlade().  Adds random variance in
		 * each blade of grass, making for a less uniform look.
		 *
		 * @param x : X position of the blade
		 * @param y : Y position of the blade
		 * @return : A Transform matrix used to affect the blade
		 */
		Nova::Math::Transform setupBlade(unsigned int x, unsigned int y);
	};
}
