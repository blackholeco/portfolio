/*
 *		WindSim.h
 *
 *	Author: Chris Allen
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

#pragma once

#include <NovaGLFWApp.h>

#include "WSFinalScene.h"

namespace WindSim::Windows
{

	/**
	 *	Application class for the Wind Simulation project for Windows
	 */
	class App : public Nova::GL::GLFWApp
	{
	public:

		/**
		 *	Construct a new instance of the App - Should only be one per program instance
		 *
		 *	@param argc: Number of arguments passed to the command line
		 *	@param argv: List of command line arguments passed into the program
		 */
		App(int argc, char** argv);

	protected:

		/**
		 *	Action to take when the App starts up.  Sets up the initial view matrix of the stage
		 */
		void onStart();
	};
}
