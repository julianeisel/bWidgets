#include "Application.h"
#include "default_stage.h"
#include "WindowManager.h"

using namespace bWidgetDemo;


Application& Application::CreateApplication()
{
	static Application app;
	return app;
}

void Application::setup()
{
	WindowManager& _wm = WindowManager::CreateWindowManager();
	Window* win = _wm.addWindow("bWidgets Demo");

	win->stage = SCR_default_stage_add();
	wm = &_wm;
}

void Application::mainLoop()
{
	wm->mainLoop();
}

void Application::exit()
{
}
