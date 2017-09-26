#include <iostream>

#include "GPU.h"

#include "EventManager.h"
#include "WindowManager.h"

using namespace bWidgetsDemo;


static void wm_glfw_error_callback(int /* error */, const char* description)
{
	std::cout << "Error: " << description << std::endl;
}

WindowManager::WindowManager() :
    event_manager(EventManager::ensureEventManager())
{
	if (!glfwInit()) {
		assert(0);
	}
	glfwSetErrorCallback(wm_glfw_error_callback);

//	GPU_init(); // needs context, so delay until window creation
}

WindowManager& WindowManager::CreateWindowManager()
{
	static WindowManager instance;
	return instance;
}

WindowManager::~WindowManager()
{
	GPU_exit();
	for (Window *win : windows) {
		delete win;
	}
	glfwTerminate();
}

WindowManager::WindowManagerAction WindowManager::processEvents()
{
	event_manager.waitEvents();
	if (!event_manager.processEvents(windows)) {
		return WM_ACTION_CLOSE;
	}

	return WM_ACTION_CONTINUE;
}

void WindowManager::drawWindows()
{
	for (Window* win: windows) {
		win->draw();
	}
}

void WindowManager::mainLoop()
{
	while (processEvents() == WM_ACTION_CONTINUE) {
		drawWindows();
	}
}

Window* WindowManager::addWindow(std::string name)
{
	Window *win_new = new Window(name);

	windows.push_back(win_new);
	if (windows.size() == 1) {
		main_win = win_new;
	}

	return win_new;
}

void WindowManager::removeWindow(Window* win)
{
	windows.remove(win);
}

const bool WindowManager::isMainWindow(const Window *win) const
{
	return win == main_win;
}
