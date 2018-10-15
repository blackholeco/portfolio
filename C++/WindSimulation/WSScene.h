/*
 *		WSScene.h
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

#include <NovaParticle.h>
#include <NovaSprite.h>
#include <NovaPackage.h>
#include <NovaFPSCounter.h>
#include <NovaScene.h>
#include <NovaApp.h>

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
		void update(long millis);

		/**
		 * Clear up any memory used by this Scene, making it unusable
		 */
		void release();

		/**
		 * Add this Scene, and its objects, to Stage for rendering
		 *
		 * @param mainStage : Reference to the App's main rendering Stage
		 */
		virtual void addToStage(Nova::MainStage_p& mainStage);

		void resize(Nova::MainStage_p& mainStage, int width, int height);

	protected:

		/**
		 * Set up the SubScene's required by the project
		 *
		 * @param context : App's Context
		 * @param mainStage : App's main rendering Stage.
		 */
		void createScenes(Nova::Context_p& context, Nova::MainStage_p& mainStage);

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
		 * Load the resources required by this project
		 *
		 * @param context : The App's Context
		 */
		void loadResources(Nova::Context_p& context);

		/**
		 * Handle any input from the user.
		 *
		 * This may be implemented in a separate file to the other functions,
		 * based on the platform, and based on requirements.
		 *
		 * @param event : The latest Input Event
		 */
		void handleInput(Nova::Input_p event);

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
		 * Update the Camera's position if required
		 */
		void setupCamera(Nova::MainStage_p& mainStage);

		/**
		 * Update the wind emitter's position, direction and effect
		 */
		void updateEmitter(float seconds);

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

		/**
		 * Package containing the Scene's resources.
		 */
		Nova::Package mPackage;

		/**
		 * Texture for the terrain
		 */
		Nova::Image_p terrainTex;

		/**
		 * Texture for the wind particle
		 */
		Nova::Image_p windParticle;

		/**
		 * Texture for the arrow showing the wind direction
		 */
		Nova::Image_p windImage;

		/**
		 * Model for the terrain
		 */
		Nova::_Model terrain;

		/**
		 * Arrow sprite showing the direction of wind
		 */
		Nova::Sprite windOrigin;

		/**
		 * Model for the blade of grass.
		 *
		 * A _Custom allows us to render a 3D model with a user defined
		 * shader.  This lets us use a shader which samples the wind's sub scene
		 * texture for data rather than colour.
		 */
		Nova::_Custom grass;

		/**
		 * Particle emitter for the wind effect.  Particles emitted from a ParticleGun
		 * are unaffected by changes in their parent emitter's movements after they are
		 * emitted
		 */
		Nova::ParticleGun windEmitter;

		/**
		 * Transformation Matrix of the arrow showing the wind direction
		 */
		Nova::Math::Transform wOTrans;

		/**
		 * Transformation Matrix of the wind sub scene (if you want to display it)
		 */
		Nova::Math::Transform windSceneTrans;

		/**
		 * Transformation Matrix of the world's Sub Scene
		 */
		Nova::Math::Transform mainSceneTrans;

		/**
		 * Debug FPS Counter showing the frames per second
		 */
		Nova::Debug::FPSCounter framerateText;


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
	};

}
