#include "Application.h"
#include "DefaultStage.h"
#include "WindowManager.h"

using namespace bWidgetsDemo;


Application& Application::ensureApplication()
{
	static Application app;
	return app;
}

void Application::setup()
{
	WindowManager& _wm = WindowManager::CreateWindowManager();
	Window* win = _wm.addWindow("bWidgets Demo");

	win->stage = new DefaultStage(win->getWidth(), win->getHeight());
	wm = &_wm;
}

void Application::mainLoop()
{
	wm->mainLoop();
}

void Application::exit()
{
}
