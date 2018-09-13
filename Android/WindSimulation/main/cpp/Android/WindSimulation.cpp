//
// Created by Chris Allen on 11/Sep/2018.
//
// Definition of native functions required by the project
//

#include "WSAndroidApp.h"
#include "WSFinalScene.h"

extern "C"
{

JNIEXPORT void JNICALL
Java_com_nova_windsimulation_WindSimActivity_novaStep(JNIEnv* /*env*/, jobject /*type*/)
{
	Nova::Android::App* app = (Nova::Android::App*) (Nova::App::getInstance().get());

	app->requestStep();
	app->run();
}


JNIEXPORT void JNICALL
Java_com_nova_windsimulation_WindSimActivity_novaReshape(JNIEnv* /*env*/, jobject /*type*/,
                                                         jint width, jint height)
{
	Nova::App::reshape(width, height);
}

JNIEXPORT void JNICALL
Java_com_nova_windsimulation_WindSimActivity_novaInitialise(JNIEnv* env, jobject /*type*/,
                                                            jobject assets, jint width,
                                                            jint height)
{
	Nova::App::WindowSettings settings;

	settings.width = static_cast<unsigned int>(width);
	settings.height = static_cast<unsigned int>(height);

	Nova::Android::App* app = DBG_NEW WindSim::Android::App(env, assets);

	Nova::App::open(app, settings);

	Nova::App::openScene(DBG_NEW WindSim::FinalScene());

	Nova::App::start();
}

}