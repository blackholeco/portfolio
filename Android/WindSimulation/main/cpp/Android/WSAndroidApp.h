//
// Created by Chris Allen on 09/Sep/2018.
//

#ifndef WS_ANDROID_APP_H
#define WS_ANDROID_APP_H


#include <NovaAndroidApp.h>

namespace WindSim
{
	namespace Android
	{
		/**
		 * Android version of the Wind Simulation app.
		 * 
		 * Handles the Stage/Scene, as well as App's resources
		 */
		class App : public Nova::Android::App
		{
		public:
			App(JNIEnv* env, jobject assets);

		protected:
			
			/**
			 * Handle a window reshape event
			 * 
			 * @param width : New width of the window
			 * @param height : New height of the window
			 */
			void onReshape(int width, int height);

		private:
			
			/**
			 * Handle the startup of the App, setting up
			 * the initial state of the Stage.
			 */
			void onStart();
		};
	}
}

#endif //WS_ANDROID_APP_H
