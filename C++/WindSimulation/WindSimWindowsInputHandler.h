//
//	Created by Chris Allen on 29-July-18
//
// Comitted at $Rev$ on $Date$
//


/*
 *		WindSimWindowsInputHandler.h
 *
 *	Author: Chris Allen
 *	Copyright Chris Allen 2018, all rights reserved
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

#include "WindSimInputHandler.h"

namespace WindSim::Windows
{
	class InputHandler : public WindSim::InputHandler
	{
	public:
		int handle(Nova::Input_p event);
	};
}