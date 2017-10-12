#include <list>

#include "Event.h"
#include "GPU.h"
#include "Stage.h"
#include "WindowManager.h"

#include "EventManager.h"

using namespace bWidgetsDemo;
using namespace bWidgets; // Less verbose


bool EventManager::is_dragging = false;

EventManager& EventManager::ensureEventManager()
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
	glfwSetWindowSizeCallback(glfw_window, handleWindowResizeEvent);
	glfwSetCursorPosCallback(glfw_window, handleMouseMovementEvent);
	glfwSetMouseButtonCallback(glfw_window, handleMouseButtonEvent);
	glfwSetScrollCallback(glfw_window, handleMouseScrollEvent);
}

bool EventManager::isDragging()
{
	return is_dragging;
}

void EventManager::handleWindowResizeEvent(GLFWwindow* glfw_win, int new_win_x, int new_win_y)
{
	Window* win = (Window*)glfwGetWindowUserPointer(glfw_win);
	win->handleResizeEvent(new_win_x, new_win_y);
}

bwWidget::MouseButton EventManager::convertGlfwMouseButton(int glfw_button)
{
	switch (glfw_button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			return bwWidget::MOUSE_BUTTON_LEFT;
		case GLFW_MOUSE_BUTTON_RIGHT:
			return bwWidget::MOUSE_BUTTON_RIGHT;
	}

	return bwWidget::MOUSE_BUTTON_UNKNOWN;
}

MouseEvent::MouseEventType EventManager::convertGlfwMouseButtonAction(int glfw_action)
{
	switch (glfw_action) {
		case GLFW_PRESS:
			return MouseEvent::MOUSE_EVENT_PRESS;
		case GLFW_RELEASE:
			return MouseEvent::MOUSE_EVENT_RELEASE;
	}

	return MouseEvent::MOUSE_EVENT_UNKNOWN;
}

void EventManager::handleMouseMovementEvent(GLFWwindow* glfw_win, double /*x*/, double /*y*/)
{
	const Window* win = (Window*)glfwGetWindowUserPointer(glfw_win);
	const bwPoint& position = win->getCursorPosition();
	MouseEvent* event;

	event = new MouseEvent(MouseEvent::MOUSE_EVENT_MOVE, bwWidget::MOUSE_BUTTON_UNKNOWN, position);
	if (is_dragging) {
		win->stage->handleMouseDragEvent(*event);
	}
	else {
		win->stage->handleMouseMovementEvent(*event);
	}

	delete event;
}

void EventManager::handleMouseButtonEvent(GLFWwindow* glfw_win, int glfw_button, int glfw_action, int /*glfw_mods*/)
{
	const Window* win = (Window*)glfwGetWindowUserPointer(glfw_win);
	const bwPoint& position = win->getCursorPosition();
	const MouseEvent::MouseEventType action_type = convertGlfwMouseButtonAction(glfw_action);
	const bwWidget::MouseButton mouse_button = convertGlfwMouseButton(glfw_button);
	MouseEvent* event;

	if (action_type == MouseEvent::MOUSE_EVENT_PRESS) {
		is_dragging = true;
	}
	else if (action_type == MouseEvent::MOUSE_EVENT_RELEASE) {
		is_dragging = false;
	}

	event = new MouseEvent(action_type, mouse_button, position);
	win->stage->handleMouseButtonEvent(*event);

	delete event;
}

void EventManager::handleMouseScrollEvent(
        GLFWwindow* glfw_win,
        double /*value_x*/, double value_y)
{
	const Window* win = (Window*)glfwGetWindowUserPointer(glfw_win);
	const MouseEvent::MouseEventType event_type = (value_y > 0) ?
	                                                    MouseEvent::MOUSE_EVENT_SCROLL_UP :
	                                                    MouseEvent::MOUSE_EVENT_SCROLL_DOWN;
	const bwPoint& position = win->getCursorPosition();
	std::unique_ptr<MouseEvent> event;

	event = std::unique_ptr<MouseEvent>(new MouseEvent(event_type, bwWidget::MOUSE_BUTTON_WHEEL, position));
	win->stage->handleMouseScrollEvent(*event);
}
