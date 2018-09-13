package com.nova.windsimulation;

import android.content.res.AssetManager;
import android.graphics.Point;
import android.view.WindowManager;

import com.nova.android.GLES.JNovaGLRenderer;
import com.nova.android.GLES.NovaGLView;

/**
 * Created by Chris Allen on 22/Mar/2018.
 * <p>
 * Android version of the Wind Simulation project, powered by Nova.
 * <p>
 * Wind Simulation simulates a field of grass being affected by the wind.
 * This version is controlled by touch, using a virtual joystick.
 * Moving the joystick in a direction makes the wind blow in that direction,
 * and the further the joystick is moved in a direction increases the wind's power.
 */

public class WindSimuActivity extends NovaAndroidApi18Activity {

	static {
		System.loadLibrary("ws-android");
	}

	/**
	 * Instance of the interface used to respond to the Renderer.
	 */
	private final JNovaGLRenderer.OnRenderListener listener = new JNovaGLRenderer.OnRenderListener() {

		/**
		 * Called when the Renderer opens
		 */
		@Override
		public void initialize() {

			Point size = new Point();

			WindowManager manager = getWindowManager();

			manager.getDefaultDisplay().getSize(size);

			novaInitialise(getAssets(), size.x, size.y);
		}

		/**
		 * Called each time the Renderer requests a render.
		 */
		@Override
		public void step() {
			novaStep();
		}

		/**
		 * Called when the Render Surface changes dimensions
		 *
		 * @param width : New width of the Render Surface
		 * @param height : New height of the Render Surface
		 */
		@Override
		public void onSurfaceChanged(int width, int height) {
			novaReshape(width, height);
		}
	};

	/**
	 * Set up the Render View, providing a Listener to respond to different events.
	 *
	 * @param view : The Render View (a View which will display the game world)
	 */
	public void setupView(NovaGLView view) {
		view.setListener(listener);
	}

	/**
	 * Function used to initialise Nova and the game world (implemented in C++)
	 *
	 * @param manager : This Activity's AssetManager
	 * @param width   : Initial width of the Render View
	 * @param height  : Initial height of the Render View
	 */
	public native void novaInitialise(AssetManager manager, int width, int height);

	/**
	 * Function used to update and render the game world (implemented in C++)
	 */
	public native void novaStep();

	/**
	 * Function used to handle a resize of the Render View (implemented in C++)
	 *
	 * @param width  : The new width of the Render View
	 * @param height : The new height of the Render View
	 */
	public native void novaReshape(int width, int height);
}
