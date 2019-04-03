/*
 *		WindSimWindowsInputHandler.cpp
 *
 *	Author: Chris Allen
 *	Date: 29-July-18
 *	Copyright Chris Allen 2018, all rights reserved
 *
 *	Committed at $Rev: 16 $ on $Date: 2019-04-01 13:55:28 +0100 (Mon, 01 Apr 2019) $
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

#include "WSScene.h"

using Nova::Input_p;
using Nova::Event;

namespace WindSim
{
	bool Scene::handleInput(const Input_p& event)
	{
		if (event == nullptr)
			return false;

		if (event->getInputType() != Event::Input::Type::KEYBOARD)
			return false;

		auto input = std::static_pointer_cast<Event::KeyboardInput>(event);

		switch (tolower(input->getKey()))
		{
		case 'd':

			vEmitter = (input->isActive() ? 1.0f : 0.0f);
			break;

		case 's':
			dPower = (input->isActive() ? -1.0f : 0.0f);
			break;

		case 'a':
			vEmitter = (input->isActive() ? -1.0f : 0.0f);
			break;

		case 'w':
			dPower = (input->isActive() ? 1.0f : 0.0f);
			break;

		default:
			return false;
		}

		eUpdate = true;

		return true;
	}
}

