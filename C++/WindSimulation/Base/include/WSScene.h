/*
 *		WSScene.h
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

#include <NovaPackage.h>
#include <NovaFPSCounter.h>
#include <NovaScene.h>
#include <NovaApp.h>
#include <NovaProp.h>

namespace WindSim
{
	/**
	 *	Base class for our Wind Sim Scenes.
	 *
	 *	Contains the terrain, grass field, and the particle effect for the wind,
	 *	plus the render targets needed for the displacement calculation
	 */
	class Scene : public Nova::Scene
	{
	public:
		Scene();

		/**
		 * Update the scene and its objects
		 *
		 * @param millis : Time in milliseconds since the last update
		 */
		void update(long millis) override;

		/**
		 * Clear up any memory used by this Scene, making it unusable
		 */
		void release() override;

		/**
		 *	Handle a window resize event
		 *
		 *	@param mainStage : Reference to the App's main render stage
		 *	@param width : New width of the window in pixels
		 *	@param height : New height of the window in pixels
		 */
		void resize(Nova::MainStage_p& mainStage, int width, int height) override;

	protected:

		/**
		 * Orbital angle of the camera in the scene
		 */
		float cameraAngle;

		/**
		 * Orbital angle of the emitter in the scene
		 */
		float emitterAngle;

		/**
		 * Speed and direction of camera movement in the scene.
		 *
		 * If movement is required, the camera orbits around the field, providing
		 * a different view of the scene.
		 */
		float vCamera;

		/**
		 * Speed and direction of emitter movement in the scene.
		 *
		 * If movement is required, the emitter orbits around the center of the world,
		 * giving the appearance of the wind changing direction
		 */
		float vEmitter;

		/**
		 * Render target for the wind effect.  The effect renders to texture, rather
		 * than into the main scene.  This gives us a texture we can sample when displacing
		 * our grass.
		 */
		Nova::SubStage_p wind;


		Nova::ImageWidget windEffect;

		/**
		 * Package containing the Scene's resources.
		 */
		Nova::Package mPackage;


		std::shared_ptr<Nova::SimpleActor> windOverlay;


		/**
		 * Particle emitter for the wind effect.  Particles emitted from a ParticleGun
		 * are unaffected by changes in their parent emitter's movements after they are
		 * emitted
		 */
		std::shared_ptr<Nova::SimpleActor> windEmitter;

		/**
		 *	Arrow which shows the direction of travel of the wind
		 */
		std::shared_ptr<Nova::SimpleActor> windDirection;

		/**
		 *	Simple square to act as the floor
		 */
		std::shared_ptr<Nova::SimpleActor> terrain;

		/**
		 * Debug FPS Counter showing the frames per second
		 */
		std::shared_ptr<Nova::Debug::FPSCounter> framerateText;

		/**
		 *	Text widget showing the current wind speed range
		 */
		Nova::TextWidget textPower;

		/**
		 * Current wind power
		 */
		float power;


		float dPower;

		/**
		 * Constant value used to convert degrees to radians
		 */
		const float rads;

		/**
		 * Flag to state if the camera needs moving
		 */
		bool cUpdate;

		/**
		 * Flag to state if the wind emitter needs moving, or the effect
		 * needs changing
		 */
		bool eUpdate;

		/**
		 * Name of the shader file that this Scene uses (gets defined per implementation
		 * of this class)
		 */
		std::string shaderFile;

		/**
		 *	Prepare the Scene's content, loading resources and creating objects
		 *
		 *	@param context : App's Context
		 */
		void setupContent(const Nova::Context_p& context) override;

		/**
		 * Set up the SubScenes required by the project
		 *
		 * @param context : App's Context
		 * @param mainStage : App's main rendering Stage.
		 */
		void createScenes(const Nova::Context_p& context, Nova::MainStage_p& mainStage);

		/**
		 * Create the field of grass.
		 *
		 * This allows you to change the density and the
		 * patchiness of the field, by changing the number of blades in each direction,
		 * the size of the field, and the amount of grass culled.
		 *
		 * @param bladesX : Number of blades of grass in the X axis
		 * @param bladesY : Number of blades of grass in the Y axis
		 * @param width : Width of the field
		 * @param height : Height of the field
		 * @param cull : Fraction of the blades to cull
		 */
		void createGrass(unsigned int bladesX, unsigned int bladesY, unsigned int width, unsigned int height, float cull = 0.0f);

		/**
		 * Setup and create the field of grass.  Must be implemented by subclasses.
		 *
		 *	This function needs to set the filename of the shader program to use for rendering
		 *	the field, and also define the dimensions and parameters of the field
		 */
		virtual void setupField() = 0;

		/**
		 * Handle any input from the user.
		 *
		 * This may be implemented in a separate file to the other functions,
		 * based on the platform, and based on requirements.
		 *
		 * @param event : The latest Input Event
		 */
		virtual bool handleInput(const Nova::Input_p& event) override;

		/**
		 * Setup a single blade of grass, affecting the size, position and rotation.
		 *
		 * This prevents the field from appearing uniform.
		 *
		 * @param x : X position of the blade in the field
		 * @param y : Y position of the blade in the field
		 * @return A transform matrix to apply to the Blade, which changes its height and rotation
		 */
		virtual Nova::Math::Transform setupBlade(unsigned int x, unsigned int y) = 0;

		/**
		 * Setup the wind particle effect
		 */
		void setupWind();

		/**
		 *	Setup the widgets to appear in the UI
		 */
		void setupUI();

		/**
		 *	Create the flat plane to act as a floor
		 */
		void createTerrain();

		/**
		 * Update the Camera's position if required
		 *
		 *	@param mainStage : Reference to the App's main stage
		 */
		void setupCamera(Nova::MainStage_p& mainStage);

		/**
		 * Update the wind emitter's position, direction and effect
		 *
		 *	@param seconds : Number of seconds since the last update
		 */
		void updateEmitter(float seconds);
	};

}
