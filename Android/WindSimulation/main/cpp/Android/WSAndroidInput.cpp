//
// Created by Chris Allen on 31/07/18.
//
// Android implementation of the Scene's handleInput method
//

#include "WSScene.h"

using Nova::Input_p;
using Nova::Event;
using Nova::Math::Vector;
using Nova::Math::VectorUtil;

namespace WindSim
{

	/**
	 * Handles the input from the user.
	 *
	 * This version handles input from a Joystick, and sets the wind
	 * direction and power based on the direction the joystick is moved in
	 * (relative to the origin), and the amount it has moved in that direction.
	 *
	 * @param event The latest Input Event received from the user.
	 */
	void Scene::handleInput(Input_p event)
	{
		// If no event received, skip function
		if (event == nullptr)
			return;

		// If event is not a Joystick Input, return
		if (event->getInputType() != Event::Input::JOYSTICK)
			return;

		// If event is not a joystick moved event, return
		if (!event->isActive())
			return;

		// Cast to Joystick Input event
		Event::JoystickInput* input = (Event::JoystickInput*)(event.get());

		Vector position = input->getPosition();

		// Calculate the angle of the emitter based on the joystick's position relative
		// to its origin.
		emitterAngle = static_cast<float>((atan2f(position.y, position.x)) * (180.0f / M_PI)) + 90.0f;

		// Calculate the wind power based on distance from the origin
		power = VectorUtil::magnitude(position) / 100.0f;

		// Force power to stay within 0 and 1
		if (power < 0.0f)
			power = 0.0f;

		if (power > 1.0f)
			power = 1.0f;

		// Request an emitter update.
		eUpdate = true;
	}

}

