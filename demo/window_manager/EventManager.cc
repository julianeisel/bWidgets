#include <list>

#include "GPU.h"

#include "Stage.h"

#include "EventManager.h"
#include "WindowManager.h"

using namespace bWidgetDemo;


EventManager& EventManager::CreateEventManager()
{
	static EventManager instance;
	return instance;
}

void EventManager::waitEvents()
{
	glfwWaitEvents();
}

bool EventManager::processEvents(std::list<Window*>& windows)
{
	for (std::list<Window*>::iterator i = windows.begin(); i != windows.end(); i++) {
		Window* win = *i;

		/* TODO, should register handler lists that are handled in
		 * event-manager, for now, just doing it with callbacks */
		if (win->processEvents() == Window::WINDOW_ACTION_CLOSE) {
#if 0
			if (isMainWindow(win)) {
				return WM_ACTION_CLOSE;
			}
			else {
				removeWindow(win);
				i--;
			}
#endif
			return false;
		}
	}

	return true;
}

void EventManager::setupWindowHandlers(Window& window)
{
	GLFWwindow* glfw_window = window.getGlfwWindow();

	glfwSetWindowUserPointer(glfw_window, &window);
	glfwSetMouseButtonCallback(glfw_window, handleMouseButtonEvent);
	glfwSetWindowSizeCallback(glfw_window, handleWindowResizeEvent);
}

void EventManager::handleWindowResizeEvent(GLFWwindow* glfw_win, int new_win_x, int new_win_y)
{
	Window* win = (Window*)glfwGetWindowUserPointer(glfw_win);
	win->handleResizeEvent(new_win_x, new_win_y);
}

void EventManager::handleMouseButtonEvent(GLFWwindow* glfw_win, int button, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(glfw_win);
	int mouse_xy[2];

	win->getCursorPosition(mouse_xy);
	win->stage->handleMouseButtonEvent(*win, mouse_xy, button, action, mods);
}
