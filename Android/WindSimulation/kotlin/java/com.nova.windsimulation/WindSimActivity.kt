package com.nova.windsimulation

import android.content.res.AssetManager
import android.graphics.Point
import com.nova.android.GLES.JNovaGLRenderer
import com.nova.android.GLES.NovaGLView
import com.nova.android.api18.NovaAndroidApi18Activity

/**
 * Created by Chris Allen on 11/Sep/2018.
 *
 * Kotlin version of the Wind Simulation App
 *
 * Wind Simulation simulates a field of grass being affected by the wind.
 * This version is controlled by touch, using a virtual joystick.
 * Moving the joystick in a direction makes the wind blow in that direction,
 * and the further the joystick is moved in a direction increases the wind's power.
 */
class WindSimActivity : NovaAndroidApi18Activity() {

	init {
		System.loadLibrary("ws-android")
	}

	/**
	 * Instance of the interface used to respond to the Renderer.
	 */
	private val listener: JNovaGLRenderer.OnRenderListener = object : JNovaGLRenderer.OnRenderListener {

		/**
		 * Called when the Renderer opens
		 */
		override fun initialize() {
			val size = Point()

			val manager = windowManager

			manager.defaultDisplay.getSize(size)

			novaInitialise(assets, size.x, size.y)
		}

		/**
		 * Called each time the Renderer requests a render.
		 */
		override fun step() {
			novaStep()
		}

		/**
		 * Called when the Render Surface changes dimensions
		 *
		 * @param width : New width of the Render Surface
		 * @param height : New height of the Render Surface
		 */
		override fun onSurfaceChanged(width: Int, height: Int) {
			novaReshape(width, height)
		}
	}

	/**
	 * Set up the Render View, providing a Listener to respond to different events.
	 *
	 * @param view : The Render View (a View which will display the game world)
	 */
	public override fun setupView(view: NovaGLView) {
		view.setListener(listener)
	}

	/**
	 * Function used to initialise Nova and the game world (implemented in C++)
	 *
	 * @param manager : This Activity's AssetManager
	 * @param width : Initial width of the Render View
	 * @param height : Initial height of the Render View
	 */
	external fun novaInitialise(manager: AssetManager, width: Int, height: Int)

	/**
	 * Function used to update and render the game world (implemented in C++)
	 */
	external fun novaStep()

	/**
	 * Function used to handle a resize of the Render View (implemented in C++)
	 *
	 * @param width : The new width of the Render View
	 * @param height : The new height of the Render View
	 */
	external fun novaReshape(width: Int, height: Int)
}
