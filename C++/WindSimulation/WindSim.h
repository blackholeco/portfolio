/*
 *		WindSim.h
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

#include <NovaGLFWApp.h>

#include "WSFinalScene.h"

namespace WindSim::Windows
{

	class App : public Nova::GL::GLFWApp
	{
	public:
		App(int argc, char** argv);

	protected:
		void onStart();
	};
}
