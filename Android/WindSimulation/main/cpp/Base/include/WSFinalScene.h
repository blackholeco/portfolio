//
//  Created by Chris Allen
//

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
